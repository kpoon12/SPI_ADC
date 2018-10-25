import time
import socket

ops = bytearray([0xAA, 0x00])
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.settimeout(5)
s.connect(("10.65.4.142",700))


while(True):
    print time.strftime('%X %x %Z')
    s.send(ops)
    data = s.recv(255)
    print data
s.close()