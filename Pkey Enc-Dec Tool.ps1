using namespace System
using namespace System.Management.Automation
using namespace System.Runtime.InteropServices
Import-Module NativeInteropLib -ErrorAction Stop

$IsForge = ([PSTypeName]'LibTSforge.SPP.ProductConfig').Type

$objs   = (Join-Path $PSScriptRoot "SppDll\sppobjs.dll")
$winob  = (Join-Path $PSScriptRoot "SppDll\sppwinob.dll")
$pidGen = (Join-Path $PSScriptRoot "SppDll\pidgenx.dll")
$winRT  = (Join-Path $PSScriptRoot "SppDll\LicensingWinRT.dll")
$pidIns = (Join-Path $PSScriptRoot "SppDll\pidgenxInsider.dll")

$objs   = if (Test-Path $objs)   { $objs   } else { "sppobjs.dll"  }
$winob  = if (Test-Path $winob)  { $winob  } else { "sppwinob.dll" }
$pidGen = if (Test-Path $pidGen) { $pidGen } else { "pidgenx.dll"  }
$winRT  = if (Test-Path $winRT)  { $winRT  } else { "LicensingWinRT.dll" }
$pidIns = if (Test-Path $pidIns) { $pidIns } else { Write-Warning "Pidgex Insider Not found .!" }

# API: LicensingWinRT.dll
# __int64 __fastcall GetDownlevelPkeyData(unsigned __int8 *a1, __int64 a2, __int64 a3, __int64 a4)
# __int64 __fastcall HwidGetCurrentEx(unsigned __int8 *a1, int a2, struct _HWID **a3, unsigned int *a4, int **a5, unsigned int *a6)
# __int64 __fastcall _HWID::ConvertToShort(_HWID *this, __int64 *a2)

# Spp Store
# Get-SppStoreLicense -SkuType Windows -IgnoreEsu -Dump | ? Value -match 'current' | select -First 1

function Get-ProductHWID {
    [CmdletBinding(DefaultParameterSetName = 'Manual')]
    param (
        [Parameter(Mandatory = $true, ParameterSetName = 'Manual')]
        [string]$CdKey,

        [Parameter(Mandatory = $true, ParameterSetName = 'Store')]
        [switch]$FromStore,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "LicensingWinRT.dll",

        [Parameter(Mandatory = $false)]
        [Int64]$Offset = 0x18002DD10,

        [switch]$UseApi
    )

    function Hwid-ConvertToLargeInt {
        param ([byte[]]$Raw)

        # State tracking: Use [int64] internally to prevent overflow during calculation
        $S = [PSCustomObject]@{ P = 28; L = [int64]0; H = [int64]0; S = 0 }

        $Pack = {
            param([int]$idx, [int]$bits, [int]$shift, [int64]$mask, [bool]$isHigh, $sShift)
        
            $cnt = [BitConverter]::ToUInt16($Raw, $idx * 2)
            if ($cnt -eq 0) { return }

            $v4 = [BitConverter]::ToUInt16($Raw, $S.P)
            for ($i=0; $i -lt $cnt; $i++) {
                $val = [BitConverter]::ToUInt16($Raw, $S.P + ($i * 2))
                if (($val -band 1) -eq 0) { $v4 = $val; break }
            }
            $S.S = ($v4 -band 1)
        
            $m = (1 -shl $bits) - 1
            $hash = $m -band ($v4 -shr 1)
            if ($hash -eq 0) { $hash = $m }

            # Logic: Perform bitwise as Int64, then mask to 32-bit to mimic register overflow
            if ($isHigh) {
                $valToXor = ([int64]$hash -shl $shift)
                $S.H = ($S.H -bxor (($S.H -bxor $valToXor) -band $mask)) -band 0xFFFFFFFF
            } else {
                $valToXor = ([int64]$hash -shl $shift)
                $S.L = ($S.L -bxor (($S.L -bxor $valToXor) -band $mask)) -band 0xFFFFFFFF
            }

            if ($null -ne $sShift) {
                $S.L = ($S.L -bxor (($S.L -bxor ($S.L -bor ($S.S -shl $sShift))) -band 0x7C0)) -band 0xFFFFFFFF
            }

            $S.P += (2 * $cnt)
        }

        # Header Logic
        $v3 = [BitConverter]::ToUInt16($Raw, 26)
        if ($v3) { 
            $v6 = (($v3 -shr 1) -band 0x3F)
            $S.L = if ($v6) { [int64]$v6 } else { [int64]63 }
        }

        $v8 = [BitConverter]::ToUInt16($Raw, 24)
        if ($v8) { 
            $v9 = (($v8 -shr 1) -band 7)
            $v10 = if ($v9) { $v9 } else { 7 }
            $S.H = ([int64]$v10 -shl 29) -band 0xFFFFFFFF
        }

        # Word 2-10 (Full Implementation)
        &$Pack 2  7 21 0xFE00000    $false 6
        &$Pack 3  4 28 0xF0000000   $false $null
        &$Pack 4  7 9  0xFE00       $true  7
        &$Pack 5  5 21 0x3E00000    $true  $null
        &$Pack 6  5 16 0x1F0000     $true  8
        &$Pack 7  6 3  0x1F8        $true  9
    
        $S.P += (2 * [BitConverter]::ToUInt16($Raw, 16)) 
    
        &$Pack 9  10 11 0x1FF800    $false 10
        &$Pack 10 3  26 0x1C000000   $true  $null

        # Merge High and Low into final 64-bit ID
        $final = ([int64]$S.H -shl 32) -bor ([uint32]$S.L)
        return $final
    }
    
    try {
        # --- Parameter Set: Store ---
        if ($PSCmdlet.ParameterSetName -eq 'Store') {
            if(!([PSTypeName]'LibTSforge.SPP.ProductConfig').Type) {
                Write-Warning "Missing nececery libraries !"
                Write-Warning "Please load first PkeyConsole"
                return
            }
            $dataBlock = Get-SppStoreLicense -SkuType Windows -IgnoreEsu -Dump | 
                            Where-Object Value -match 'current' | 
                            Select-Object -First 1

            if ($dataBlock) {
                # Assuming .Data contains the 0x118 buffer
                return [PSCustomObject]@{
                    Success   = $true
                    Source    = "SPP Store"
                    HResult   = "0x00000000"
                    HWIDPtr   = New-IntPtr -Data ($dataBlock.Raw)
                    RawBytes  = $dataBlock.Raw
                    ShortHWID = $(if ($UseApi) { Convert-HWIDToShort -HWIDBytes $dataBlock.Raw } else { Hwid-ConvertToLargeInt -Raw $dataBlock.Raw })
                }
            }
            throw "No 'current' license block found in SPP Store."
        }

        # --- Parameter Set: Manual (DLL Invoke) ---
        $Enc        = Get-PidGenEncoder -ProductKey $Key -Modern
        $cdKeyBytes = New-IntPtr -Data $Enc.BinaryKey
        $hwidStruct = [IntPtr]::Zero
        $shortHwid  = [UInt32]0
        $a5 = [IntPtr]::Zero
        $a6 = [Uint64]0L

        $params = @(
            $cdKeyBytes,      # a1
            0,                # a2
            [ref]$hwidStruct, # a3
            [ref]$shortHwid,  # a4
            $a5,              # a5
            $a6               # a6
        )

        $hr = Invoke-UnmanagedMethod -Dll $DllPath -Function "Inner" -Values $params -Sub $Offset

        if ($hr -ge 0 -and $hwidStruct -ne [IntPtr]::Zero) {
            $structSize = 0x118 
            $byteArray = New-Object Byte[] $structSize
            [Marshal]::Copy($hwidStruct, $byteArray, 0, $structSize)

            return [PSCustomObject]@{
                Success   = $true
                Source    = "DLL Invoke"
                HResult   = "0x$($hr.ToString('X8'))"
                HWIDPtr   = $hwidStruct
                RawBytes  = $byteArray 
                ShortHWID = $(if ($UseApi) { Convert-HWIDToShort -HWIDBytes $byteArray } else { Hwid-ConvertToLargeInt -Raw $byteArray })
            }
        }
        else {
            return [PSCustomObject]@{
                Success = $false
                HResult = "0x$($hr.ToString('X8'))"
                Error   = "HWID Extract failed via DLL."
            }
        }
    }
    catch {
        Write-Error $_.Exception.Message
    }
    finally {
        # Clean up CdKey pointer if it was created
        if ($cdKeyBytes) { Free-IntPtr $cdKeyBytes }
    }
}
function Convert-HWIDToShort {
    [CmdletBinding(DefaultParameterSetName = 'FromPointer')]
    param (
        # Set 1: Provide the existing pointer
        [Parameter(Mandatory = $true, ParameterSetName = 'FromPointer')]
        [IntPtr]$HWIDStruct,

        # Set 2: Provide bytes, we create/clear the pointer
        [Parameter(Mandatory = $true, ParameterSetName = 'FromBytes')]
        [byte[]]$HWIDBytes,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "LicensingWinRT.dll",

        [Parameter(Mandatory = $false)]
        [Int64]$Offset = 0x18002E4C8
    )

    $shortOut = [Marshal]::AllocHGlobal(8)
    $localAlloc = $false # Track if we allocated the HWID pointer here

    try {
        [Marshal]::WriteInt64($shortOut, 0, 0)

        # Logic for Set 2: Convert bytes to a pointer
        if ($PSCmdlet.ParameterSetName -eq 'FromBytes') {
            $HWIDStruct = [Marshal]::AllocHGlobal($HWIDBytes.Length)
            [Marshal]::Copy($HWIDBytes, 0, $HWIDStruct, $HWIDBytes.Length)
            $localAlloc = $true
        }

        $params = @(
            $HWIDStruct,
            $shortOut
        )

        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "ConvertToShort" `
            -Values $params `
            -Sub $Offset

        if ($hr -ge 0) {
            return [PSCustomObject]@{
                Success   = $true
                ShortHWID = [Marshal]::ReadInt64($shortOut)
                HResult   = "0x$($hr.ToString('X8'))"
            }
        }

        return [PSCustomObject]@{
            Success = $false
            HResult = "0x$($hr.ToString('X8'))"
        }
    }
    finally {
        # Clean up the output buffer
        if ($shortOut -ne [IntPtr]::Zero) { 
            [Marshal]::FreeHGlobal($shortOut) 
        }

        # Clean up the HWID pointer ONLY if we created it in this function
        if ($localAlloc -and $HWIDStruct -ne [IntPtr]::Zero) {
            [Marshal]::FreeHGlobal($HWIDStruct)
        }
    }
}

# API: sppwinob.dll
# API: pidgenx.dll / sppobjs.dll
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
function Get-PidGenEncoder {
    [CmdletBinding(DefaultParameterSetName = 'Modern')]
    param (
        # ProductKey must be in BOTH sets to work everywhere
        [Parameter(Mandatory = $true, ParameterSetName = 'Manual')]
        [Parameter(Mandatory = $true, ParameterSetName = 'Modern')]
        [string]$ProductKey,

        # Only used in Manual mode
        [Parameter(Mandatory = $true, ParameterSetName = 'Manual')]
        [ValidateSet("pidgenx.dll", "sppobjs.dll")]
        [string]$DllName = 'pidgenx.dll',

        # Custom path is optional, but belongs to Manual mode
        [Parameter(Mandatory = $false, ParameterSetName = 'Manual')]
        [string]$CustomPath = '',

        # The switch that triggers the PS1-only logic
        [Parameter(Mandatory = $true, ParameterSetName = 'Modern')]
        [switch]$Modern
    )

    # Local Helper
    function EncodeKey {
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
        
        $hr = -1
        if ($Direct.IsPresent) {
            $hr = 0
            $binBytes = EncodeKey -ProductKey $Key
        } else {
            $DllPath = if ([String]::IsNullOrEmpty($CustomPath)) { $DllName } else { $CustomPath }
            $hr = Invoke-UnmanagedMethod `
                -Dll $DllPath `
                -Function "Decode" `
                -Values @($ProductKey, $binKeyPtr, [uint32]0x10, $flagPtr) `
                -Sub $subAddress
            if ($hr -ge 0) {
                $binBytes = New-Object byte[] 0x10
                [Marshal]::Copy($binKeyPtr, $binBytes, 0, 0x10)
            }
        }

        if ($hr -ge 0) {
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

    # 1. Group ID: Straight UInt16
    $groupId = [BitConverter]::ToUInt16($BinaryKey, 0)

    # 2. Serial Logic: Fixed with 0xFF mask
    $serialBytes = New-Object byte[] 4 
    for ($i = 0; $i -lt 3; $i++) {
        # Mask the result to 8 bits to stay within [byte] limits
        $rawSerial = (($BinaryKey[$i + 3] -band 0xFF) -shl 4) -bor ($BinaryKey[$i + 2] -shr 4)
        $serialBytes[$i] = [byte]($rawSerial -band 0xFF)
    }
    $serial = [BitConverter]::ToUInt32($serialBytes, 0)

    # 3. Security Logic: Fixed with 0xFF mask
    $securityBytes = New-Object byte[] 8
    for ($i = 0; $i -lt 6; $i++) {
        # Mask the result to 8 bits to stay within [byte] limits
        $rawSec = (($BinaryKey[$i + 7] -band 0xFF) -shl 6) -bor ($BinaryKey[$i + 6] -shr 2)
        $securityBytes[$i] = [byte]($rawSec -band 0xFF)
    }
    $security = [BitConverter]::ToUInt64($securityBytes, 0)

    # 4. Modern N Flag
    $isNKey = ($BinaryKey[14] -band 0x08) -ne 0

    return [Psobject]@{
        Group    = $groupId
        Serial   = $serial
        Security = $security
        IsNKey   = $isNKey
    }
}
function Invoke-IIDRequest {
    [CmdletBinding(DefaultParameterSetName = "FromFields")]
    param (
        [Parameter(Mandatory = $true, ParameterSetName = "FromStruct")]
        [byte[]]$RawStruct,

        [Parameter(Mandatory = $true, ParameterSetName = "FromForge")]
        [int]$GroupID,
        [Parameter(Mandatory = $true, ParameterSetName = "FromForge")]
        [int]$Serial,
        [Parameter(Mandatory = $true, ParameterSetName = "FromForge")]
        [long]$SecurityID,

        [Parameter(Mandatory = $true, ParameterSetName = "FromAPI")]
        [Switch]$UseApi,
        [Parameter(Mandatory = $true, ParameterSetName = "FromAPI")]
        [ValidateSet("GetPKeyData", "SLGenerateOfflineInstallationIdEx")]
        [String]$ApiMode,

        [Parameter(Mandatory = $false)]
        [long]$HWID = 0,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "pidgenx.dll",

        [ValidateSet("Retail", "Insider")]
        [Parameter(Mandatory = $false)]
        [string]$Mode = "Retail",

        [String]$Key,
        [String]$ConfigPath,
        [Nullable[Guid]]$SkuID
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

    $param = $PSCmdlet.ParameterSetName
    if ($param -match "FromStruct|FromForge") {
        if ($param -eq "FromStruct") {
        
            # Copy the whole thing 1:1
            [Array]::Copy($RawStruct, 0, $buffer, 0, [Math]::Min($RawStruct.Length, $bufSize))
    
        } elseif (($param -eq "FromForge")) {

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

    } elseif ($param -match 'FromAPI') {

        if(!([PSTypeName]'LibTSforge.SPP.ProductConfig').Type) {
            Write-Warning "Missing nececery libraries !"
            Write-Warning "Please load first PkeyConsole"
            return
        }

        if ($ApiMode -eq 'SLGenerateOfflineInstallationIdEx' -and (
            $SkuID -and $SkuID -ne [Guid]::Empty)) {
                $hSLC = Manage-SLHandle
                $ppwszInstallation = $null
                $ppwszInstallationIdPtr = [IntPtr]::Zero
                $pProductSkuId = [Guid]$SkuID
                $null = $Global:SLC::SLGenerateOfflineInstallationIdEx(
                    $hSLC, [ref]$pProductSkuId, 0, [ref]$ppwszInstallationIdPtr)
                if ($ppwszInstallationIdPtr -ne [IntPtr]::Zero) {
                    return (
                        [marshal]::PtrToStringAuto($ppwszInstallationIdPtr)
                    )
                }

        } elseif ($ApiMode -eq 'GetPKeyData') {

            $Pattern = '^[A-Z0-9]{5}-[A-Z0-9]{5}-[A-Z0-9]{5}-[A-Z0-9]{5}-[A-Z0-9]{5}$'
            if ([String]::IsNullOrEmpty($Key) -or $Key.ToUpper() -notmatch $Pattern) {
                Write-Warning "Validation Error: '$Key' does not match the 5x5 format (XXXXX-XXXXX-XXXXX-XXXXX-XXXXX)."
                return
            }
            if (-not (Test-Path $configPath)) {
                Write-Warning  "Validation Error: PKeyConfig file not found at: $configPath"
                return
            }
            return (
                Get-PKeyData -key $Key -configPath $configPath -HWID $HWID
            )
        }
    }
}

Clear-Host
Write-Host

$Key        = "7H674-NPCV7-7QVJ3-RQG68-78T77"
$SkuID      = [Guid]'ed655016-a9e8-4434-95d9-4345352c2552'
$PKeyConfig = "C:\windows\System32\spp\tokens\pkeyconfig\pkeyconfig.xrm-ms"
$EncResult  = Get-PidGenEncoder -ProductKey $Key -DllName sppobjs.dll -CustomPath $objs
#$EncResult = Get-PidGenEncoder -ProductKey $Key -DllName pidgenx.dll -CustomPath $pidGen

# Recover HWID directly from SPP Store
if ($IsForge) {
    $hwid = Get-ProductHWID -FromStore 
} elseif (-not $hwid) {
    # Generate new one using Internal Api
    $hwid = Get-ProductHWID -CdKey $Key -DllPath $winRT
}

Write-Host "--- AS BINARY OUTPUT ---" -ForegroundColor Cyan
$EncResult = Get-PidGenEncoder -ProductKey $Key -Modern
Write-Host ("Generated Bytes: {0}" -f $EncResult.HexString)
Write-Host

if ($EncResult.Success) {
    
    Write-Host "--- API CALL OUTPUT  ---" -ForegroundColor Cyan
    Write-Host "Generated Bytes: $($EncResult.HexString)"

    $Contex = Get-PidGenXContext -BinaryKey $EncResult.BinaryKey -DllPath $pidGen
    Print-PidGenReport -Result $Contex
    Print-PidGenReport -Result $Contex -AsHex

    $pKey = Get-PidGenDecoder -BinaryKey $EncResult.BinaryKey -DllPath $winob
    if ($pKey.Success) {
        Write-Host "`n--- DECODER OUTPUT (Round-Trip) ---" -ForegroundColor Green
        Write-Host "Recovered Key  : $($pKey.ProductKey)"
        
        if ($Key -eq $pKey.ProductKey) {
            Write-Host "`n[!] Match Confirmed: Logic is 100% correct." -ForegroundColor Magenta
            Write-Host
        }
    }
}

if ($IsForge) {
    $Req = Invoke-IIDRequest `
        -UseApi -ApiMode SLGenerateOfflineInstallationIdEx `
        -SkuID $SkuID
    Write-Host (" - Offline  Call Api : {0}" -f $Req)
}

$Info = Extract-KeyInfo `
    -BinaryKey $EncResult.BinaryKey
$Req = Invoke-IIDRequest `
    -GroupID $Info.Group `
    -Serial $Info.Serial `
    -SecurityID $Info.Security `
    -HWID $hwid.ShortHWID `
    -DllPath $pidIns `
    -Mode Insider
Write-Host (" - PKeyData Call Api : {0}" -f $Req.IID)

if ($IsForge) {
    $Req = Invoke-IIDRequest `
        -UseApi -ApiMode GetPKeyData `
        -Key $Key -configPath $PKeyConfig `
        -HWID $hwid.ShortHWID
    if ($Req -and $Req[3]) {
        Write-Host (" - PKeyData Call Api : {0}" -f $Req[3].Value)
    }
}

$Result = Get-PidGenXContext -ProductKey $Key -DllPath $pidIns
if ($Result.Success) {
    $Req = Invoke-IIDRequest `
        -RawStruct $Result.RawStruct `
        -HWID $hwid.ShortHWID `
        -DllPath $pidGen `
        -Mode Retail
    Write-Host (" - PKeyData Call Api : {0}" -f $Req.IID)

    Print-PidGenReport -Result $Result
    Print-PidGenReport -Result $Result -AsHex
} else {
    Write-Host "[-] Failed to decode key. HRESULT: $($Result.HResult)" -ForegroundColor Red
}