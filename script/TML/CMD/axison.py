import socket

UDP_IP = "10.65.4.147"
UDP_PORTSET = 30689
UDP_PORT = 1700
axison = bytearray([0x04, 0x00, 0x10, 0x01, 0x02, 0x17])
axisoff = bytearray([0x04, 0x00, 0x10, 0x00, 0x02, 0x16])

socket = socket.socket( socket.AF_INET, #Internet
                        socket.SOCK_DGRAM) #UDP
socket.sendto(axison, (UDP_IP, UDP_PORT))
