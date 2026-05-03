
Clear-Host
Write-Host

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

Clear-Host

# 1. Forge Logic
if ($IsForge) {
    $hwid = Get-ProductHWID -FromStore 
    Invoke-HWIDParser -Bytes $hwid.RawBytes -Label "Parse SPP Store HWID"
}

# 2. SPP Store
$hwid = Get-ProductHWID -DllPath $winRT
Invoke-HWIDParser -Bytes $hwid.RawBytes -Label "Generate HWID using WinRT Call"

# 3. laomms, GitHub
$hwid = Get-ProductHWID -Generate -Mode RTL 
Invoke-HWIDParser -Bytes $hwid.RawBytes -Label "HwidGenerator [by laomms]"

# 4. laomms, GitHub [New Version]
$hwid = Get-ProductHWID -Generate -Mode NEW
Invoke-HWIDParser -Bytes $hwid.RawBytes -Label "HwidGenerator [by laomms]"