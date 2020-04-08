@echo off

py quiver\src\devtools\qpc\qpc.py -b "base.qpc_base" -a game -r libraries vphysics -g visual_studio -m DEMEZ -mf demez_hl2 -p win32

pause
