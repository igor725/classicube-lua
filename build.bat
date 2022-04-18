@ECHO OFF
@CL /MTd /FC /Z7 /DDEBUG /I..\ClassiCube\src\ ^
/I..\cserver\Lua\LuaJIT\src\ /Feout\cc-lua.dll ^
..\ClassiCube\src\ClassiCube.lib kernel32.lib ^
..\cserver\Lua\LuaJIT\src\lua51.lib ^
/Foout\objs\ src\*.c /link /dll /noentry
IF NOT "%ERRORLEVEL%"=="0" EXIT /b 1

@COPY .\out\cc-lua.dll "D:\SteamLibrary\steamapps\common\ClassiCube\plugins\"
@COPY .\out\cc-lua.pdb "D:\SteamLibrary\steamapps\common\ClassiCube\plugins\"
