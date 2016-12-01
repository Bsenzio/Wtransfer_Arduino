# Wtransfer_Arduino
Wave file uploader for wave file on Arduino Mega

This set of files are an example for uploading files to a socket server service through arduino with low transfer time

The script detects file size and divides it for proper chunk transfer according to buffer specifications. For files lower than 1 KB the buffer can be reduced.

In this repository is the Arduino Script and a Python2.7 example file

Necessary Hardware:

Arduino Mega
Ethernet/Ethernet2 shield
SD Card board

Necessary Arduino libraries:

SdFat / proper configuration for Arduino Mega
