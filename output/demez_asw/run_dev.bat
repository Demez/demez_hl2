@echo off

set GameDir=%~dp0
set GameDir=%GameDir:~0,-1%
set cmd_line=-console -sw -noborder -w 1920 -h 1080 -override_vpk -game "%GameDir%"

"C:\Program Files (x86)\Steam\steamapps\common\Alien Swarm\swarm.exe" %cmd_line%

timeout 5
