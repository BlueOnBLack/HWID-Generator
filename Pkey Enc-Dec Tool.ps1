using namespace System
using namespace System.Management.Automation
using namespace System.Runtime.InteropServices
Import-Module NativeInteropLib -ErrorAction Stop

$IsForge = ([PSTypeName]'LibTSforge.SPP.ProductConfig').Type

# Digital certificate generation tool written in .NET
# https://github.com/laomms/HwidGenerator

if (!([PSTypeName]'HwidGetCurrentEx.CPUID').Type) {
  Import-Block -PSPath $PSCommandPath -BlockName RTL
}

if (!([PSTypeName]'HwidGetCurrentExNew.HWID').Type) {
  Import-Block -PSPath $PSCommandPath -BlockName NEW
}

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
# __int64 __fastcall CHwidDataCollectorBase::AddInstanceHash(__int64 a1, _OWORD *a2, _BOOL8 a3, int a4)
# CHardwareComponentManagerT<0>::CreateInstance(_HWIDCLASS,ISppHwidCollector * *)	.text	000000018002F4B0	00000351	00000028	00000018	R	.	.	.	.	.	T	.

# Spp Store
# Get-SppStoreLicense -SkuType Windows -IgnoreEsu -Dump | ? Value -match 'current' | select -First 1

function Get-ProductHWID {
    [CmdletBinding(DefaultParameterSetName = 'Api')]
    param (
        [Parameter(Mandatory = $true, ParameterSetName = 'Store')]
        [switch]$FromStore,

        [Parameter(Mandatory = $true, ParameterSetName = 'Generate')]
        [switch]$Generate,

        [Parameter(Mandatory = $true, ParameterSetName = 'Generate')]
        [ValidateSet("RTL", "NEW")]
        [String]$Mode = 'RTL',

        [Parameter(Mandatory = $false, ParameterSetName = 'Api')]
        [string]$DllPath = "LicensingWinRT.dll",

        [switch]$UseApi
    )    
    
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
        # --- Parameter Set: Generate ---
        if ($PSCmdlet.ParameterSetName -eq 'Generate') {
            $isNotValid = (
                !([PSTypeName]'HwidGetCurrentEx.CPUID').Type -or 
                !([PSTypeName]'HwidGetCurrentExNew.HWID').Type
            )

            if ($isNotValid) {
                throw "HwidGetCurrentEx Classes not found. Ensure the DLL/Source is loaded."
            }
            if ($Mode -eq 'RTL') {
                $byteArray = [HwidGetCurrentEx.HWID]::HwidGetCurrentEx()
            } else {
                $byteArray = [HwidGetCurrentExNew.HWID]::HwidGetCurrentEx()
            }
            $hr = 0x0
            $_HWID = New-IntPtr -Data $byteArray
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

        # --- Parameter Set: Api ---

        $WinrtDll = $DllPath
        if (-not [System.IO.Path]::IsPathRooted($WinrtDll)) {
            $WinrtDll = Join-Path $env:windir "System32\LicensingWinRT.dll"
        }
        $Offset = Get-HwidRVA -dllpath $WinrtDll

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
function Get-HWIDDetails {
    param ([byte[]]$bytes)

    if ($bytes.Count -lt 28) { return "Byte array too small" }

    # Component map aligned to ConvertToShort usage ONLY
    $Map = @(
        @{ Name = "PnP (General)";   Idx = 2 }
        @{ Name = "PnP (Secondary)"; Idx = 3 }
        @{ Name = "Hard Drive";      Idx = 4 }
        @{ Name = "PnP (USB/Misc)";  Idx = 5 }
        @{ Name = "PnP (Display)";   Idx = 6 }
        @{ Name = "Audio Adapter";   Idx = 7 }
        # Index 8 = SKIP (not a component)
        @{ Name = "Network (MAC)";   Idx = 9 }
        @{ Name = "CPU Signature";   Idx = 10 }
    )

    # Header fields (USED in ConvertToShort)
    $Header12 = [BitConverter]::ToUInt16($bytes, 24) # Index 12
    $Header13 = [BitConverter]::ToUInt16($bytes, 26) # Index 13

    # Decode seeds (matches native logic)
    $SeedLow  = if ($Header13) { (($Header13 -shr 1) -band 0x3F); if (!$?) {63} } else {0}
    if ($SeedLow -eq 0 -and $Header13) { $SeedLow = 63 }

    $SeedHigh = if ($Header12) { (($Header12 -shr 1) -band 0x7); if (!$?) {7} } else {0}
    if ($SeedHigh -eq 0 -and $Header12) { $SeedHigh = 7 }

    # Skip block (Index 8)
    $SkipWords  = [BitConverter]::ToUInt16($bytes, 16)
    $SkipBytes  = $SkipWords * 2

    # Payload pointer
    $Pointer = 28

    Write-Host
    Write-Host ("{0,-20} | {1,-10} | {2}" -f "Component", "Length", "Data Extract")
    Write-Host ("-" * 80)

    foreach ($Item in $Map) {
        $Count = [BitConverter]::ToUInt16($bytes, ($Item.Idx * 2))

        # Apply skip BEFORE network (matches native v19 usage)
        if ($Item.Idx -eq 9) {
            if ($SkipBytes -gt 0) {
                Write-Host ("{0,-20} | {1,-10} | {2}" -f "[SKIPPED BLOCK]", $SkipWords, "Offset +$SkipBytes bytes") -ForegroundColor DarkGray
                $Pointer += $SkipBytes
            }
        }

        $ByteSize = $Count * 2
        $DataHex = "None"

        if ($ByteSize -gt 0) {
            if (($Pointer + $ByteSize) -le $bytes.Count) {
                $Raw = $bytes[$Pointer..($Pointer + $ByteSize - 1)]
                $DataHex = ($Raw | ForEach-Object { $_.ToString("X2") }) -join ""
                $Pointer += $ByteSize
            } else {
                $DataHex = "[ERR: Overflow]"
            }
        }

        "{0,-20} | {1,-10} | {2}" -f $Item.Name, $Count, $DataHex
    }

    Write-Host "`n[ Header Seeds (Used in Short HWID) ]" -ForegroundColor Yellow
    Write-Host ("Short HWID : {0}" -f (Convert-HWIDToShort -HWIDBytes $hwid.RawBytes))
    Write-Host ("Index 12 (High Seed Raw): {0}" -f $Header12)
    Write-Host ("Index 13 (Low  Seed Raw): {0}" -f $Header13)
    Write-Host ("Derived High Seed (3-bit): {0}" -f $SeedHigh)
    Write-Host ("Derived Low  Seed (6-bit): {0}" -f $SeedLow)
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
function Import-Block {
    [CmdletBinding(DefaultParameterSetName = "ToFile")]
    param (
        [ValidateNotNullOrEmpty()]
        [Parameter(Mandatory=$true, Position=1)]
        [string]$PSPath,

        [ValidateNotNullOrEmpty()]
        [Parameter(Mandatory=$true, Position=0)]
        [string]$BlockName
    )

    try {
        # Managed .NET Read (significant speed boost over Get-Content)
        $content = [System.IO.File]::ReadAllText($PSPath)

        # Managed .NET Regex for extraction
        $regexPattern = "(?s)## $BlockName ##\r?\n<#\r?\n(.*?)\r?\n#>\r?\n## END ##"
        $match = [System.Text.RegularExpressions.Regex]::Match($content, $regexPattern)

        if (-not $match.Success) {
            Write-Warning "Block '## $BlockName ##' not found."
            return $false
        }

        # Cleanup Base64 and Decompress via .NET Streams
        $b64 = $match.Groups[1].Value -replace "[\r\n\s]", ""
        $data = [System.Convert]::FromBase64String($b64)
        
        $msIn = [System.IO.MemoryStream]::new($data)
        $deflate = [System.IO.Compression.DeflateStream]::new($msIn, [System.IO.Compression.CompressionMode]::Decompress)
        $msOut = [System.IO.MemoryStream]::new()
        
        $deflate.CopyTo($msOut)
        $finalBytes = $msOut.ToArray()

        # Explicit cleanup
        $deflate.Dispose(); $msIn.Dispose(); $msOut.Dispose()

        [System.Reflection.Assembly]::Load($finalBytes) | Out-Null
    } catch {
        Write-Error "Failed to process block $BlockName : $($_.Exception.Message)"
        return $false
    }
}
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

if ($IsForge) {
    $hwid = Get-ProductHWID -FromStore 
    Write-Host
    Write-Host "Parse RT Generated HWID" -ForegroundColor Magenta
    Get-HWIDDetails -Bytes $hwid.RawBytes | Format-List
}

Write-Host
$hwid = Get-ProductHWID -DllPath $winRT
Write-Host "Parse SPP Store HWID" -ForegroundColor Magenta
Get-HWIDDetails -Bytes $hwid.RawBytes | Format-List

## RTL ##
<#
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
RSvVCYBGknwdhf4cgMh8WL3/zb//Dw==
#>
## END ##

## NEW ##
<#
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
#>
## END ##