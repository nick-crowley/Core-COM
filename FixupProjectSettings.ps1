Write-Host "`nSelectively modifying project settings..."

gci .\build\*.vcxproj | ?{ $_.Name -notmatch "ALL_BUILD|ZERO_CHECK" } | %{
	Write-Host -NoNewLine -ForegroundColor Cyan (Split-Path $_ -Leaf)
	Write-Host -NoNewLine " : Setting 'Build STL Modules' to "
	Write-Host -ForegroundColor Yellow "false" 
	
	$xml = Get-Content $_
	$xml = $xml -replace "</ClCompile>", "  <BuildStlModules>false</BuildStlModules>`r`n    </ClCompile>"
	Set-Content -Path $_ -Value $xml -Encoding UTF8
}