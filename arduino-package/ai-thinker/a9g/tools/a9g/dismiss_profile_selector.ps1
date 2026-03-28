$ErrorActionPreference = 'SilentlyContinue'

$wshell = New-Object -ComObject WScript.Shell
$deadline = (Get-Date).AddSeconds(15)
$titles = @(
    'V2.01.03 by 2018-06-24',
    'CoolWatcher'
)

while ((Get-Date) -lt $deadline) {
    foreach ($title in $titles) {
        if ($wshell.AppActivate($title)) {
            Start-Sleep -Milliseconds 300
            $wshell.SendKeys('~')
            exit 0
        }
    }
    Start-Sleep -Milliseconds 250
}

exit 0
