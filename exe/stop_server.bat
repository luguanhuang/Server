
taskkill /F /IM gateserver.exe
taskkill /F /IM logserver.exe
taskkill /F /IM audioserver.exe
taskkill /F /IM gmserver.exe
taskkill /F /IM loginserver.exe

timeout /t 2 /nobreak > nul 

taskkill /F /IM gameserver.exe

timeout /t 2 /nobreak > nul 

taskkill /F /IM masterserver.exe
taskkill /F /IM controlserver.exe

timeout /t 1 /nobreak > nul 
taskkill /F /IM dbserver.exe
