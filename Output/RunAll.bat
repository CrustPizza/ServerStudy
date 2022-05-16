@echo off
start cmd /k Server.exe %1 %2
for /L %%i in (1,1,%3) do start cmd /k Client.exe %1 %2
echo Done!