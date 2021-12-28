# stm32_doom

This is my Doom port for the STM32F746G-DISCO board.

The debug connector can be connected to a serial consol to capture runtime debug log output (115200).

FS USB used for Keayboard HID interface.

HS USB used for accessing the onboard SPI Flash as a standard flash drive with FatFS.
The flash should be initialized as an external disk with the empty bin file (Flash the bin file with the ST_link utility External Loader option).
Then get your copy of the doom image file and copy it to the flash drive over the USB (Caution: copying to the SPI Flash like this is painfully slow).

After bootup the Enter key should be pressed to exit from the default USB FLash drive mode, then the game starts.

Both the external flash and the external sram have been mapped to the mcu address space.
The game is laggy beacuse of the speed of the external spi flash and the overhead of FatFS. The external SPI flash used for frame buffering and for some variable used by the doom core.
The whole project runs under FreeRTOS, it handles the USB HID input, the doom main loop, the file system handling and the usb flash mode in different tasks.

Future improvements:
- Sounds could be added (via the onboard audio codec).
- USB Flash drive or SD Card could be used instead of the onboard Flash.
- Keyboard issues (some keys could be stuck maybe not handled fast enough by the game engine)
