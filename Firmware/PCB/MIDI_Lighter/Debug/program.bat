@echo off

echo -------------------------
echo Resetting MIDI Lighter...
echo -------------------------
@start "." /B MIDI_Lighter_Reset.exe
timeout /t 3 /nobreak > NUL

echo. 
echo -----------------------
echo Erasing flash memory...
echo -----------------------
start "." /B dfu-programmer.exe atmega32u4 erase --force
timeout /t 3 /nobreak > NUL

echo. 
echo ---------------------------
echo Programming flash memory...
echo ---------------------------
start "." /B dfu-programmer.exe atmega32u4 flash ATmega32u4.hex
timeout /t 3 /nobreak > NUL

echo. 
echo ------------------------------
echo Starting MIDI Lighter again...
echo ------------------------------

start "." /B dfu-programmer.exe atmega32u4 launch --no-reset

echo Finished.