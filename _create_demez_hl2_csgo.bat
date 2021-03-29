@echo off

@REM py qpc\qpc.py -b "qpc_scripts/base.qpc_base" -a demez_game client server -g visual_studio -m DEMEZ BASE_CSGO BASE_ASW CSGO VR -mf demez_hl2_csgo -ar i386
@REM py qpc\qpc.py -b "qpc_scripts/base.qpc_base" -a demez_gamedlls client server -g visual_studio -m DEMEZ BASE_CSGO CSGO VR -mf demez_hl2_csgo -ar i386
py qpc\qpc.py -b "qpc_scripts/base.qpc_base" -a demez_gamedlls game -r libraries vscript -g visual_studio -m DEMEZ BASE_CSGO CSGO VR -mf demez_hl2_csgo -ar i386

pause
