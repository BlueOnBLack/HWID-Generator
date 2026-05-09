using namespace System
using namespace System.Management.Automation
using namespace System.Runtime.InteropServices

Clear-Host

$pidGen  = (Join-Path $PSScriptRoot "pidgenx.dll")
$objs    = (Join-Path $PSScriptRoot "sppobjs.dll")
$ClSVC   = (Join-Path $PSScriptRoot "ClipSVC.dll")
$winob   = (Join-Path $PSScriptRoot "sppwinob.dll")
$pidIns  = (Join-Path $PSScriptRoot "pidgenxIn.dll")
$comApi  = (Join-Path $PSScriptRoot "SppComApi.dll")
$clwinrt = (Join-Path $PSScriptRoot "clipwinrt.dll")
$winRT   = (Join-Path $PSScriptRoot "LicensingWinRT.dll")

$ClSVC   = if (Test-Path $ClSVC)   { $ClSVC  }  else { "$env:windir\system32\ClipSVC.dll" }
$objs    = if (Test-Path $objs)    { $objs   }  else { "$env:windir\system32\sppobjs.dll"  }
$winob   = if (Test-Path $winob)   { $winob  }  else { "$env:windir\system32\sppwinob.dll" }
$pidGen  = if (Test-Path $pidGen)  { $pidGen }  else { "$env:windir\system32\pidgenx.dll"  }
$comApi  = if (Test-Path $comApi)  { $comApi }  else { "$env:windir\system32\SppComApi.dll" }
$clwinrt = if (Test-Path $clwinrt) { $clwinrt } else { "$env:windir\system32\clipwinrt.dll" }
$winRT   = if (Test-Path $winRT)   { $winRT  }  else { "$env:windir\system32\LicensingWinRT.dll" }
$pidIns  = if (Test-Path $pidIns)  { $pidIns }  else { Write-Warning "Pidgex Insider Not found .!" }

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
                Write-Warning "HwidGetCurrentEx Classes not found. Ensure the DLL/Source is loaded."
                return
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
        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "Inner" `
            -Values $params `
            -Sub $Offset

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

# Encoder : pidgenx.dll / sppobjs.dll / ClipSVC.dll / clipwinrt.dll
# Decoder : sppwinob.dll / SppComApi.dll / LicensingWinRT.dll # LicensingDiagSpp.dll
function Encode-BinaryKey {
    [CmdletBinding(DefaultParameterSetName = 'Modern')]
    param (
        # ProductKey must be in BOTH sets to work everywhere
        [Parameter(Mandatory = $true, ParameterSetName = 'Manual')]
        [Parameter(Mandatory = $true, ParameterSetName = 'Modern')]
        [string]$ProductKey,

        # Only used in Manual mode
        [Parameter(Mandatory = $true, ParameterSetName = 'Manual')]
        [ValidateSet("pidgenx.dll", "sppobjs.dll", "ClipSVC.dll", "clipwinrt.dll")]
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

        if ($Modern.IsPresent) {
            $hr = 0
            $binBytes = EncodeKey -ProductKey $Key
        } else {
            $DllPath = if ([String]::IsNullOrEmpty($CustomPath)) { $DllName } else { $CustomPath }
            $hr = Invoke-UnmanagedMethod `
                -Dll $DllPath `
                -Function "Inner" `
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
function Decode-BinaryKey {
    [CmdletBinding(DefaultParameterSetName = 'Modern')]
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true, ParameterSetName = 'Manual')]
        [Parameter(Mandatory = $true, ParameterSetName = 'Modern')]
        [byte[]]$BinaryKey,

        [Parameter(Mandatory = $false, ParameterSetName = 'Manual')]
        [string]$DllPath = "sppwinob.dll",

        [Parameter(Mandatory = $false, ParameterSetName = 'Manual')]
        [switch]$pVoid,

        # The switch that triggers the PS1-only logic
        [Parameter(Mandatory = $true, ParameterSetName = 'Modern')]
        [switch]$Modern
    )

    # LicensingDiagSpp.dll, LicensingWinRT.dll, SppComApi.dll, SppWinOb.dll
    # __int64 __fastcall CProductKeyUtilsT<CEmptyType>::BinaryDecode(__m128i *a1, __int64 a2, unsigned __int16 **a3)
    function DecodeKey {
        param (
            [Parameter(Mandatory=$true)]
            [byte[]]$bCDKeyArray,

            [Parameter(Mandatory=$false)]
            [switch]$Log
        )

        # Clone input to v21 (like C++ __m128i copy)
        $keyData = $bCDKeyArray.Clone()

        # +2 for N` Logic Shift right [else fail]
        $Src = New-Object char[] 27

        # Character set for base-24 decoding
        $charset = "BCDFGHJKMPQRTVWXY2346789"

        # Validate input length
        if ($keyData.Length -lt 15 -or $keyData.Length -gt 16) {
            throw "Input data must be a 15 or 16 byte array."
        }

        # Win.8 key check
        if (($keyData[14] -band 0xF0) -ne 0) {
            throw "Failed to decode.!"
        }

        # N-flag
        $T = 0
        $BYTE14 = [byte]$keyData[14]
        $flag = (($BYTE14 -band 0x08) -ne 0)

        # BYTE14(v22) = (4 * (((BYTE14(v22) & 8) != 0) & 2)) | BYTE14(v22) & 0xF7;
        $keyData[14] = (4 * (([int](($BYTE14 -band 8) -ne 0)) -band 2)) -bor ($BYTE14 -band 0xF7)

        # BYTE14(v22) ^= (BYTE14(v22) ^ (4 * ((BYTE14(v22) & 8) != 0))) & 8;
        #$keyData[14] = $BYTE14 -bxor (($BYTE14 -bxor (4 * ([int](($BYTE14 -band 8) -ne 0)))) -band 8)

        # Base-24 decoding loop
        for ($idx = 24; $idx -ge 0; $idx--) {
            $last = 0
            for ($j = 14; $j -ge 0; $j--) {
                $val = $keyData[$j] + ($last -shl 8)
                $keyData[$j] = [math]::Floor($val / 0x18)
                $last = $val % 0x18
            }
            $Src[$idx] = $charset[$last]
        }

        if ($keyData[0] -ne 0) {
            throw "Invalid product key data"
        }

        # Handle N-flag
        $rev = $last -gt 13
        $pos = if ($rev) {25} else {-1}
        if ($Log) {
            $Output = (0..4 | % { -join $Src[(5*$_)..((5*$_)+4)] }) -join '-'
            Write-Warning "Before, $Output"
        }

        # Shift Left, Insert N, At position 0 >> $Src[0]=`N`
        if ($flag -and ($last -le 0)) {
            $Src[0] = [Char]78
        }
        # Shift right, Insert N, Count 1-25 [27 Base,0-24 & 2` Spacer's]
        elseif ($flag -and $rev) {
            while ($pos-- -gt $last){$Src[$pos + 1]=$Src[$pos]}
            $T, $Src[$last+1] = 1, [char]78
        }
        # Shift left, Insert N,
        elseif ($flag -and !$rev) {
            while (++$pos -lt $last){$Src[$pos] = $Src[$pos + 1]}
            $Src[$last] = [char]78
        }

        # Dynamically format 5x5 with dashes
        $Output = (0..4 | % { -join $Src[((5*$_)+$T)..((5*$_)+4+$T)] }) -join '-'
        if ($Log) {
            Write-Warning "After,  $Output"
        }
        return $Output
    }
    
    if ($Modern.IsPresent) {
        return (
            DecodeKey -bCDKeyArray $BinaryKey
        )
    }

    $Force = $false
    if ($dllPath -notmatch "sppwinob") { $Force = $true }
    $pPtr = $Force -or $pVoid.IsPresent

    $DllbaSe = $DllPath
    if (-not [System.IO.Path]::IsPathRooted($DllbaSe)) {
        $DllbaSe = Join-Path $env:windir "System32\$DllbaSe"
    }
    $Offset = Get-DecodeRVA -dllpath $DllbaSe

    if ($BinaryKey.Length -ne 16) {
        Write-Error "BinaryKey must be exactly 16 bytes (128-bit)."
        return
    }

    $poBox = [IntPtr]::Zero
    $binKeyPtr = [Marshal]::AllocHGlobal(0x10) 

    try {
        [Marshal]::Copy($BinaryKey, 0, $binKeyPtr, 16)

        $handle = $null
        if ($pPtr) {
            $poBox  = New-IntPtr -Size 0x08
            $handle = New-IntPtr -hHandle $poBox -MakeRefType
        } else {
            $handle = [ref]$poBox
        }
        $params = $binKeyPtr, 0L, $handle
        $hr = Invoke-UnmanagedMethod `
            -Dll $DllPath `
            -Function "Inner" `
            -Values $params `
            -sub $Offset

        if ($hr -ge 0) {
            $ProductKey = ''
            if ($pPtr) {
                $ProductKey = [Marshal]::PtrToStringAuto(
                    [Marshal]::ReadIntPtr($handle))
            } else {
                $ProductKey = [Marshal]::PtrToStringAuto($poBox)
            }

            return [PSCustomObject]@{
                Success    = $true
                ProductKey = $ProductKey
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
        if($pPtr) {
            if ($handle -ne 0L) {
                $tmpPtr = [Marshal]::ReadIntPtr($handle)
                if($tmpPtr -ne 0L) {
                    #Free-IntPtr $tmpPtr
                }
                Free-IntPtr $handle
            }
        } elseif ($poBox -ne 0L) {
            #Free-IntPtr $poBox
        }
    }
}

# Context : pidgenx.dll [ Retail / Insider ]
<#
Clear-Host
Write-Host

$Key
$info = Parse-BinaryKey -BinaryKey $EncResult.BinaryKey
$Source = [System.BitConverter]::ToString($EncResult.BinaryKey)
$info | Format-Table

$pKey = Pack-BinaryKey -Group $info.Group -Serial $info.Serial -Security $info.Security -IsNKey $true
$newKey = Decode-BinaryKey -BinaryKey $pKey -Modern
$newKey
$New = [System.BitConverter]::ToString($pKey)
Parse-BinaryKey -BinaryKey $pKey | Format-Table
Write-Host ("Binary -> IS MAtch ? {0}" -f ($Source -eq $new)) -ForegroundColor Green
Write-Host ("Key    -> IS MAtch ? {0}" -f ($newKey -eq $Key)) -ForegroundColor Green
Write-Host
#>
$CrcTable = @(
0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
)
function Get-KeyChecksum {
    param (
        [byte[]]$BinaryKey
    )

    # Replicate the sanitization/manipulation seen in sub_180020A1C
    # The code works on a copy (v35)
    $v35 = $BinaryKey.Clone()

    # ASM: v11 = HIWORD(_mm_srli_si128(v7, 8).m128i_u64[0]); (This is Byte 14)
    $v11 = [int]$v35[14]
    
    # ASM: v14 = v11 ^ (v11 ^ (4 * ((v11 & 8) != 0))) & 8;
    # This effectively isolates/toggles the NKey bit (Bit 3)
    $isNKeySet = ($v11 -band 8) -ne 0
    $v14 = $v11 -bxor (($v11 -bxor (4 * [int]$isNKeySet)) -band 8)

    # ASM: v35.m128i_i16[6] = v12 & 0x7F; (Byte 12)
    $v35[12] = [byte]($v35[12] -band 0x7F)

    # ASM: v17 = v14 & 0xFE; v35.m128i_i8[14] = v17; (Byte 14)
    $v17 = [byte]($v14 -band 0xFE)
    $v35[14] = $v17

    # Byte 13 is included in the CRC but is usually zeroed in the 'clean' version
    # The ASM doesn't explicitly zero it in the v35 copy before the loop, 
    # but the extractor implies it's a dedicated CRC byte.
    $v35[13] = 0

    # --- CRC-32 LOOP ---
    $v20 = [uint32]"0xFFFFFFFF"
    foreach ($b in $v35) {
        $idx = ([int]$b -bxor [int]($v20 -shr 24)) -band 0xFF
        $v20 = [uint32]((($v20 -shl 8) -bxor $global:CrcTable[$idx]) -band 0xFFFFFFFF)
    }

    # ASM: if ( v31 == (~(_WORD)v20 & 0x3FF) )
    $FinalCRC = [int]((-bnot $v20) -band 0x3FF)
    
    return $FinalCRC
}
function Parse-BinaryKey {
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
function Pack-BinaryKey {
    param (
        [uint16]$Group,
        [uint32]$Serial,
        [uint64]$Security,
        [bool]$IsNKey
    )

    # Pidgenx.dll, Retail Version, sub_180020A1C
    # Take Binary Key ANd make it Struct,
    # $group    = [Marshal]::ReadInt32(a2, 0x00)
    # $serial   = [Marshal]::ReadInt32(a2, 0x04)
    # $security = [Marshal]::ReadInt32(a2, 0x10)

    $BinaryKey = New-Object byte[] 16

    # --- PACKING DATA ---
    # Group
    $gBytes = [BitConverter]::GetBytes($Group)
    $BinaryKey[0] = $gBytes[0]
    $BinaryKey[1] = $gBytes[1]

    # Serial (4-bit offset)
    $sBytes = [BitConverter]::GetBytes($Serial)
    for ($i = 0; $i -lt 4; $i++) {
        $BinaryKey[$i + 2] = [byte]((($sBytes[$i] -band 0x0F) -shl 4) -bor ($BinaryKey[$i + 2] -band 0x0F))
        if ($i -lt 3) { $BinaryKey[$i + 3] = [byte]($sBytes[$i] -shr 4) }
    }

    # Security (6-bit offset)
    $secBytes = [BitConverter]::GetBytes($Security)
    for ($i = 0; $i -lt 7; $i++) {
        $BinaryKey[$i + 6] = [byte]((($secBytes[$i] -band 0x3F) -shl 2) -bor ($BinaryKey[$i + 6] -band 0x03))
        if ($i -lt 7) { $BinaryKey[$i + 7] = [byte]($secBytes[$i] -shr 6) }
    }

    # NKey Flag
    if ($IsNKey) { $BinaryKey[14] = [byte]($BinaryKey[14] -bor 0x08) }

    # --- CRC INJECTION ---
    # Call the checksum function
    $Crc = Get-KeyChecksum -BinaryKey $BinaryKey

    # Spread the 10 bits back into the key
    if ($Crc -band 0x01) { $BinaryKey[12] = [byte]($BinaryKey[12] -bor 0x80) }
    $BinaryKey[13] = [byte](($Crc -shr 1) -band 0xFF)
    if ($Crc -band 0x200) { $BinaryKey[14] = [byte]($BinaryKey[14] -bor 0x01) }

    return $BinaryKey
}
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
            $hr = Invoke-UnmanagedMethod `
                -Dll $DllPath `
                -Function "Inner" `
                -Values @($ProductKey, $rawPtr, 0L) `
                -Sub $offset

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
            
            $hr = Invoke-UnmanagedMethod `
                -Dll $DllPath `
                -Function "Inner" `
                -Values @($pKeyPtr, $rawPtr, [ref]$flag) `
                -Sub $offset
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

# Offline IID
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
            
            <#
            msft:rm/algorithm/pkey/2009
            .text:000000018000A050 CLSID_IProductKeyAlgorithm2009 dd 660672EFh            ; Data1
            .text:000000018000A050                                         ; DATA XREF: CAlgorithmsFactoryClient::CreateInstance(_GUID const &,_GUID const &,void * *)+79↓r
            .text:000000018000A050                                         ; CConfigCacheUtil::CreatePkeyAlgorithmObject<CAlgorithmsFactoryClient>(ushort const *,IProductKeyAlgorithm * *,_GUID *)+3E↓r ...
            .text:000000018000A050                 dw 7809h                ; Data2
            .text:000000018000A050                 dw 4CFDh                ; Data3
            .text:000000018000A050                 db 8Dh, 54h, 41h, 0B7h, 0FBh, 73h, 89h, 88h; Data4
            #>
            $IProductKeyAlgorithm2009 = [Guid]'660672EF-7809-4CFD-8D54-41B7FB738988'

            # Copy Settings
            $IProductKeyAlgorithm2009.ToByteArray().CopyTo($buffer, 0x08)
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

# Helper
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
function Invoke-HWIDParser {
    param (
        [Parameter(Mandatory=$true)]
        [byte[]]$Bytes,

        [Parameter(Mandatory=$true)]
        [string]$Label,

        [ConsoleColor]$Color = "Magenta"
    )

    Write-Host ""
    Write-Host "* $Label" -ForegroundColor $Color
    
    if ($null -eq $Bytes -or $Bytes.Count -eq 0) {
        Write-Warning "   No bytes received for $Label"
        return
    }

    Get-HWIDDetails -Bytes $Bytes | Format-List
    $lastIndex = [Array]::FindLastIndex($Bytes, [Predicate[byte]]{ $args[0] -ne 0 })
    $trimmedBytes = if ($lastIndex -ge 0) { $Bytes[0..$lastIndex] } else { $Bytes }

    Write-Host
    Write-Host "Address    00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F" -ForegroundColor Cyan
    Write-Host "-------    ----------------------- | -----------------------"

    for ($i = 0; $i -lt $trimmedBytes.Count; $i += 16) {
        $address = "{0:X8}" -f $i
        $leftPart = for ($j = 0; $j -lt 8; $j++) {
            if (($i + $j) -lt $trimmedBytes.Count) { "{0:X2}" -f $trimmedBytes[$i + $j] } else { "  " }
        }
    
        $rightPart = for ($j = 8; $j -lt 16; $j++) {
            if (($i + $j) -lt $trimmedBytes.Count) { "{0:X2}" -f $trimmedBytes[$i + $j] } else { "  " }
        }

        $leftStr  = $leftPart -join " "
        $rightStr = $rightPart -join " "    
        Write-Output "$address   $leftStr | $rightStr"
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

# RVA Offset
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

    # Will work on
    #0x18004046C -eq (Get-DecodeRVA -dllpath $env:windir\system32\sppwinob.dll)
    #0x18003ACEC -eq (Get-DecodeRVA -dllpath $env:windir\system32\SppComApi.dll)
    #0x18002CB8C -eq (Get-DecodeRVA -dllpath $env:windir\system32\LicensingWinRT.dll)
    # Will fail on
    #0x180054C18 -eq (Get-DecodeRVA -dllpath $env:windir\system32\LicensingDiagSpp.dll)

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

        # 2. Specific Backwalk (inside the patternMatched block)
        if ($patternMatched) {
            $fStart = -1
            $minJ = [Math]::Max(1, $i - 400) 

            for ($j = $i; $j -ge $minJ; $j--) {

                # 1. Existing Standard Checks
                $isStandardSub = ($b[$j] -eq 0x48 -and $b[$j+1] -eq 0x83 -and $b[$j+2] -eq 0xEC)
                $isStandardMov = ($b[$j] -eq 0x48 -and $b[$j+1] -eq 0x89 -and $b[$j+2] -eq 0x5C)
                $isFramePtr    = ($b[$j] -eq 0x48 -and $b[$j+1] -eq 0x8B -and $b[$j+2] -eq 0xC4)

                if ($isStandardSub -or $isStandardMov -or $isFramePtr) {
                    if ($b[$j-1] -eq 0xCC -or $b[$j-1] -eq 0x90 -or $b[$j-1] -eq 0xC3) {
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

Import-Module NativeInteropLib -ErrorAction Stop
$SPP = ([PSTypeName]'LibTSforge.SPP.ProductConfig').Type

# Digital certificate generation tool written in .NET
# https://github.com/laomms/HwidGenerator

if (!([PSTypeName]'HwidGetCurrentEx.CPUID').Type) {
  Import-Block -PSPath $PSCommandPath -BlockName RTL
  Import-Block -PSPath $PSCommandPath -BlockName NEW
}

Clear-Host
Write-Host

$Key        = "QPM6N-7J2WJ-P88HH-P3YRH-YY74H"
$SkuID      = [Guid]'ed655016-a9e8-4434-95d9-4345352c2552'
$PKeyConfig = "C:\windows\System32\spp\tokens\pkeyconfig\pkeyconfig.xrm-ms"

#$EncResult = Encode-BinaryKey -ProductKey $Key -DllName ClipSVC.dll   -CustomPath $ClSVC
#$EncResult = Encode-BinaryKey -ProductKey $Key -DllName clipwinrt.dll -CustomPath $clwinrt
#$EncResult = Encode-BinaryKey -ProductKey $Key -DllName sppobjs.dll   -CustomPath $objs
$EncResult  = Encode-BinaryKey -ProductKey $Key -DllName pidgenx.dll   -CustomPath $pidGen

# Recover HWID directly from SPP Store
if ($SPP) {
    $hwid = Get-ProductHWID -FromStore
} elseif (-not $hwid) {
    # Generate new one using Internal Api
    $hwid = Get-ProductHWID -DllPath $winRT
}
if (!([Int64]$hwid.HResult -eq 0L)){
    throw "HWID IS NOT VALID !"
}

Write-Host "--- AS BINARY OUTPUT ---" -ForegroundColor Cyan
$EncResult = Encode-BinaryKey -ProductKey $Key -Modern
Write-Host ("Generated Bytes: {0}" -f $EncResult.HexString)
Write-Host

if ($EncResult.Success) {
    
    Write-Host "--- API CALL OUTPUT  ---" -ForegroundColor Cyan
    Write-Host "Generated Bytes: $($EncResult.HexString)"

    $Contex = Get-PidGenXContext -BinaryKey $EncResult.BinaryKey -DllPath $pidGen
    Print-PidGenReport -Result $Contex
    Print-PidGenReport -Result $Contex -AsHex

    $pKey = Decode-BinaryKey -BinaryKey $EncResult.BinaryKey -DllPath $winob
   #$pKey = Decode-BinaryKey -BinaryKey $EncResult.BinaryKey -DllPath $winrt
   #$pKey = Decode-BinaryKey -BinaryKey $EncResult.BinaryKey -DllPath $comApi

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
Write-Host "# SLGenerateOfflineInstallationIdEx Api Call" -ForegroundColor Green
Write-Host (" - Offline  Call Api : {0}" -f $Req)

if ($SPP) {
    $Req = Invoke-IIDRequest `
        -UseApi -ApiMode GetPKeyData `
        -Key $Key -configPath $PKeyConfig `
        -HWID $hwid.ShortHWID
    if ($Req -and $Req[3]) {
        Write-Host "# GetPKeyData Api Call" -ForegroundColor Green
        Write-Host (" - PKeyData Call Api : {0}" -f $Req[3].Value)
    }
}

$Info = Parse-BinaryKey `
    -BinaryKey $EncResult.BinaryKey
$Req = Invoke-IIDRequest `
    -GroupID $Info.Group `
    -Serial $Info.Serial `
    -SecurityID $Info.Security `
    -HWID $hwid.ShortHWID `
    -DllPath $pidIns `
    -Mode Insider
Write-Host "# Custom Call {G,S,S & Salt}" -ForegroundColor Green
Write-Host (" - Internal Call Api : {0}" -f $Req.IID)

$Result = Get-PidGenXContext `
    -ProductKey $Key `
    -DllPath $pidIns
if ($Result.Success) {
    $Req = Invoke-IIDRequest `
        -RawStruct $Result.RawStruct `
        -HWID $hwid.ShortHWID `
        -DllPath $pidGen `
        -Mode Retail
    Write-Host "# Custom Call {Struct & Salt}" -ForegroundColor Green
    Write-Host (" - Internal Call Api : {0}" -f $Req.IID)

    Print-PidGenReport -Result $Result
    Print-PidGenReport -Result $Result -AsHex
} else {
    Write-Host "[-] Failed to decode key. HRESULT: $($Result.HResult)" -ForegroundColor Red
}

if ($SPP) {
    $hwid = Get-ProductHWID -FromStore 
    Write-Host
    Write-Host "Parse RT Generated HWID" -ForegroundColor Magenta
    Get-HWIDDetails -Bytes $hwid.RawBytes | Format-List
}

Write-Host
$hwid = Get-ProductHWID -DllPath $winRT
Write-Host "Parse SPP Store HWID" -ForegroundColor Magenta
Get-HWIDDetails -Bytes $hwid.RawBytes | Format-List