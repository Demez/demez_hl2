@echo off

py qpc\qpc.py -b "qpc_scripts/base.qpc_base" -a libraries -g visual_studio -m BASE_2013 QPC_CONFIG=$CONFIG -mf libs_2013 -ar i386

pause
