import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
dac = [26,19,13,6,5,11,9,10]
GPIO.setup(dac, GPIO.OUT, initial = GPIO.HIGH)
comp = 4
troyka = 17
GPIO.setup(troyka, GPIO.OUT)
GPIO.setup(comp, GPIO.IN)

def adc():
    k = 0
    for i in range(7, -1, -1):
        k+=2**i
        GPIO.output(dac, dec2bin(k))
        time.sleep(0.005)
        if GPIO.input(comp) == 0:
            k-=2**i
    return k 

def dec2bin(value):
    return[int(bit) for bit in bin(value)[2:].zfill(8)]

try:
    voltage = 0
    result = []
    GPIO.output(troyka, 1)
    time_start = time.time()
    while -time_start + time.time() < 60:
        voltage = adc()
        result.append(voltage)
        print(voltage)
    GPIO.output(troyka, 0)
    
    
    with open('fintess.txt', 'w') as f:
        for res in result:
            f.write(str(res) + '\n')
    f.close()

finally:
    GPIO.output(dac, GPIO.LOW)
    GPIO.cleanup()