#!/usr/local/bin/python3

import sys
import os
import serial
import time


def wait_for_ok_complete():
    data = ser.readline();
    print(data)
    if not data:
        print("Timeout reading over serial");
    if data == str.encode("OK COMPLETE\n"):
        return "OK"
    return "ERR"

def wait_for_ok():
    data = ser.readline();
    if not data:
        print("Timeout reading over serial");
    if data == str.encode("OK\n"):
        return "OK"
    return "ERR"


if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print(f"Usage: {sys.argv[0]} <portname> <filename>")
        sys.exit()
    port = sys.argv[1]
    filename = sys.argv[2]


filestat = os.stat(filename)
print(f"File size {filestat.st_size}")

ser = serial.Serial(port, 115200, timeout=60)
cmd = "AT+OTW "+ str(filestat.st_size) + ",2048\n"
ser.write(str.encode(cmd))

stream = open(filename, 'rb')

total_data_written = 0;
while True:
    data = stream.read(2048);
    if not data:
        break;
    ser.write(data)
    total_data_written += len(data)
    ret = wait_for_ok()
    percent_str = 'Uploaded ' + str(round(total_data_written * 100 / filestat.st_size, 2)) + '%'
    sys.stdout.write('\r%s' % percent_str)
    if ret != "OK":
        print("\nError in OTW update")
        break

ret = wait_for_ok_complete()
print("\nDone...")
