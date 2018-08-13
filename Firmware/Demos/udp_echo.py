import socket


print "Data: "

UDP_IP = "192.168.1.2"
UDP_PORT = 4567

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

count = 0

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    
    count = count+1
    #print " - received message:", data
    print "Received msg ", count, data
