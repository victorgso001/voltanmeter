import serial
import numpy
import matplotlib.pyplot as plt
from drawnow import *

voltage=[]
amps=[]
arduino = serial.Serial("/dev/ttyACM0", 9600)
plt.pause(1)
plt.ion()
cnt=0

def makegraph():
    plt.ylim(0, 300)
    plt.title("Sensor Tensão X Corrente")
    plt.grid(True)
    plt.ylabel('Tensão (V)')
    plt.plot(voltage, 'ro-', label = 'Tensão (V)')
    plt.legend(loc='upper left')
    plt2=plt.twinx()
    plt.ylim(0, 2)
    plt2.plot(amps, 'b^-', label = 'Corrente (A)')
    plt2.set_ylabel('Corrente (A)')
    plt2.ticklabel_format(useOffset=False)
    plt2.legend(loc='upper right')

while True:
    while(arduino.inWaiting()==0):
        pass
    arduinoString = arduino.readline()
    dataArray = arduinoString.decode().split(',')
    volt = float(dataArray[0])
    amp = float(dataArray[1])
    voltage.append(volt)
    amps.append(amp)
    drawnow(makegraph)
    plt.pause(.000001)
    cnt = cnt+1
    if(cnt>50):
        voltage.pop(0)
        amps.pop(0)
