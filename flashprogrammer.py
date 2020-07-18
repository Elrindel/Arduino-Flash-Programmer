#!/usr/bin/env python3

import os.path, io
import getopt, sys, time
import re
import serial
from serial.tools.list_ports import comports

def usage():
    print("""
Usage : flashprogrammer.py [OPTION]... [DATAS]

Options :
   -p, --port        Serial port
   -s, --sequence    Flash sequence, see below (default : 1)
   -c, --command     Flash command, see below (default : R)
   -a, --address     Flash memory address for R and W commands, see the format below (default : 0 for R command)
   -l, --length      Data length for R and W commands, see the format below (default : 1 for R command, data length for W command)
   -f, --file        DATAS is a file for W and R command
   -x, --hex         To get the results of the I and R commands in hexadecimal

Flash sequence :
   1  : 5555/AA, 2AAA/55, 5555/A0
   2  : 555/AA, 2AA/55, 555/A0

Flash command :
   I  : Get manufacturer ID and device ID (2 bytes)
   R  : Read address (use -a)
   W  : Write data to address (use -a, -l, -f, DATAS)
   E  : Erase memory

Address and Length format :
   [0-9]+     for a decimal value (example : 1234)
   x[0-9A-F]+ for hexadecimal value (example : x4D2)
""")
    sys.exit(2)

def display_error(error, exit=False):
    print("[!] "+error)
    if exit:
        sys.exit(2)

def query_yes_no(question, default='yes'):
    answer = None
    while answer not in ("yes", "y", "no", "n"):
        answer = input(question).lower()
        if answer == '':
            answer = default
        if answer in ("yes", "y"):
            return True
        elif answer in ("no", "n"):
            return False
        else:
            print("Please respond with 'yes' or 'no' (or 'y' or 'n').")

def serial_write_command(ser, command, hexformat = False):
    ser.write(b'X'+command if hexformat else command)
    ser.flush()

def main():
    devices = [p.device for p in comports()]
    argValidation = r"^(X[0-9A-F]{1,5})|([0-9]{1,6})$"
    try:
        opts, args = getopt.getopt(sys.argv[1:],"p:s:c:a:l:fxh",["port=", "sequence=", "command=", "address=", "length=", "file", "hex", "help"])
    except getopt.GetoptError as error:
        print(error)
        usage()

    if len(opts) == 0:
        usage()

    datas = ' '.join(args).encode()
    port = False
    sequence = False
    command = False
    address = False
    length = False
    isFile = False
    hexformat = False

    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-p", "--port"):
            if arg and arg in devices:
                port = arg
            else:
                display_error("Available serial port required :")
                display_error(", ".join([p.device for p in comports()]), True)   
        elif opt in ("-s", "--sequence"):
            arg = int(arg)
            if arg >= 1 and arg <= 2:
                sequence = arg
            else:
                display_error("Selected sequence not available")
                usage()
        elif opt in ("-c", "--command"):
            arg = arg.lower()
            if arg in ['i', 'r', 'w', 'e']:
                command = arg
            else:
                display_error("Selected command not available")
                usage()
        elif opt in ("-a", "--address"):
            arg = arg.upper()
            if re.match(argValidation, arg):
                address = arg
            else:
                display_error("Invalid address format")
                usage()
        elif opt in ("-l", "--length"):
            arg = arg.lower()
            if re.match(argValidation, arg):
                length = arg
            else:
                display_error("Invalid length format")
                usage()
        elif opt in ("-f", "--file"):
            isFile = True
        elif opt in ("-x", "--hex"):
            hexformat = True
        else:
            usage()

    if port == False:
        display_error("Serial port is required", True)

    if command == False:
        display_error("Command is required", True)
    
    if address == False:
        if command == 'r':
            address = 0
        elif command == 'w':
            display_error("Address is required", True)

    try:
        ser = serial.Serial(port, 115200, timeout=1, bytesize=8, parity=serial.PARITY_NONE, stopbits=1)
        time.sleep(2)
        if ser.readline().strip() != b"READY":
            ser.close()
            display_error("Connection timeout", True)
    except Exception as e:
        display_error("Serial port error : "+e)
        sys.exit(0)

    if sequence:
        serial_write_command(ser, b"S%d " % sequence)
        if ser.readline().strip() != b"OK":
            ser.close()
            display_error("Error when setting up the sequence", True)

    if command == 'w':
        if len(datas) == 0 or (isFile and not os.path.isfile(datas)):
            ser.close()
            display_error("The write command requires datas", True)
        
        dataSize = os.path.getsize(datas) if isFile else len(datas)
        if dataSize <= 0:
            ser.close()
            display_error("File is empty", True)
        
        if length == False:
            length = dataSize
        elif length[0] == 'X':
            length = int(length[1:], 16)
        else:
            length = int(length)

        if length > 524288:
            display_error("The length exceeds the maximum size (524288 bytes).", True)
        elif length > dataSize:
            display_error("The specified length exceeds the data length. The writing is limited to the length of the data")
        elif length < dataSize:
            display_error("You're going to write only part of the data")

        serial_write_command(ser, b"W%s %s " % (str(address).encode(), str(length if length else 1).encode()))
        if ser.readline().strip() != b'>':
            ser.close()
            display_error("Error when initializing writing", True)

        print("[+] Ready to write %d bytes" % length)
        reader = open(datas, "rb") if isFile else io.BytesIO(datas)
        readSize = 32
        writeSize = 0
        while True:
            if length-writeSize < readSize:
                readSize = length-writeSize
            ser.write(reader.read(readSize))
            ser.flush()
            if readSize == 32:
                while ser.readline().strip() != b'>':
                    pass
            writeSize += readSize
            print("[+] Write %d/%d (%d bytes)" % (writeSize, length, readSize))
            if writeSize >= length:
                break
        if readSize != 32:
            while ser.readline().strip() != b'>':
                pass
    elif command == 'r':
        if isFile:
            if os.path.isfile(datas) and not query_yes_no("Overwrite the output file? [Y/n]"):
                isFile = False
            else:
                writer = open(datas, "wb")
        serial_write_command(ser, b"R%s %s " % (str(address).encode(), str(length if length else 1).encode()), hexformat)
        while True:
            datas = ser.read(128)
            if len(datas) == 0:
                break
            
            if isFile:
                writer.write(datas)
            else:
                print(datas.decode('unicode_escape'), end='')
        if isFile:
            writer.close()
    elif command == 'i':
        serial_write_command(ser, b"I ", hexformat)
        print(ser.readline().strip().decode('unicode_escape'))
    elif command == 'e':
        if query_yes_no("Do you really want to erase the memory? [Y/n]"):
            serial_write_command(ser, b"E ")
            if ser.readline().strip() != b"OK":
                display_error("Error when erasing")

    ser.close()

if __name__ == "__main__":
    main()