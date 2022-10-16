import subprocess
import serial
import syslog
import time

port = '/dev/ttyUSB0'

ard = serial.Serial(port,115200,timeout=1)

while(True):
    msg = ard.readline()
    print ("Message from arduino: ")

    if (len(msg) != 0 ):
        msg = str(msg[0:-2])
        print(msg)

    if (str(msg) == "b'PLAYING'"):
        print('injam')
        subprocess.Popen(['vlc-ctrl',  'play'])
    
    if (str(msg) == "b'PAUSE'"):
        subprocess.Popen(['vlc-ctrl',  'pause'])

    if (str(msg) == "b'UP'"):
        subprocess.Popen(['vlc-ctrl',  'volume',  '+0.5'])

    if (str(msg) == "b'DOWN'"):
        subprocess.Popen(['vlc-ctrl',  'volume',  '-0.5'])




