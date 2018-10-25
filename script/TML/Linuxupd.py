import socket
import binascii
import sys

UDP_IP = "10.65.4.147"
UDP_PORTSET = 30689
UDP_PORT = 1700

SET_CONNECT1 = bytearray([0x01, 0xA4, 0x06])
SET_CONNECT2 = bytearray([0x03, 0x80, 0x25, 0x00, 0x00])
SET_CONNECT3 = bytearray([0x03, 0x00, 0xC2, 0x01, 0x00])

SET_INIT1 = bytearray([0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])
SET_INIT2 = bytearray([0x06, 0x07, 0x80, 0xD8, 0x01, 0x07, 0x81, 0xEE])
SET_INIT3 = bytearray([0x06, 0x07, 0x80, 0x08, 0x20, 0x00, 0x02, 0xB7])
SET_INIT4 = bytearray([0x03, 0x00, 0x96, 0x00, 0x00])
SET_INIT5 = bytearray([0x06, 0x07, 0x80, 0xD8, 0x01, 0x07, 0x81, 0xEE])

SET_CLOSE1 = bytearray([0x06, 0x00, 0x10, 0x21, 0x66, 0x5F, 0xBF, 0xBB])
SET_CLOSE2 = bytearray([0x08, 0x00, 0x10, 0x90, 0x28, 0x03, 0x66, 0x12, 0x34, 0x7F])
SET_CLOSE3 = bytearray([0x08, 0x00, 0x10, 0x90, 0x28, 0x03, 0x66, 0x12, 0x34, 0x7F])

PTR_VAR_ID = bytearray([0x06, 0x00, 0x10, 0x21, 0x66, 0x40, 0x00, 0x00])
Get_VAR_I1_ID = bytearray([0x08, 0x00, 0x10, 0xB2, 0x04, 0x07, 0x80, 0x03, 0x66, 0x00])
WRT_EERPROM_ID = bytearray([0x08, 0x00, 0x10, 0x90, 0x28, 0x03, 0x66, 0x12, 0x34, 0x00])

AXISON = bytearray([0x04, 0x00, 0x10, 0x01, 0x02, 0x17])
AXISOFF = bytearray([0x04, 0x00, 0x10, 0x00, 0x02, 0x16])
AXISRESET = bytearray([ 0x04, 0x00, 0x10, 0x04, 0x02, 0x1A])
shift = [4096, 256, 16, 1, 0]

socket = socket.socket( socket.AF_INET, #Internet
                        socket.SOCK_DGRAM) #UDP

def bytes_to_int(bytes):
    result = 0
    for b in bytes:
        result = result * 256 + int(b)
    return result

def int_to_bytes(value, length):
    result = []
    for i in range(0, length):
        result.append(value >> (i * 8) & 0xff)
    result.reverse()
    return result

def string_to_bytes(line):
    l = 5 - len(line)
    w = 0
    xx = 0
    for i in range(1, len(line)):
        if(ord(line[i-1]) >= 65):
            xx = ((ord(line[i-1]) - 65) + 10)
        else:
            xx = (ord(line[i-1]) - 48)
        #print i, xx, w
        w += xx*shift[i + l]

    result = int_to_bytes(w, 2)
    #print hex(result[0]), hex(result[1])
    return result

def calculate_sum(cmd):
    l = len(cmd)
    w = 0;
    for i in xrange(0, l-1):
        w += cmd[i]
    result = int_to_bytes(w, 2)
    cmd[l-1] = result[1]
    return cmd

def send_cmd(cmd, axis):
    cmd[2] = axis;
    set_cmd = calculate_sum(cmd)

    socket.sendto(set_cmd, (UDP_IP, UDP_PORT))
    data, addr = socket.recvfrom(1024)
    #print data
    return data

def upload_swfile(axis, swfile):
    # Enable communication to drive
    socket.sendto(SET_CONNECT1, (UDP_IP, UDP_PORTSET))
    data, addr = socket.recvfrom(1024)
    socket.sendto(SET_CONNECT2, (UDP_IP, UDP_PORTSET))
    data, addr = socket.recvfrom(1024)
    socket.sendto(SET_CONNECT3, (UDP_IP, UDP_PORTSET))
    data, addr = socket.recvfrom(1024)

    socket.sendto(SET_INIT1, (UDP_IP, UDP_PORT))
    data, addr = socket.recvfrom(1024)
    socket.sendto(SET_INIT2, (UDP_IP, UDP_PORT))
    data, addr = socket.recvfrom(1024)
    socket.sendto(SET_INIT3, (UDP_IP, UDP_PORT))
    data, addr = socket.recvfrom(1024)
    socket.sendto(SET_INIT4, (UDP_IP, UDP_PORTSET))
    data, addr = socket.recvfrom(1024)
    socket.sendto(SET_INIT5, (UDP_IP, UDP_PORT))
    data, addr = socket.recvfrom(1024)

    b = send_cmd(AXISOFF, axis)
    # Open the file with read only permit
    f = open(swfile)
    # use readline() to read the first line 
    line = f.readline()
    inputbytes = string_to_bytes(line)
    PTR_VAR_ID[5] = inputbytes[0]
    PTR_VAR_ID[6] = inputbytes[1]
    b = send_cmd(PTR_VAR_ID, axis)

    b = send_cmd(Get_VAR_I1_ID, axis)
    data, addr = socket.recvfrom(1024)
    #print "NEWLINE" , data
    #print data

    # use the read line to read further.
    # If the file is not empty keep reading one line
    # at a time, till the file is empty
    #send_cmd(axison, 0x20)
    print ("Loading sw file....")
    while line:
    #for a in range(0, 5):
        # use realine() to read next line
        line = f.readline()

        if line == '\r\n':
            line = f.readline()
            if(line):
                inputbytes = string_to_bytes(line)
                PTR_VAR_ID[5] = inputbytes[0]
                PTR_VAR_ID[6] = inputbytes[1]
                b = send_cmd(PTR_VAR_ID, axis)
                
                b = send_cmd(Get_VAR_I1_ID, axis)
                data, addr = socket.recvfrom(1024)
                #print "NEWLINE" , hex(inputbytes[0]), hex(inputbytes[1]), data
        else:
            inputbytes = string_to_bytes(line)
            WRT_EERPROM_ID[7] = inputbytes[0];
            WRT_EERPROM_ID[8] = inputbytes[1];
            b = send_cmd(WRT_EERPROM_ID, axis)
            
            b = send_cmd(Get_VAR_I1_ID, axis)
            data, addr = socket.recvfrom(1024)
            #print data

    f.close()
    print ("Finish")
    send_cmd(AXISRESET, axis)

    # Disable communication to drive
    socket.sendto(SET_CLOSE1, (UDP_IP, UDP_PORT))
    data, addr = socket.recvfrom(1024)
    socket.sendto(SET_CLOSE2, (UDP_IP, UDP_PORT))
    data, addr = socket.recvfrom(1024)
    socket.sendto(SET_CLOSE3, (UDP_IP, UDP_PORT))
    data, addr = socket.recvfrom(1024)

# argv[1] axis number 16, 32, 48, 64
# argv[2] file name "1_Travel.sw"
if __name__ == "__main__":
    print ("Technosoft update....")
    print ("argv[1] axis number 16, 32, 48, 64")
    print ("argv[2] file name 1_Travel.sw")
    a = int(sys.argv[1])
    b = sys.argv[2]
    upload_swfile(a, b)