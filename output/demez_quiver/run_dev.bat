@echo off

set cmd_line=-console -sw -noborder -game ..\..\output\demez_quiver

start "" ..\..\quiver\game\bin\win32\quiver.exe %cmd_line%
echo ..\..\quiver\game\bin\win32\quiver.exe %cmd_line%

timeout 5