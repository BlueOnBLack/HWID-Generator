using namespace System
using namespace System.Runtime.InteropServices
Import-Module NativeInteropLib -ErrorAction Stop

$objs   = (Join-Path $PSScriptRoot "SppDll\sppobjs.dll")
$winob  = (Join-Path $PSScriptRoot "SppDll\sppwinob.dll")
$pidGen = (Join-Path $PSScriptRoot "SppDll\pidgenx.dll")
$winRT  = (Join-Path $PSScriptRoot "SppDll\LicensingWinRT.dll")
$pidIns = (Join-Path $PSScriptRoot "SppDll\pidgenxInsider.dll")

# API: LicensingWinRT.dll
# __int64 __fastcall GetDownlevelPkeyData(unsigned __int8 *a1, __int64 a2, __int64 a3, __int64 a4)
# __int64 __fastcall _HWID::ConvertToShort(_HWID *this, __int64 *a2)
function Get-ProductHWID {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]$CdKey,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "LicensingWinRT.dll",

        [Parameter(Mandatory = $false)]
        [Int64]$Offset = 0x18002DD10
    )

    
    try {

        $cdKeyBytes = New-IntPtr -Data (Encode-ProductKey -ProductKey $CdKey)
        $hwidStruct = [IntPtr]::Zero
        $shortHwid  = [UInt32]0

        $a5 = [IntPtr]::Zero
        $a6 = [Uint64]0L

        $params = @(
            $cdKeyBytes,        # a1: unsigned char*
            0,                  # a2: int
            [ref]$hwidStruct,   # a3: HWID**
            [ref]$shortHwid,    # a4: uint*
            $a5,                # a5: int**
            $a6                 # a6: uint*
        )
        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "Inner" `
            -Values $params `
            -Sub $Offset

        if ($hr -ge 0) {
            return [PSCustomObject]@{
                Success     = $true
                HResult     = "0x$($hr.ToString('X8'))"
                HWIDPtr     = $hwidStruct
                ShortHWID   = $shortHwid
            }
        } 
        else {
            return [PSCustomObject]@{
                Success    = $false
                HResult    = "0x$($hr.ToString('X8'))"
                Error      = "HWID Extract failed."
            }
        }
    }
    finally {
    }
}
function Convert-ToShort {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [IntPtr]$HWIDStruct,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "LicensingWinRT.dll",

        [Parameter(Mandatory = $false)]
        [Int64]$Offset = 0x18002E4C8
    )

    # OUTPUT BUFFER (THIS IS CRITICAL)
    $shortOut = [Marshal]::AllocHGlobal(8)

    try {
        [Marshal]::WriteInt64($shortOut, 0, 0)

        $params = @(
            $HWIDStruct,   # this
            $shortOut      # out __int64*
        )

        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "ConvertToShort" `
            -Values $params `
            -Sub $Offset

        if ($hr -ge 0) {

            $value = [Marshal]::ReadInt64($shortOut)

            return [PSCustomObject]@{
                Success   = $true
                ShortHWID = $value
                HResult   = "0x$($hr.ToString('X8'))"
            }
        }

        return [PSCustomObject]@{
            Success = $false
            HResult = "0x$($hr.ToString('X8'))"
        }
    }
    finally {
        [Marshal]::FreeHGlobal($shortOut)
    }
}

# API: sppwinob.dll
function Get-PidGenDecoder {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [byte[]]$BinaryKey,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "sppwinob.dll"
    )

    if ($BinaryKey.Length -ne 16) {
        Write-Error "BinaryKey must be exactly 16 bytes (128-bit)."
        return
    }

    $binKeyPtr = [Marshal]::AllocHGlobal(0x10) 
    $outStrPtr = [Marshal]::AllocHGlobal([IntPtr]::Size)

    try {
        [Marshal]::Copy($BinaryKey, 0, $binKeyPtr, 16)
        [Marshal]::WriteIntPtr($outStrPtr, [IntPtr]::Zero)

        $decodedKeyPtr = [IntPtr]::Zero
        $params = $binKeyPtr, 0L, [ref]$decodedKeyPtr
        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "sub_18004046C" `
            -Values $params `
            -Sub 0x18004046C

        if ($hr -ge 0) {
            return [PSCustomObject]@{
                Success    = $true
                ProductKey = [Marshal]::PtrToStringAuto($decodedKeyPtr)
                HResult    = "0x$($hr.ToString('X8'))"
            }
        } 
        else {
            return [PSCustomObject]@{
                Success    = $false
                HResult    = "0x$($hr.ToString('X8'))"
                Error      = "Binary decoding failed."
            }
        }
    }
    finally {
        [Marshal]::FreeHGlobal($outStrPtr)
    }
}

# API: pidgenx.dll / sppobjs.dll
function Get-PidGenEncoder {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]$ProductKey,

        [Parameter(Mandatory = $true)]
        [ValidateSet("pidgenx.dll", "sppobjs.dll")]
        [string]$DllName,

        [Parameter(Mandatory = $false)]
        [string]$CustomPath = ''
    )

    $binKeyPtr = [Marshal]::AllocHGlobal(0x10) 
    $flagPtr   = [Marshal]::AllocHGlobal(0x04)

    try {
        [Marshal]::WriteInt64($binKeyPtr, 0, 0L)
        [Marshal]::WriteInt64($binKeyPtr, 8, 0L)
        [Marshal]::WriteInt32($flagPtr, 0, 0)

        # Set address based on the DLL chosen
        if ($DllName -eq "pidgenx.dll") {
            $subAddress = 0x180020C5C
        } elseif ($DllName -eq "sppobjs.dll") {
            $subAddress = 0x18013C8DC
        }

        $DllPath = if ([String]::IsNullOrEmpty($CustomPath)) { $DllName } else { $CustomPath }
        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "Decode" `
            -Values @($ProductKey, $binKeyPtr, [uint32]0x10, $flagPtr) `
            -Sub $subAddress

        if ($hr -ge 0) {
            $binBytes = New-Object byte[] 0x10
            [Marshal]::Copy($binKeyPtr, $binBytes, 0, 0x10)
            
            return [PSCustomObject]@{
                Success   = $true
                BinaryKey = $binBytes
                HexString = ($binBytes | ForEach-Object { "{0:X2}" -f $_ }) -join " "
                IsModernN = [bool]([Marshal]::ReadInt32($flagPtr))
                HResult   = "0x$($hr.ToString('X8'))"
            }
        } 

        return [PSCustomObject]@{ Success = $false; HResult = "0x$($hr.ToString('X8'))" }
    }
    finally {
        [Marshal]::FreeHGlobal($binKeyPtr)
        [Marshal]::FreeHGlobal($flagPtr)
    }
}

# API: pidgenx.dll [ Retail / Insider ]
function Get-PidGenXContext {
    [CmdletBinding(DefaultParameterSetName = "String")]
    param (
        [Parameter(Mandatory = $true, ParameterSetName = "String")]
        [string]$ProductKey,

        [Parameter(Mandatory = $true, ParameterSetName = "Bytes")]
        [byte[]]$BinaryKey,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "pidgenx.dll"
    )

    $contextSize = 0x60
    $rawPtr = [Marshal]::AllocHGlobal($contextSize)
    $pKeyPtr = [IntPtr]::Zero

    try {
        # Initialize Context memory to zero
        for ($i = 0; $i -lt $contextSize; $i += 8) { [Marshal]::WriteInt64($rawPtr, $i, 0L) }

        if ($PSCmdlet.ParameterSetName -eq "String") {
            # MODE: High-Level Wrapper (Insider Style)
            $hr = Invoke-UnmanagedMethod -Dll $DllPath -Function "sub_1800090B0" -Values @($ProductKey, $rawPtr, 0L) -Sub 0x1800090B0
        } else {
            # MODE: Low-Level Bit-Parser (Retail Style)
            $pKeyPtr = [Marshal]::AllocHGlobal(0x10)
            [Marshal]::Copy($BinaryKey, 0, $pKeyPtr, 0x10)
            $flag = $false
            $hr = Invoke-UnmanagedMethod -Dll $DllPath -Function "sub_180020A1C" -Values @($pKeyPtr, $rawPtr, [ref]$flag) -Sub 0x180020A1C
        }

        if ($hr -eq 0) {
            # --- AUTO-DETECTION LOGIC ---
            # If 0x08 contains data, it's a High-Level Context (Insider)
            $InsiderBuild = [Marshal]::ReadInt64($rawPtr, 0x08) -ne 0

            if ($InsiderBuild) {
                # Map Context Offsets (Insider)
                $group    = [Marshal]::ReadInt32($rawPtr, 0x18)
                $serial   = [Marshal]::ReadInt32($rawPtr, 0x20)
                $security = [Marshal]::ReadInt64($rawPtr, 0x28)
            } else {
                # Map Raw Shuffler Offsets (Retail / v20A1C)
                $group    = [Marshal]::ReadInt32($rawPtr, 0x00)
                $serial   = [Marshal]::ReadInt32($rawPtr, 0x04)
                $security = [Marshal]::ReadInt64($rawPtr, 0x10)
            }

            $rawBytes = New-Object byte[] $contextSize
            [Marshal]::Copy($rawPtr, $rawBytes, 0, $contextSize)

            return [PSCustomObject]@{
                Success      = $true
                GroupID      = $group
                Serial       = $serial
                Security     = $security
                RawStruct    = $rawBytes
                IsFullContext = $hasSignature
                HResult      = "0x00000000"
            }
        }
        else {
            return [PSCustomObject]@{ Success = $false; HResult = "0x$($hr.ToString('X8'))" }
        }
    }
    finally {
        [Marshal]::FreeHGlobal($rawPtr)
        if ($pKeyPtr -ne [IntPtr]::Zero) { [Marshal]::FreeHGlobal($pKeyPtr) }
    }
}

# Helpers
function Print-PidGenReport {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        $Result,

        [Parameter(Mandatory = $false)]
        [switch]$AsHex
    )

    if ($Result.Success) {
        # IF -AsHex IS NOT USED: Show the standard ID summary
        if (-not $AsHex) {
            Write-Host "`n[+] PID Decoder Success" -ForegroundColor Green
            Write-Host "---------------------------"
            Write-Host "Group ID : $($Result.GroupID)"
            Write-Host "Serial   : $($Result.Serial)"
            Write-Host "Security : $($Result.Security)"
            Write-Host "---------------------------"
        } 
        # IF -AsHex IS USED: Show only the Hex Dump
        else {
            Write-Host "`nFull Context Hex Dump:" -ForegroundColor Gray
            for ($i = 0; $i -lt $Result.RawStruct.Length; $i += 16) {
                $chunk = $Result.RawStruct[$i..($i+15)] | ForEach-Object { "{0:X2}" -f $_ }
                Write-Host ("{0:X4}: {1}" -f $i, ($chunk -join " ")) -ForegroundColor DarkGray
            }
        }
    } else {
        Write-Host "[-] Failed to decode key. HRESULT: $($Result.HResult)" -ForegroundColor Red
    }
}
function Encode-ProductKey {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]$ProductKey
    )

    # Standard Microsoft Base24 Alphabet
    $Alphabet = "BCDFGHJKMPQRTVWXY2346789"
    
    # Remove dashes and force uppercase
    $RawKey = $ProductKey.Replace("-", "").ToUpper()
    if ($RawKey.Length -ne 25) { throw "Key must be 25 characters." }

    # This buffer holds the mapped values (0-23)
    $Digits = New-Object byte[] 25
    $isNKey = $false
    $digitCount = 0

    # --- Part 1: Map characters to Digits ---
    # This handles the 'N' logic: if 'N' is found, we shift the 
    # current buffer and place the position index at the start.
    foreach ($char in $RawKey.ToCharArray()) {
        if ($char -eq 'N') {
            if (-not $isNKey) {
                $isNKey = $true
                # Manual shift (the 'memmove' from the DLL)
                for ($i = $digitCount; $i -gt 0; $i--) {
                    $Digits[$i] = $Digits[$i-1]
                }
                $Digits[0] = [byte]$digitCount
                $digitCount++
                continue
            }
        }

        $val = $Alphabet.IndexOf($char)
        if ($val -lt 0) { throw "Invalid character in key: $char" }
        
        $Digits[$digitCount] = [byte]$val
        $digitCount++
    }

    # --- Part 2: BigInt Conversion (Base24 -> Base256) ---
    # We convert the 25 base-24 digits into a 16-byte binary array.
    $Binary = New-Object byte[] 16
    
    foreach ($digit in $Digits) {
        $carry = [uint32]$digit
        for ($i = 0; $i -lt 16; $i++) {
            # Standard "Multiply by Base and Add" BigInt logic
            $res = ($Binary[$i] * 24) + $carry
            $Binary[$i] = [byte]($res -band 0xFF)
            $carry = $res -shr 8
        }
    }

    # --- Part 3: Apply the Modern 'N' Bit ---
    # In sub_180020C5C, this is the BYTE14 |= 8 logic.
    if ($isNKey) {
        $Binary[14] = $Binary[14] -bor 0x08
    }

    return $Binary
}
function Extract-KeyInfo {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [byte[]]$BinaryKey
    )
    
    # Pidgenx.dll, Retail Version, sub_180020A1C
    # Take Binary Key ANd make it Struct,
    # $group    = [Marshal]::ReadInt32(a2, 0x00)
    # $serial   = [Marshal]::ReadInt32(a2, 0x04)
    # $security = [Marshal]::ReadInt32(a2, 0x10)

    # 1. Group ID is a straight UInt16 from the start
    $groupId = [BitConverter]::ToUInt16($BinaryKey, 0)

    # 2. Serial Logic (The first do-while loop)
    # v24 = Byte[3], v35[v23+2] = Byte[2]
    # Serial = (16 * Byte3) | (Byte2 >> 4)
    # Note: The assembly actually builds a larger 32-bit serial across the loop, 
    # but for most keys, the primary Serial value is here:
    $serial = ([int]$BinaryKey[3] * 16) -bor ($BinaryKey[2] -shr 4)

    # 3. Security Logic (The second do-while loop)
    # v28 = Byte[7], v35[v27+6] = Byte[6]
    # Security = (Byte7 << 6) | (Byte6 >> 2)
    $security = (([int]$BinaryKey[7] -band 0xFF) -shl 6) -bor ($BinaryKey[6] -shr 2)

    # 4. Modern N Flag
    # _mm_srli_si128(v7, 8) shifts right by 8 bytes.
    # The original Byte 14 and 15 move to positions 6 and 7 of the new chunk.
    # HIWORD then grabs those bits.
    $isNKey = ($BinaryKey[14] -band 0x08) -ne 0

    return [Psobject]@{
       Group     = $groupId
       Serial    = $serial
       Security  = $security
    }
}
function Invoke-IIDRequest {
    [CmdletBinding(DefaultParameterSetName = "FromFields")]
    param (
        [Parameter(Mandatory = $true, ParameterSetName = "FromStruct")]
        [byte[]]$RawStruct,

        [Parameter(Mandatory = $true, ParameterSetName = "FromFields")] [int]$GroupID,
        [Parameter(Mandatory = $true, ParameterSetName = "FromFields")] [int]$Serial,
        [Parameter(Mandatory = $true, ParameterSetName = "FromFields")] [long]$SecurityID,

        [Parameter(Mandatory = $false)]
        [long]$HWID = 0,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "pidgenx.dll",

        [ValidateSet("Retail", "Insider")]
        [Parameter(Mandatory = $false)]
        [string]$Mode = "Retail"
    )

    <#
    // Size: 0x58 (88 bytes) // v12
    typedef struct _MSFT_PKEY_DATA {
        uint64_t Reserved;          // 0x00
        uint8_t  Signature[16];     // 0x08
        uint32_t GroupID;           // 0x18
        uint32_t ChannelID;         // 0x1C
        uint32_t Serial;            // 0x20
        uint32_t Sequence;          // 0x24
        uint64_t SecurityID;        // 0x28
        uint32_t LicenseAttributes; // 0x30
        uint32_t KeyType;           // 0x34
        uint64_t IssuanceTimestamp; // 0x38
        uint32_t ActivationFlags;   // 0x40
        uint32_t ReservedPadding;   // 0x44
        uint8_t  HardwareID[16];    // 0x48
    } MSFT_PKEY_DATA;

    // Size: 0x98 (152 bytes) // lpMem
    typedef struct _PID_OBJ {
        uint64_t       RefCount;          // 0x00
        wchar_t*       ProductIdStr;      // 0x08
        FILETIME       ValidationTime;    // 0x10
        uint32_t       DataIdSequence;    // 0x18
        uint32_t       AlignmentPadding;  // 0x1C

        // v12, Function Take V12+8
        MSFT_PKEY_DATA Data;              // 0x20 - 0x78

        void*          pPKeyConfig;       // 0x78
        void*          pKeyBits;          // 0x80
        void*          pMetadata;         // 0x88
        uint8_t        RandomSeed[8];     // 0x90
    } PID_OBJ;
    #>

    $bufSize = 0x58
    $buffer = New-Object byte[] $bufSize
    [Array]::Clear($buffer, 0, $buffer.Length)

    if ($PSCmdlet.ParameterSetName -eq "FromStruct") {
        
        # Copy the whole thing 1:1
        [Array]::Copy($RawStruct, 0, $buffer, 0, [Math]::Min($RawStruct.Length, $bufSize))
    
    } else {

        # FORGERY MODE: Start at Offset 8 to leave the header space
        # This ensures that [IntPtr]::Add($hBuffer, 8) lands on our Signature
        
        # 1. Signature (Starts at buffer index 8)
        [BitConverter]::GetBytes([int]1711698671).CopyTo($buffer, 0x08)
        [BitConverter]::GetBytes([int]1291679753).CopyTo($buffer, 0x0C)
        [BitConverter]::GetBytes([int]-1220455283).CopyTo($buffer, 0x10)
        [BitConverter]::GetBytes([int]-2004257797).CopyTo($buffer, 0x14)

        # 2. Identity (Indices adjusted +8)
        [BitConverter]::GetBytes([int]$GroupID).CopyTo($buffer, 0x18) 
        [BitConverter]::GetBytes([int]$Serial).CopyTo($buffer, 0x20)
        [BitConverter]::GetBytes([long]$SecurityID).CopyTo($buffer, 0x28)
    }

    $hBuffer = [Marshal]::AllocHGlobal($bufSize)

    try {
        # v25 = sub_180006A94((__int64)v12 + 8
        [Marshal]::Copy($buffer, 0, $hBuffer, $bufSize)

        $pOutString = ''
        $signatureBase = [IntPtr]::Add($hBuffer, 0x8)
        $params = $signatureBase, 0L, [int64]$HWID, [int64]0L, [ref]$pOutString
        [Int64]$offset = if($Mode -eq "Insider") { 0x180006A94 } else { 0x18001D8B8 }
        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "InnerCall" `
            -Values $params `
            -Sub $offset

        if ($hr -ge 0) {
            return [PSCustomObject]@{
                Success = $true
                IID     = $pOutString
                HResult = "0x$($hr.ToString('X8'))"
            }
        }
        return [PSCustomObject]@{ Success = $false; HResult = "0x$($hr.ToString('X8'))" }
    }
    finally {
        [Marshal]::FreeHGlobal($hBuffer)
    }
}

Clear-Host
Write-Host

$Key = "7H674-NPCV7-7QVJ3-RQG68-78T77"
$EncResult = Get-PidGenEncoder -ProductKey $Key -DllName sppobjs.dll -CustomPath $objs
#$EncResult = Get-PidGenEncoder -ProductKey $Key -DllName pidgenx.dll -CustomPath $pidGen

$hwid = Get-ProductHWID -CdKey $Key -DllPath $winRT
#Dump-MemoryAddress -Pointer $hwid.HWIDPtr -Length 0x118
$ShortHWID = Convert-ToShort -HWIDStruct $hwid.HWIDPtr -DllPath $winRT | select -ExpandProperty ShortHWID

Write-Host "--- AS BINARY OUTPUT ---" -ForegroundColor Cyan
$HexBytes = [BitConverter]::ToString((Encode-ProductKey -ProductKey $Key))
Write-Host "Generated Bytes: $HexBytes"
Write-Host

if ($EncResult.Success) {
    
    # Print First Results
    Write-Host "--- API CALL OUTPUT  ---" -ForegroundColor Cyan
    Write-Host "Generated Bytes: $($EncResult.HexString)"

    $Contex = Get-PidGenXContext -BinaryKey $EncResult.BinaryKey -DllPath $pidGen
    Print-PidGenReport -Result $Contex
    Print-PidGenReport -Result $Contex -AsHex

    $pKey = Get-PidGenDecoder -BinaryKey $EncResult.BinaryKey -DllPath $winob
    if ($pKey.Success) {
        Write-Host "`n--- DECODER OUTPUT (Round-Trip) ---" -ForegroundColor Green
        Write-Host "Recovered Key  : $($pKey.ProductKey)"
        
        # Print Second Results
        if ($Key -eq $pKey.ProductKey) {
            Write-Host "`n[!] Match Confirmed: Logic is 100% correct." -ForegroundColor Magenta
            Write-Host
        }
    }
}

$hSLC = Manage-SLHandle
$ppwszInstallation = $null
$ppwszInstallationIdPtr = [IntPtr]::Zero
$pProductSkuId = [Guid]'ed655016-a9e8-4434-95d9-4345352c2552'
$null = $Global:SLC::SLGenerateOfflineInstallationIdEx(
    $hSLC, [ref]$pProductSkuId, 0, [ref]$ppwszInstallationIdPtr)
if ($ppwszInstallationIdPtr -ne [IntPtr]::Zero) {
    $ppwszInstallation = [marshal]::PtrToStringAuto($ppwszInstallationIdPtr)
    Write-Host " - Offline ^ Install : $ppwszInstallation"
}

$Info = Extract-KeyInfo -BinaryKey $EncResult.BinaryKey
$Req = Invoke-IIDRequest `
    -GroupID $Info.Group `
    -Serial $Info.Serial `
    -SecurityID $Info.Security `
    -HWID $ShortHWID `
    -DllPath $pidIns `
    -Mode Insider
Write-Host (" - PKeyData Call Api : {0}" -f $Req.IID)

$res = Get-PKeyData `
    -key $Key `
    -configPath "C:\windows\System32\spp\tokens\pkeyconfig\pkeyconfig.xrm-ms" `
    -HWID $ShortHWID
Write-Host (" - PKeyData Call Api : {0}" -f $res[3].Value)

$Result = Get-PidGenXContext -ProductKey $Key -DllPath $pidIns
if ($Result.Success) {

    $Req = Invoke-IIDRequest `
        -RawStruct $Result.RawStruct `
        -HWID $ShortHWID `
        -DllPath $pidGen `
        -Mode Retail
    Write-Host (" - PKeyData Call Api : {0}" -f $Req.IID)

    Print-PidGenReport -Result $Result
    Print-PidGenReport -Result $Result -AsHex
} else {
    Write-Host "[-] Failed to decode key. HRESULT: $($Result.HResult)" -ForegroundColor Red
}