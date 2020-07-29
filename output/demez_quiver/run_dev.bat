@echo off

set cmd_line=-console -sw -noborder -game ..\..\..\output\demez_quiver

start "" ..\..\branches\quiver\game\bin\win32\quiver.exe %cmd_line%
echo ..\..\branches\quiver\game\bin\win32\quiver.exe %cmd_line%

timeout 5