using namespace System
using namespace System.Management.Automation
using namespace System.Runtime.InteropServices
Import-Module NativeInteropLib -ErrorAction Stop

$IsForge = ([PSTypeName]'LibTSforge.SPP.ProductConfig').Type

$objs   = (Join-Path $PSScriptRoot "sppobjs.dll")
$winob  = (Join-Path $PSScriptRoot "sppwinob.dll")
$pidGen = (Join-Path $PSScriptRoot "pidgenx.dll")
$winRT  = (Join-Path $PSScriptRoot "LicensingWinRT.dll")
$pidIns = (Join-Path $PSScriptRoot "pidgenxIn.dll")

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
        [Parameter(Mandatory = $true, ParameterSetName = 'Store')]
        [switch]$FromStore,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "LicensingWinRT.dll",

        [switch]$UseApi
    )

    $WinrtDll = $DllPath
    if (-not [System.IO.Path]::IsPathRooted($WinrtDll)) {
        $WinrtDll = Join-Path $env:windir "System32\LicensingWinRT.dll"
    }
    $Offset = Get-HwidRVA -dllpath $WinrtDll
    
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
                    ShortHWID = $(if ($UseApi) { Convert-HWIDToShort -HWIDBytes $dataBlock.Raw } else { Convert-HWIDToShort -HWIDBytes $dataBlock.Raw -Modern })
                }
            }
            throw "No 'current' license block found in SPP Store."
        }

        # --- Parameter Set: Manual (DLL Invoke) ---
        $_HWID = [IntPtr]::Zero
        $params = 0L, 0x0, [ref]$_HWID, [ref]0L, [ref]0L, [ref]0L
        $hr = Invoke-UnmanagedMethod -Dll $DllPath -Function "Inner" -Values $params -Sub $Offset

        if ($hr -ge 0 -and $_HWID -ne [IntPtr]::Zero) {
            $byteArray = New-Object Byte[] 0x118
            [Marshal]::Copy($_HWID, $byteArray, 0, 0x118)
            $ShortHWID = $(if ($UseApi) { Convert-HWIDToShort -HWIDBytes $byteArray } else { Convert-HWIDToShort -HWIDBytes $byteArray -Modern })
            return [PSCustomObject]@{
                Success   = $true
                Source    = "DLL Invoke"
                HResult   = "0x$($hr.ToString('X8'))"
                HWIDPtr   = $_HWID
                RawBytes  = $byteArray 
                ShortHWID = $ShortHWID
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
        [Parameter(Mandatory = $true, ParameterSetName = 'FromPointer')]
        [IntPtr]$HWIDStruct,

        [Parameter(Mandatory = $true, ParameterSetName = 'FromBytes')]
        [byte[]]$HWIDBytes,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "LicensingWinRT.dll",

        [Parameter(Mandatory = $false, ParameterSetName = 'FromBytes')]
        [Switch]$Modern
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

    $WinrtDll = $DllPath
    if (-not [System.IO.Path]::IsPathRooted($WinrtDll)) {
        $WinrtDll = Join-Path $env:windir "System32\LicensingWinRT.dll"
    }

    $shortOut = [Marshal]::AllocHGlobal(8)
    $localAlloc = $false

    try {
        [Marshal]::WriteInt64($shortOut, 0, 0)

        if ($PSCmdlet.ParameterSetName -eq 'FromBytes') {
            if ($Modern.IsPresent) {
                return Hwid-ConvertToLargeInt -Raw $HWIDBytes
            }

            $HWIDStruct = [Marshal]::AllocHGlobal($HWIDBytes.Length)
            [Marshal]::Copy($HWIDBytes, 0, $HWIDStruct, $HWIDBytes.Length)
            $localAlloc = $true
        }

        $params = $HWIDStruct, $shortOut
        $Offset = Get-ShortHwidRVA -dllpath $WinrtDll
        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "ConvertToShort" `
            -Values $params `
            -Sub $Offset

        if ($hr -ge 0) {
            return (
                [Marshal]::ReadInt64($shortOut)
            )
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
function Get-PidGenDecoder {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [byte[]]$BinaryKey,

        [Parameter(Mandatory = $false)]
        [string]$DllPath = "sppwinob.dll"
    )

    $WinobDll = $DllPath
    if (-not [System.IO.Path]::IsPathRooted($WinobDll)) {
        $WinrtDll = Join-Path $env:windir "System32\sppwinob.dll"
    }
    $Offset = Get-DecodeRVA -dllpath $WinobDll

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
            -Function "Inner" `
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
        [Marshal]::WriteInt32($flagPtr,   0, 0)
        
        $hr = -1
 
        try {
            $DllPath = if ([string]::IsNullOrEmpty($CustomPath)) { $DllName } else { $CustomPath }
            if (-not [System.IO.Path]::IsPathRooted($DllName)) {
                $DllPath = "$env:windir\System32\$DllName"
            }
            $Offset = Get-EncodeRVA -dllpath $DllPath
        }
        catch {
            Write-Host $_
        }

        if ($Direct.IsPresent) {
            $hr = 0
            $binBytes = EncodeKey -ProductKey $Key
        } else {
            $DllPath = if ([String]::IsNullOrEmpty($CustomPath)) { $DllName } else { $CustomPath }
            $hr = Invoke-UnmanagedMethod `
                -Dll $DllPath `
                -Function "Decode" `
                -Values @($ProductKey, $binKeyPtr, [uint32]0x10, $flagPtr) `
                -Sub $Offset
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
        $DllName = $DllPath
        if (-not [System.IO.Path]::IsPathRooted($DllName)) {
            $DllName = "$env:windir\System32\$DllName"
        }

        if ($PSCmdlet.ParameterSetName -eq "String") {
            # MODE: High-Level Wrapper (Insider Style)
            $offset = 0L
            try {
                $offset = Get-ContextRVA -dllpath $DllName
            } catch {}
            if ($offset -lt 0L) {
                throw "Get-ContextRVA: can't find offset"
            }
            $hr = Invoke-UnmanagedMethod -Dll $DllPath -Function "Inner" -Values @($ProductKey, $rawPtr, 0L) -Sub 0x1800090B0
        } else {
            # MODE: Low-Level Bit-Parser (Retail Style)
            $pKeyPtr = [Marshal]::AllocHGlobal(0x10)
            [Marshal]::Copy($BinaryKey, 0, $pKeyPtr, 0x10)
            $flag = $false
            
            $offset = 0L
            try {
                $offset = Get-XMMDecoderRVA -dllpath $DllName
            } catch {}
            if ($offset -lt 0L) {
                throw "Get-ContextRVA: can't find offset"
            }
            
            $hr = Invoke-UnmanagedMethod -Dll $DllPath -Function "Inner" -Values @($pKeyPtr, $rawPtr, [ref]$flag) -Sub $offset
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

    try {
        $Module = [AppDomain]::CurrentDomain.GetAssemblies() | ? { $_.ManifestModule.ScopeName -eq "OFF" } | select -Last 1
        $Global:OFF = $Module.GetTypes()[0]
    }
    catch {
        $Module = [AppDomain]::CurrentDomain.DefineDynamicAssembly("null", 1).DefineDynamicModule("OFF", $False).DefineType("null")
        @(
            @('null', 'null', [int], @()), # place holder
            @('SLOpen',                             'sppc.dll', [Int32], @([IntPtr].MakeByRefType())),
            @('SLClose',                            'sppc.dll', [Int32], @([IntPtr])),
            @('SLGenerateOfflineInstallationIdEx',  'sppc.dll', [Int32], @([IntPtr], [Guid].MakeByRefType(), [Int32], [IntPtr].MakeByRefType())),
            @('SLDepositOfflineConfirmationId',     'sppc.dll', [Int32], @([IntPtr], [Guid].MakeByRefType(), [IntPtr], [IntPtr]))

        ) | % {
            $Module.DefinePInvokeMethod(($_[0]), ($_[1]), 22, 1, [Type]($_[2]), [Type[]]($_[3]), 1, 3).SetImplementationFlags(128)
        }
        $Global:OFF = $Module.CreateType()
    }

    $bufSize = 0x58
    $buffer = New-Object byte[] $bufSize
    [Array]::Clear($buffer, 0, $buffer.Length)

    $PidDll = $DllPath
    if (-not [System.IO.Path]::IsPathRooted($PidDll)) {
        $PidDll = Join-Path $env:windir "System32\pidgenx.dll"
    }
    [Int64]$offset = Get-PidGenRVA -dllpath $PidDll

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

        if ($ApiMode -eq 'SLGenerateOfflineInstallationIdEx' -and (
            $SkuID -and $SkuID -ne [Guid]::Empty)) {
            $hSLC = 0L
            $Global:OFF::SLOpen([ref]$hSLC) | Out-Null
            $ppwszInstallation = $null
            $ppwszInstallationIdPtr = [IntPtr]::Zero
            $pProductSkuId = [Guid]$SkuID
            $null = $Global:OFF::SLGenerateOfflineInstallationIdEx(
                $hSLC, [ref]$pProductSkuId, 0, [ref]$ppwszInstallationIdPtr)
            if ($ppwszInstallationIdPtr -ne [IntPtr]::Zero) {
                return (
                    [marshal]::PtrToStringAuto($ppwszInstallationIdPtr)
                )
            }
            $Global:OFF::SLClose($hSLC) | Out-Null
        }

        if(!([PSTypeName]'LibTSforge.SPP.ProductConfig').Type) {
            Write-Warning "Missing nececery libraries !"
            Write-Warning "Please load first PkeyConsole"
            return
        }

        if ($ApiMode -eq 'GetPKeyData') {

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

#RVA HElpers
function Get-VAFromOffset {
    param (
        [byte[]]$Bytes,
        [long]$FileOffset,
        [long]$ImageBase = 0x180000000
    )

    # 1. Locate PE Header
    $pePos = [BitConverter]::ToUInt32($Bytes, 0x3C)
    
    # 2. Extract Section Metadata
    $nSections = [BitConverter]::ToUInt16($Bytes, $pePos + 0x06)
    $optHeaderSize = [BitConverter]::ToUInt16($Bytes, $pePos + 0x14)
    $sectionTable = $pePos + 0x18 + $optHeaderSize

    # 3. Iterate Sections to find where the FileOffset lives
    for ($i = 0; $i -lt $nSections; $i++) {
        $ptr = $sectionTable + ($i * 40)
        
        $rawPtr  = [BitConverter]::ToUInt32($Bytes, $ptr + 0x14)
        $rawSize = [BitConverter]::ToUInt32($Bytes, $ptr + 0x10)
        $virtAddr = [BitConverter]::ToUInt32($Bytes, $ptr + 0x0C)

        # Check if the offset falls within this section's raw data
        if ($FileOffset -ge $rawPtr -and $FileOffset -lt ($rawPtr + $rawSize)) {
            $rva = ($FileOffset - $rawPtr) + $virtAddr
            return [Int64]($ImageBase + $rva)
        }
    }
    return $null
}
function Get-ShortHwidRVA {
    param (
        [string]$dllpath = "$env:windir\system32\LicensingWinRT.dll",
        # Fix: 1C000000h in Little-Endian is 00 00 00 1C
        $pattern = [byte[]](0x00, 0x00, 0x00, 0x1C)
    )

    if (-not (Test-Path $dllpath)) { return $null }
    $dllBytes = [System.IO.File]::ReadAllBytes($dllpath)
    $constantOffset = -1

    # 1. Look for the mask 1C000000h
    for ($i = 0; $i -lt ($dllBytes.Length - 4); $i++) {
        if ($dllBytes[$i] -eq $pattern[0] -and $dllBytes[$i+1] -eq $pattern[1] -and $dllBytes[$i+2] -eq $pattern[2] -and $dllBytes[$i+3] -eq $pattern[3]) {
            
            # Check for common mask instructions: 
            # 25 (AND EAX, imm32) or 81 /4 (AND rm32, imm32)
            $isAndImm = ($dllBytes[$i-1] -eq 0x25)
            $isAndReg = ($dllBytes[$i-2] -eq 0x81 -and ($dllBytes[$i-1] -ge 0xE0 -and $dllBytes[$i-1] -le 0xE7))
            $isMovReg = ($dllBytes[$i-1] -eq 0xB8 -or $dllBytes[$i-1] -eq 0xBA) # mov eax/edx, imm32

            if ($isAndImm -or $isAndReg -or $isMovReg) {
                $constantOffset = $i
                break
            }
        }
    }

    if ($constantOffset -eq -1) { return $null }

    # 2. Look back for the TRUE Function Start (Prologue)
    # ConvertToShort is long, so we scan back further (0x800)
    $funcStartOffset = -1
    $searchLimit = [Math]::Max(0, $constantOffset - 0x800)

    for ($i = $constantOffset; $i -gt $searchLimit; $i--) {
        # Check for standard x64 prologues
        if (($dllBytes[$i] -eq 0x48 -and $dllBytes[$i+1] -eq 0x89 -and $dllBytes[$i+2] -eq 0x5C) -or  # mov [rsp+8], rbx
            ($dllBytes[$i] -eq 0x48 -and $dllBytes[$i+1] -eq 0x83 -and $dllBytes[$i+2] -eq 0xEC) -or  # sub rsp, XX
            ($dllBytes[$i] -eq 0x48 -and $dllBytes[$i+1] -eq 0x8B -and $dllBytes[$i+2] -eq 0xC4) -or  # mov rax, rsp
            ($dllBytes[$i] -eq 0x40 -and $dllBytes[$i+1] -eq 0x53)) {                                 # push rbx
            
            # Alignment validation
            if ($dllBytes[$i-1] -eq 0xCC -or $dllBytes[$i-1] -eq 0x90 -or $dllBytes[$i-1] -eq 0xC3) {
                $funcStartOffset = $i
                break
            }
        }
    }

    if ($funcStartOffset -eq -1) { return $null }
    return Get-VAFromOffset $dllBytes $funcStartOffset
}
function Get-HwidRVA {
    param (
        [string]$dllpath = "$env:windir\system32\LicensingWinRT.dll",
        $pattern = [byte[]](0x18, 0x01, 0x00, 0x00)
    )

    if (-not (Test-Path $dllpath)) { return $null }
    $dllBytes = [System.IO.File]::ReadAllBytes($dllpath)

    # --- STAGE 1: FIRST CMP FIND ---
    $firstCmpOffset = -1
    for ($i = 3; $i -lt ($dllBytes.Length - 4); $i++) {  # Start at index 3 to safely access -1, -2, and -3 offsets
        if ($dllBytes[$i] -eq $pattern[0] -and $dllBytes[$i+1] -eq $pattern[1] -and $dllBytes[$i+2] -eq $pattern[2]) {
            $isEaxCmp      = ($dllBytes[$i-1] -eq 0x3D)
            $isStandardCmp = ($dllBytes[$i-2] -eq 0x81 -and $dllBytes[$i-1] -ge 0xF8 -and $dllBytes[$i-1] -le 0xFB)
            $isRexCmp      = ($dllBytes[$i-3] -eq 0x41 -and $dllBytes[$i-2] -eq 0x81 -and $dllBytes[$i-1] -ge 0xF8 -and $dllBytes[$i-1] -le 0xFB)

            if ($isEaxCmp -or $isStandardCmp -or $isRexCmp) {
                $firstCmpOffset = $i
                break
            }
        }
    }
    if ($firstCmpOffset -eq -1) { return "Fail: CMP 0x118 not found" }

    # --- STAGE 2: SECOND 0x118 FIND (Alloc Size) ---
    $allocConstantOffset = -1
    for ($j = ($firstCmpOffset - 1); $j -gt 0; $j--) {  # Search backward from firstCmpOffset
        if ($dllBytes[$j] -eq $pattern[0] -and $dllBytes[$j+1] -eq $pattern[1] -and $dllBytes[$j+2] -eq $pattern[2]) {
            $allocConstantOffset = $j
            break
        }
    }
    if ($allocConstantOffset -eq -1) { return "Fail: Second 0x118 not found before CMP" }

    # --- UNIVERSAL STAGE 3: Multi-Prologue Precision Scan ---
    $funcStartOffset = -1
    # 0x150 (336 bytes) is the "sweet spot" for distance from the 118h constant
    $searchLimit = [Math]::Max(0, $allocConstantOffset - 0x150)

    for ($k = $allocConstantOffset; $k -gt $searchLimit; $k--) {
        # Signature A: IDA "Hot-Patch" (mov rax, rsp) -> 48 8B C4
        $isHotPatch = ($dllBytes[$k] -eq 0x48 -and $dllBytes[$k+1] -eq 0x8B -and $dllBytes[$k+2] -eq 0xC4)

        # Signature B: Standard Stack Alloc (sub rsp, XX) -> 48 83 EC
        $isSubRsp   = ($dllBytes[$k] -eq 0x48 -and $dllBytes[$k+1] -eq 0x83 -and $dllBytes[$k+2] -eq 0xEC)

        # Signature C: Standard Frame Pointer (push rbp; mov rbp, rsp) -> 55 48 89 E5 (or just 55)
        $isPushRbp  = ($dllBytes[$k] -eq 0x55 -and $dllBytes[$k+1] -eq 0x48 -and $dllBytes[$k+2] -eq 0x89)

        if ($isHotPatch -or $isSubRsp -or $isPushRbp) {
            # --- UNIVERSAL VALIDATION ---
            # Every true function start must be preceded by alignment/padding:
            # CC (int3), 90 (nop), or C3 (previous function's return)
            $prev = $dllBytes[$k-1]
            if ($prev -eq 0xCC -or $prev -eq 0x90 -or $prev -eq 0xC3) {
                $funcStartOffset = $k
                break
            }
        }
    }

    return Get-VAFromOffset $dllBytes $funcStartOffset
}
function Get-PidGenRVA {
    param (
        [string]$dllpath = "$env:windir\system32\pidgenx.dll",

        # B8731595h, 0xB8731595, -1200417387, 
        $pattern = [byte[]](0x95, 0x15, 0x73, 0xB8)
    )

    $dllBytes = [System.IO.File]::ReadAllBytes($dllpath)
    $constantOffset = -1

    for ($i = 0; $i -lt ($dllBytes.Length - 4); $i++) {
        if ($dllBytes[$i] -eq $pattern[0] -and $dllBytes[$i+1] -eq $pattern[1] -and $dllBytes[$i+2] -eq $pattern[2] -and $dllBytes[$i+3] -eq $pattern[3]) {
            $isStandardCmp = ($dllBytes[$i-2] -eq 0x81 -and $dllBytes[$i-1] -ge 0xF8 -and $dllBytes[$i-1] -le 0xFB)
            $isEaxCmp      = ($dllBytes[$i-1] -eq 0x3D)
            $isRexCmp      = ($dllBytes[$i-3] -eq 0x41 -and $dllBytes[$i-2] -eq 0x81 -and $dllBytes[$i-1] -ge 0xF8 -and $dllBytes[$i-1] -le 0xFB)
            if ($isStandardCmp -or $isEaxCmp -or $isRexCmp) {
                $constantOffset = $i
                break
            }
        }
    }

    if ($constantOffset -eq -1) { return $null }

    # 2. Look back for the TRUE Function Start (Prologue)
    $funcStartOffset = -1
    for ($i = $constantOffset; $i -gt 0; $i--) {
        if (($dllBytes[$i] -eq 0x48 -and $dllBytes[$i+1] -eq 0x89 -and $dllBytes[$i+2] -eq 0x5C) -or 
            ($dllBytes[$i] -eq 0x48 -and $dllBytes[$i+1] -eq 0x83 -and $dllBytes[$i+2] -eq 0xEC) -or
            ($dllBytes[$i] -eq 0x40 -and $dllBytes[$i+1] -eq 0x53)) {
            
            if ($dllBytes[$i-1] -eq 0xCC -or $dllBytes[$i-1] -eq 0x90 -or $dllBytes[$i-1] -eq 0xC3) {
                $funcStartOffset = $i
                break
            }
        }
    }

    if ($funcStartOffset -eq -1) { return $null }
    return Get-VAFromOffset $dllBytes $funcStartOffset
}
function Get-DecodeRVA {
    param (
        [string]$dllpath = "$env:windir\system32\sppwinob.dll"
    )

    if (-not (Test-Path $dllpath)) { return "File not found." }

    # Read all bytes in the DLL
    $b = [System.IO.File]::ReadAllBytes($dllpath)
    $len = $b.Length

    # 1. Main Loop: Optimized Pattern Scan for 'N' Assignment
    for ($i = 0; $i -lt ($len - 10); $i++) {
        
        # Fast Gate: Only check for patterns if it's either 0x41 or 0xB8
        if ($b[$i] -eq 0x41) {
            # Check the sequence (0xBB 0x4E) at $i+1, $i+2 for 'mov r11d, 4Eh'
            if ($b[$i+1] -eq 0xBB -and $b[$i+2] -eq 0x4E) {
                $patternMatched = $true
            }
            else {
                continue
            }
        } elseif ($b[$i] -eq 0xB8) {
            # Check the sequence (0x4E 0x00) at $i+1, $i+2
            if ($b[$i+1] -eq 0x4E -and $b[$i+2] -eq 0x00) {
                $patternMatched = $true
            }
            else {
                continue
            }
        } else {
            continue
        }

        # 2. Find Function Start (backwalk) if Pattern Matched
        if ($patternMatched) {
            $fStart = -1
            $minJ = [Math]::Max(1, $i - 1024)
            for ($j = $i; $j -ge $minJ; $j--) {
                # Check for prologue: sub rsp or mov [rsp]
                if (($b[$j] -eq 0x48) -and (($b[$j+1] -eq 0x83 -and $b[$j+2] -eq 0xEC) -or ($b[$j+1] -eq 0x89 -and $b[$j+2] -eq 0x5C))) {
                    # Verify padding or RET boundary (0xCC, 0x90, 0xC3)
                    if ($b[$j-1] -in 0xCC, 0x90, 0xC3) {
                        $fStart = $j
                        break
                    }
                }
            }

            if ($fStart -ne -1) {
                return Get-VAFromOffset $b $fStart
            }
        }
    }
    return "Target logic not found."
}
function Get-EncodeRVA {
    param (
        [string]$dllpath
    )

    if (-not (Test-Path $dllpath)) { return "File not found." }
    $b = [System.IO.File]::ReadAllBytes($dllpath)

    # Instruction pattern: cmp [reg], 19h
    $anchorOffset = -1
    for ($i = 0; $i -lt ($b.Length - 3); $i++) {
        if ($b[$i] -eq 0x83) {
            if ($b[$i+2] -eq 0x19 -and $b[$i-1] -eq 0x41) {
                $anchorOffset = $i - 170
                $fStart = -1
                for ($k = $anchorOffset; $k -ge ($anchorOffset - 140); $k--) {
                    if ($b[$k] -match "83|85" -and $b[$k-1] -match "83|85") {
                        $fStart = $k-2
                    }
                }
                if ($fStart -ne -1) {
                    break 
                }
            }
        }
    }

    if ($fStart -eq -1) { return "Could not locate prologue." }
    return Get-VAFromOffset $b $fStart
}
function Get-XMMDecoderRVA {
    param (
        [string]$dllpath = 'C:\Windows\System32\pidgenx.dll'
    )

    if (-not (Test-Path $dllpath)) { return 0 }
    $b = [System.IO.File]::ReadAllBytes($dllpath)

    $fStart = -1
    # 1. Primary Anchor: and ebx, 3FFh (81 E3 FF 03 00 00)
    for ($i = 0; $i -lt ($b.Length - 6); $i++) {
        if ($b[$i] -eq 0x81 -and $b[$i+1] -eq 0xE3 -and $b[$i+2] -eq 0xFF -and $b[$i+3] -eq 0x03) {
            
            # 2. Precise Backtrack
            # Since the mask is at +0x128 (296 decimal), we jump back 300 
            # to land just slightly before the function start.
            $anchorOffset = $i - 300 

            for ($k = $anchorOffset; $k -le ($anchorOffset + 100); $k++) {
                # Look for the 'mov [rsp+...], rbx' (48 89 5C 24) landmark
                if ($b[$k] -eq 0x48 -and $b[$k+1] -eq 0x89 -and $b[$k+2] -eq 0x5C) {
                    $fStart = $k
                    break
                }
            }
        }
        if ($fStart -ne -1) { break }
    }

    if ($fStart -eq -1) { return 0 }
    return (Get-VAFromOffset $b $fStart)
}
function Get-ContextRVA {
    param (
        [string]$dllpath = 'C:\Windows\System32\pidgenx.dll'
    )

    if (-not (Test-Path $dllpath)) { return 0 }
    $b = [System.IO.File]::ReadAllBytes($dllpath)

    $fStart = -1

    # Search for the constant 0xF4240 (40 42 0F 00)
    for ($i = 0; $i -lt ($b.Length - 4); $i++) {
        if ($b[$i] -eq 0x40 -and $b[$i+1] -eq 0x42 -and $b[$i+2] -eq 0x0F -and $b[$i+3] -eq 0x00) {
            $offset = $i - 200

            # Search backward for '0A0h' 
            # Pattern: 48 83 EC A0 (sub rsp, 0A0h)
            for ($k = $offset; $k -gt ($offset - 50); $k--) {
                if ($b[$k] -eq 0xA0) {
                    
                    # Step 3: Search backward for 'mov rax, rsp' prologue (48 8B C4)
                    # This marks the true start of the function at 0x1800090B0
                    for ($j = $k; $j -gt ($k - 40); $j--) {
                        if ($b[$j] -eq 0x48 -and $b[$j+1] -eq 0x8B -and $b[$j+2] -eq 0xC4) {
                            $fStart = $j
                            break
                        }
                    }
                }
                if ($fStart -ne -1) { break }
            }
        }
        if ($fStart -ne -1) { break }
    }

    if ($fStart -eq -1) { return 0 }
    return (Get-VAFromOffset $b $fStart)
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
    $hwid = Get-ProductHWID -DllPath $winRT
}
if (!([Int64]$hwid.HResult -eq 0L)){
    throw "HWID IS NOT VALID !"
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

$Req = Invoke-IIDRequest `
    -UseApi -ApiMode SLGenerateOfflineInstallationIdEx `
    -SkuID $SkuID
Write-Host (" - Offline  Call Api : {0}" -f $Req)

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

$Result = Get-PidGenXContext `
    -ProductKey $Key `
    -DllPath $pidIns
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