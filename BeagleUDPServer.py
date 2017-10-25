import socket
import struct
import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.UART as UART
import serial

############UDP Server###########
    
UDP_IP = "172.16.27.22"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT)

#################################

##########Serial Setups##########

#Serial Port 1 : Drive


ser1 = serial.Serial(port = "/dev/ttyO1", baudrate=115200)

ser1.close()
ser1.open()


#Serial Port 2 : Dig & Dump
#UART.setup("UART2")

#ser2 = serial.Serial(port = 'platform-musb-hdrc.1.auto-usb-0:1:1.0', baudrate=115200)

#ser2.close()
#ser2.open()

#################################

###########GPIO Setup############

GPIO.setup('P8_8', output)
GPIO.setup('P8_10',output)
GPIO.setup('P8_12',output)
GPIO.setup('P8_14',output)
#################################

##########Data Structure########
packer = struct.Struct('B B B B')

################################
#

while True:
          
    print("Receiving data: ")
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    byte = packer.unpack(data)
    print ('Data recieved from: ', addr,'\n')
    print (byte,'\n')
          
    
    mR=(byte[0] - 127.5)*8
    mL=(byte[1] - 127.5)*8
    aL=(byte[2] - 127.5)*8
    aD=(byte[3] - 127.5)*8

    if -10>mR>10:
        mR=0
    if -10>mL>10:
        mL=0
    if -10>aL>10:
        aL=0
    if -10>aD>10:
        aD=0
    
    mcommands=(mR,mL,aL,aD)
    print mcommands

    ser1.write('!G 1 ')
    ser1.write('%d' % mR)
    ser1.write('\r\n')

    ser1.write('!G 2 ')
    ser1.write('%d' % mL)
    ser1.write('\r\n')

#    ser2.write('!G 1 ')
 #   ser2.write('%d' % aL)
  #  ser2.write('\r\n')

#    ser2.write('!G 2 ')
 #   ser2.write('%d' % aD)
  #  ser2.write('\r\n')

    ser1.write("!G 1 "+m1+'\r\n')
    ser1.write('!G 2 '+m2+'\r\n')

   # print "received message:", packer.unpack(data)
