import spidev
import time
import RPi.GPIO as GPIO
import numpy as np


########################################
#   Open, use and close SPI ADC
########################################

spi = spidev.SpiDev()

def initSpiAdc():
    spi.open(0, 0)
    spi.max_speed_hz = 1600000
    print ("SPI for ADC have been initialized")

def deinitSpiAdc():
    spi.close()
    print ("SPI cleanup finished")

def getAdc():
    adcResponse = spi.xfer2([0, 0])
    return ((adcResponse[0] & 0x1F) << 8 | adcResponse[1]) >> 1


########################################
#   Setup, use and cleanup GPIO
########################################

def waitForOpen():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(4, GPIO.IN)


    print('GPIO initialized. Wait for door opening...')

    while GPIO.input(4) < 1:
        pass

    GPIO.cleanup()
    print('The door is open. GPIO has been cleaned up. Start sampling...')


########################################
#   Save and read data
########################################

def save(samples, start, finish):
    filename = 'wave-data {}.txt'.format(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(start)))

    with open(filename, "w") as outfile:
        outfile.write('- Wave Lab\n')
        outfile.write('- Date: {}\n'.format(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time()))))
        outfile.write('- Duration: {:.2f} s\n\n'.format(finish - start))
        
        np.savetxt(outfile, np.array(samples).T, fmt='%d')

def read(filename):
    with open(filename) as f:
        lines = f.readlines()

    duration = float(lines[2].split()[2])
    samples = np.asarray(lines[4:], dtype=int)
    
    return samples, duration, len(samples)