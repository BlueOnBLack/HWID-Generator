Clear-Host
Write-Host

if ($SPP) {
    $hwid = Get-ProductHWID -FromStore 
    Invoke-HWIDParser -Bytes $hwid.RawBytes -Label "Parse SPP Store HWID"
}

$hwid = Get-ProductHWID -DllPath $winRT
Invoke-HWIDParser -Bytes $hwid.RawBytes -Label "Generate HWID using WinRT Call"

$hwid = Get-ProductHWID -Generate -Mode RTL 
Invoke-HWIDParser -Bytes $hwid.RawBytes -Label "HwidGenerator [by laomms]"

$hwid = Get-ProductHWID -Generate -Mode NEW
Invoke-HWIDParser -Bytes $hwid.RawBytes -Label "HwidGenerator [by laomms]"