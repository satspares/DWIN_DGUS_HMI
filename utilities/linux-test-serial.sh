#!/bin/bash

SerialPort=/dev/ttyUSB0 # or your serial port eg. /dev/ttyACM0
#or using /dev/serial/by-path
#SerialPort=/dev/serial/by-path/pci-0000:07:00.1-usb-0:5:1.0-port0  # or your serial port

stty -F $SerialPort speed 115200 cs8 -cstopb -parenb raw -echo
exec 3<$SerialPort
  cat <&3 > tty.hex &
  PID=$!
    # turn page to page 1
    # printf "%b" '\x5A\xA5\x07\x82\x00\x84\x5A\x01\x00\x01' > $SerialPort
    # returns versions
      printf "%b" '\x5A\xA5\x04\x83\x00\x0F\x01' > $SerialPort
    # sound the beep if turned on in config
    # printf "%b" '\x5A\xA5\x05\x82\x00\xA0\x00\x7D' > $SerialPort
    # reboot display
    # printf "%b" '\x5A\xA5\x07\x82\x00\x04\x55\xAA\x5A\xA5' > $SerialPort
    # play sound no 2 if sound turned on in config
#     printf "%b" '\x5A\xA5\x07\x82\x00\xA0\x02\x01\x40\x00' > $SerialPort
#     Write and read vp
#     printf "%b" '\x5A\xA5\x05\x82\x10\x00\x55\xAA' > $SerialPort  # write 0x55AA to vp 0x1000
#     printf "%b" '\x5A\xA5\x04\x83\x10\x00\x01' > $SerialPort   # read vp 0x1000 back

    sleep 0.3s # or larger value for slow speeds
  kill $PID
  wait $PID 2>/dev/null

exec 3<&-
xxd -g 1 tty.hex






