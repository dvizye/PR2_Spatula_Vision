#!/usr/bin/python
#Code to use Track bars to find HSV threshold for a particular image
import cv2
import sys
import numpy as np
hsv = None
LOWER_H = 0
LOWER_S = 0
LOWER_V = 0
UPPER_H = 180 
UPPER_S = 255 
UPPER_V = 255

def setWindowTrackBars():
    cv2.namedWindow("Image",flags=cv2.cv.CV_WINDOW_NORMAL)
    cv2.namedWindow("Trackbar", flags=cv2.cv.CV_WINDOW_NORMAL)
    cv2.namedWindow("Binary Image",flags=cv2.cv.CV_WINDOW_NORMAL)
    cv2.cv.CreateTrackbar("LowerH","Trackbar",LOWER_H,UPPER_H,on_changeLH)
    cv2.cv.CreateTrackbar("UpperH","Trackbar",UPPER_H,UPPER_H,on_changeUH)
    cv2.cv.CreateTrackbar("LowerS","Trackbar",LOWER_S,UPPER_S,on_changeLS)
    cv2.cv.CreateTrackbar("UpperS","Trackbar",UPPER_S,UPPER_S,on_changeUS)
    cv2.cv.CreateTrackbar("LowerV","Trackbar",LOWER_V,UPPER_V,on_changeLV)
    cv2.cv.CreateTrackbar("UpperV","Trackbar",UPPER_V,UPPER_V,on_changeUV)
    
def getThresholdImg():
    global hsv
    if hsv.any():
        lowerb = np.array([LOWER_H,LOWER_S,LOWER_V])
        upperb = np.array([UPPER_H,UPPER_S,UPPER_V])
        dest_image = cv2.inRange(hsv, lowerb, upperb)
        cv2.imshow("Binary Image",dest_image)

def on_changeLH(value):
    global LOWER_H
    LOWER_H = value
    getThresholdImg()
def on_changeUH(value):
    global UPPER_H
    UPPER_H = value
    getThresholdImg()
def on_changeLS(value):
    global LOWER_S
    LOWER_S = value
    getThresholdImg()
def on_changeUS(value):
    global UPPER_S
    UPPER_S = value
    getThresholdImg()
def on_changeLV(value):
    global LOWER_V
    LOWER_V = value
    getThresholdImg()
def on_changeUV(value):
    global UPPER_V
    UPPER_V = value
    getThresholdImg()
    
def mouseListener(event, x, y, flags, img):
    global hsv
    if event==cv2.cv.CV_EVENT_LBUTTONDOWN:
        print "The HSV values at this point are: "
        print hsv[y, x]

def main(args):
    if len(args)!=2:               
        print "Usage : python cv_hsv_test.py <image_file>"
    else:
        img = cv2.imread(args[1],cv2.CV_LOAD_IMAGE_COLOR)  
    if (img == None):                     
        print "Could not open or find the image"
    else:
        global hsv
        hsv = cv2.cvtColor(img, cv2.cv.CV_BGR2HSV)               
	#h: 0-180, .*2; s and v: 0-255, .%255
        getThresholdImg()
        setWindowTrackBars()
        cv2.imshow("Image", img)
        cv2.cv.SetMouseCallback("Image", mouseListener, img)
        c=cv2.waitKey(0)
        #Press "ESC" to exit
        if c==27:
            cv2.destroyAllWindows()                  

if __name__ == '__main__':
    main(sys.argv)
