#!/usr/local/bin/python3

import sys
import os
import serial
import time
import re
import argparse

semver = {"major": '0', "minor": '0', "patch": '0'}

def get_parser():
    parser = argparse.ArgumentParser(prog='OTW Update Script',
    description='''This script can be used to
    switch between firmware versions on Espressif ExpressLink modules/devkits. 
    The firmware upgrade/downgrade is carried out over the same
    UART interface that accepts AT commands.
    ''')
    parser.add_argument('port', help="UART Serial Port")
    parser.add_argument('filepath', help="Firmware binary filepath")
    parser.add_argument('--blocksize', '-b', type=int, help="Size of each block of the OTW firmware update (the default is 2048)", default=2048)
    return parser

def read_semver(input_string):
    global semver
    regex = r'''(\d+(?:\.\d+)*)'''
    version_string = re.findall(regex, input_string)[0]
    semver = dict(zip(semver.keys(), version_string.split('.')))

def wait_for_ok_complete():
    data = ser.readline();
    if not data:
        print("Timeout reading over serial");
    if data == str.encode("OK COMPLETE\n"):
        return "OK"
    return "ERR"

def wait_for_ok():
    data = ser.readline();
    if int(semver["major"]) > 0:
        ok_string = "OK\r\n"
    else:
        ok_string = "OK\n"
    if not data:
        print("Timeout reading over serial");
    if data == str.encode(ok_string):
        return "OK"
    return "ERR"

def otw(port, filepath, blocksize):
    filestat = os.stat(filepath)
    print(f"File size {filestat.st_size}")

    global ser
    ser = serial.Serial(port, 115200, timeout=60)

    cmd = "AT+CONF? Version\n"
    ser.write(str.encode(cmd))

    cmd = "AT+OTW "+ str(filestat.st_size) + f",{blocksize}\n"
    ser.write(str.encode(cmd))
    data = ser.readline();
    if data:
        read_semver(data.decode())
    else:
        print("Timeout reading over serial");
        
    stream = open(filepath, 'rb')
    total_data_written = 0;

    if int(semver["major"]) > 0:
        ret = wait_for_ok()
        if ret != "OK":
                print("\nError in OTW update")
                sys.exit()

    while True:
        data = stream.read(blocksize);
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

if __name__ == "__main__":
    parser = get_parser()
    args = parser.parse_args()
    otw(args.port, args.filepath, args.blocksize)
