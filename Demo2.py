import smbus
import math
import time
import board
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd
import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt
from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import statistics

# for RPI version 1, use “bus = smbus.SMBus(0)”
bus = smbus.SMBus(1)
# This is the address we setup in the Arduino Program
address = 0x04
def cv_miniproject():
    xSize = 1024
    ySize =768
    state = 0
    new_state = 0
    prevang = 0
    camera = PiCamera(resolution=(xSize,ySize), framerate=30)
   
    camera.iso = 100
   
    sleep(2)
   
    camera.shutter_speed = camera.exposure_speed
    camera.exposure_mode = 'off'
    g = camera.awb_gains
    camera.awb_mode = 'off'
    camera.awb_gains = g
   
    camera.close()
   
    cap = cv.VideoCapture(0)
    
    flag = 0
   
    if not cap.isOpened():
        print("Cannot open camera")
        exit()
    while True:
        #bool flagend = false
    # Capture frame-by-frame
        ret, frame = cap.read()
    # if frame is read correctly ret is True
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break
    # Our operations on the frame come here
        img2HSV = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
   
        boundaries = [([80,110,50], [120,195,255])]
        
        start = (0,0)
        end = (640,130)
        
        end2 = (150,480)
        
        start2 = (449, 0)
        end3 = (640, 480)
        
        
        white = (0,0,0)
        thicc = -1
        
   
        for (lower,upper) in boundaries:
            lower = np.array(lower, dtype = "uint8")
            upper = np.array(upper, dtype = "uint8")
       
            mask = cv.inRange(img2HSV, lower, upper)
            output = cv.bitwise_and(img2HSV, img2HSV, mask = mask)
       
           
        kernel = np.ones((5,5),np.uint8)
        opening = cv.morphologyEx(output, cv.MORPH_OPEN, kernel)
        opening2 = cv.rectangle(opening, start, end, white, thicc)
        opening2 = cv.rectangle(opening2, start, end2, white, thicc)
        opening2 = cv.rectangle(opening2, start2, end3, white, thicc)
        img4gray = cv.cvtColor(opening2, cv.COLOR_BGR2GRAY)
        img4th = cv.threshold(img4gray,50,255,cv.THRESH_BINARY)
        tup = cv.findNonZero(img4th[1])
        tup2 = cv.mean(tup)
        
        #print(tup2)
        
        
        #print(tup[:,:,1])
        newarray = np.asarray(tup)
        ymin = np.min(newarray)
        
        #print(ymin)
        
        ratio = (opening.shape[1]/2)-tup2[0]
        denom = opening.shape[1]/2
        
       
        x = round(tup2[1])
        y = round(tup2[0])
    
        #if (x == y == 0):
            #lcd.clear
            #lcd.message = ("object not found")
        #print("Object at: ", y, ",", x)
        #xMax = xSize/2
        #yMax = ySize/2
        #negDiag = yMax - x*(yMax/xMax)
        #posDiag = x*(yMax/xMax)
        #if  x == y == 0:
            #state = new_state
        #elif y<= negDiag and y<= posDiag:
            #new_state = 1
        #elif y< negDiag and y >posDiag:
            #new_state = 0
        #elif y>negDiag and y> posDiag:
            #new_state = 3
        #elif y>negDiag and y <posDiag:
            #new_state = 2
        
                
        
        #if (state != new_state):
            #state = new_state
            #display = state*90
            #lcd.clear()
            #lcd.message = (str(display) +" degrees")
            #writeNumber(state)
        angle = -round(27*((ratio)/(denom)), 0)
        #print(angle)
        yCoord = []
        if(np.all(tup) == True):
            flag = 1
            #print(np.all(tup))
            #flagend = true
            #yCoord = cv.findNonZero(img4th[1])[1][0][1]
            #print(ymax)
            writeNumber(int(angle)+27)
            
        if(tup2[0] == 0 and flag == 1 and tup2[1] == 0):
            #print("end")
            angle = 200
            writeNumber(int(angle)+27)
        
        prevang = angle

    # Display the resulting frame
        cv.imshow('frame', opening)
        if cv.waitKey(1) == ord('q'):
            break
       
# When everything done, release the capture
    cap.release()
    cv.destroyAllWindows()
   
    #return values

def writeNumber(value):
    #print(value)
    bus.write_byte(address, value)
    #bus.write_byte_data(address, offset, value)
    return -1
state_dictionary = {
    #states are from y axis counter-clockwise
    0: "0 degrees",
    1: "90 degrees",
    2: "180 degrees",
    3: "270 degrees"
}
cv_miniproject()
