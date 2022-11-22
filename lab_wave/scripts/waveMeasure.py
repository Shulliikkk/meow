import time
from time import sleep
import RPi.GPIO as gpio
import numpy as np
import matplotlib.pyplot as plt

dac = [26, 19, 13, 6, 5, 11, 9, 10]

gpio.setmode(gpio.BCM)
gpio.setup(dac, gpio.OUT)
gpio.setup(4, gpio.IN)

def decbin(n): #превратить число в список двоичной записи числа
    return [int(bit) for bit in bin(n)[2:].zfill(8)]

def adc(): #считать значения с АЦП
    vlt = 0
    for i in range(8):
        gpio.output(dac, decbin(vlt + 2**(7-i)))
        sleep(0.001)
        if(gpio.input(4) == 1):
            vlt += 2**(7-i)
    return vlt

def save(samples, start, finish, height, calibrate): #сохранить измерения в файл
    if(calibrate == '1'):
        filename = 'wave-data-{}mm.txt'.format(height)
    else:
        filename = 'wave-data-{}mm-test.txt'.format(height)

    with open(filename, "w") as outfile:
        if (calibrate == '1'): outfile.write('- Wave Lab Calibration\n')
        else: outfile.write('- Wave Lab Test\n')
        outfile.write('- Date: {}\n'.format(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time()))))
        outfile.write('- Duration: {:.2f} s\n\n'.format(finish - start))
        
        np.savetxt(outfile, np.array(samples).T, fmt='%d')


try:
    arr = []
    height = input("введите уровень воды ")
    calibrate = input("1 если калибровка ")
    start = time.time()
    timelimit = 10
    while(time.time() - start < timelimit):
        vlt = adc()
        arr.append(vlt)
        print(vlt)
except KeyboardInterrupt:
    print("завершено досрочно")
finally:
    end = time.time()
    save(arr, start, end, height, calibrate)
    gpio.output(dac, 0)
    gpio.cleanup