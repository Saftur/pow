del /SY GAMEDIRECTORY\*
xcopy INSTALLERFILES\SetupIcon.ico GAMEDIRECTORY
xcopy /Y ..\Debug\*.dll GAMEDIRECTORY
xcopy /Y ..\Debug\Project6.exe GAMEDIRECTORY
ren GAMEDIRECTORY\Project6.exe "Push Of War.exe"
xcopy /YEI ..\Data GAMEDIRECTORY\Data