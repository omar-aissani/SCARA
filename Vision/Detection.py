import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt
import socket


def sendCommand(sock, UDP_IP, UDP_PORT, command, debug_var=True):
    sock.sendto((command).encode(), (UDP_IP, UDP_PORT))
    if (debug_var):
        print("_" * 10, command, " sent!", "_" * 10)


UDP_IP = "127.0.0.1"
UDP_PORT = 5065
UDP_PORT = 25001#25002
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

FLAGS = {
    'dispHelp': False,
    'showSubstraction': False,
    'showThreshold': False,
    'showDilatedMask': False,
    'showErodedMask': False,
    'drawContours': False,
    'drawCenters': True,
    'dispCountBoxes': True,
    'sendSocket': True,
}

KEYS = {
    'dispHelp': 'h',
    'showSubstraction': 'a',
    'showThreshold': 'z',
    'showDilatedMask': 'e',
    'showErodedMask': 'r',
    'drawContours': 't',
    'drawCenters': 'y',
    'dispCountBoxes': 'u',
}

txt = "(0, 0)"

A = (9, 35)
B = (465, 38)
C = (458, 739)
D = (9, 736)

# Tunable parameters
# Good values
minArea = 6000
maxArea = 15000
dilationKernalSize = 5
erosionKernalSize = 6
minThreshold = 60
maxThreshold = 255

minArea = 6000
maxArea = 15000
dilationKernalSize = 5
erosionKernalSize = 6
minThreshold = 60
maxThreshold = 255


def segmentProperties(x):
    global img

    A = {"x": x[0, 0, 0], "y": x[0, 0, 1]}
    B = {"x": x[1, 0, 0], "y": x[1, 0, 1]}
    C = {"x": x[2, 0, 0], "y": x[2, 0, 1]}
    D = {"x": x[3, 0, 0], "y": x[3, 0, 1]}

    #print(A)
    #print(B)
    #print(C)
    #print(D)

    AB = np.sqrt((A["x"] - B["x"])**2 + (A["y"] - B["y"])**2)
    BC = np.sqrt((B["x"] - C["x"])**2 + (B["y"] - C["y"])**2)
    CD = np.sqrt((C["x"] - D["x"])**2 + (C["y"] - D["y"])**2)
    DA = np.sqrt((D["x"] - A["x"])**2 + (D["y"] - A["y"])**2)

    #print("AB = %.0f, BC = %.0f, CD = %.0f, DA = %.0f" % (AB, BC, CD, DA))

    # Orientation of the Box

    #cv.imshow('img', img)

    if(CD > DA):
        E = {"x": D["x"], "y": C["y"]}
        # If you want to visualize the angle
        img = cv.circle(img, (E['x'], E['y']), 2, (0, 0, 0), -1)
        CE = np.sqrt((C["x"] - E["x"])**2 + (C["y"] - E["y"])**2)
        #CE = np.sqrt((C["x"] - D["x"])**2) #simplified form
        angle = np.arccos(CE/CD)*180/np.pi

    else:
        F = {"x": D["x"], "y": A["y"]}
        # If you want to visualize the angle
        img = cv.circle(img, (F['x'], F['y']), 2, (255, 255, 255), -1)
        AF = np.sqrt((A["x"] - F["x"])**2 + (A["y"] - F["y"])**2)
        #AF = np.sqrt((D["x"] - A["x"])**2) #simplified form
        angle = -np.arccos(AF/DA)*180/np.pi

    return angle

    #print("alpha = %.0f" % (angle))

def drawABCD(img):
    r = 5
    color = (255, 0, 0)
    img = cv.circle(img, (A[0], A[1]), r, color, -1)
    img = cv.circle(img, (B[0], B[1]), r, color, -1)
    img = cv.circle(img, (C[0], C[1]), r, color, -1)
    img = cv.circle(img, (D[0], D[1]), r, color, -1)

def mouseFun(event, x, y, flag, params):
    #print(event)
    global txt
    txt = "(" + str(x) + ", " + str(y) + ")"

    a = (B[1] - A[1]) / (B[0] - A[0])
    b = 0
    scaleX = (B[0] - A[0]) / 28
    scaleY = (D[1] - A[1]) / 43
    X = np.ceil(10*(x - A[0]) / scaleX)
    Y = np.ceil(10 * (y - A[1]) / scaleY)

    txt += " -> (" + str(X/10) + ", " + str(Y/10) + ")"

    ''' if event == cv.EVENT_LBUTTONDOWN:
        print("Left Button Down at (%s, %s)" % (x, y))
    elif event == cv.EVENT_LBUTTONUP:
        print("Left Button Up at (%s, %s)" % (x, y)) '''

def imageToRealWordCoord(x, y):  
    scaleX = (B[0] - A[0]) / 28
    scaleY = (D[1] - A[1]) / 43

    X = np.ceil(10*(x - A[0]) / scaleX)
    Y = np.ceil(10 * (y - A[1]) / scaleY)

    return X, Y

def realWordToImageCoord(X, Y):
    scaleX = (B[0] - A[0]) / 28
    scaleY = (D[1] - A[1]) / 43

    x = np.round(X*scaleX + A[0], 0)
    y = np.round(Y*scaleY + A[1], 0)
    return x, y


# Region of Interest HD
x = {'m': 0, 'M': 500}
y = {'m': 100, 'M': 860}

bg = cv.imread("Images/Doliprane/HD/bg (1).png")
bg = cv.cvtColor(bg, cv.COLOR_BGR2RGB)
plt.imshow(bg)
plt.show()
bg = bg[y['m']:y['M'], x['m']:x['M'], :]
plt.imshow(bg)
plt.show()

'''
bg_ = cv.cvtColor(bg, cv.COLOR_RGB2BGR)
CoordTxt = (50, 25)

cv.imshow('Image', bg_)
cv.setMouseCallback('Image', mouseFun)
while True:
    bg = bg_.copy()
    drawABCD(bg)
    cv.putText(bg, txt, CoordTxt, 0, 0.6, (0, 0, 0), 1, cv.LINE_AA)
    cv.imshow('Image', bg)
    k = cv.waitKey(1)
    if k == ord('q'):
        break

    if k == ord('i'):
        X = float(input('X = '))
        Y = float(input('Y = '))
        print(realWordToImageCoord(X, Y))
'''

pause = 10
flagPause = False

# Videos Directory
vid_dir = "Videos/"

# HD
x = {'m': 0, 'M': 500}
y = {'m': 100, 'M': 860}

bg = cv.imread("Images/Doliprane/HD/bg (1).png")
bg = cv.cvtColor(bg, cv.COLOR_BGR2RGB)
plt.imshow(bg)
plt.show()
bg = bg[y['m']:y['M'], x['m']:x['M'], :]

plt.imshow(bg)
plt.show()


# Reading video
cap = cv.VideoCapture(vid_dir + 'cv3.mp4')
print(vid_dir + 'cv3.mp4')
if not cap.isOpened():
    print("Cannot open camera")
    exit()
while True:
    ret, img_ = cap.read()
    if not ret:
        # if frame is read correctly ret is True
        print("Can't receive frame (stream end?). Exiting ...")
        break

    # HD
    x = {'m': 0, 'M': 500}
    y = {'m': 100, 'M': 860}

    # Convert from BGR to RGB
    img_ = cv.cvtColor(img_, cv.COLOR_BGR2RGB)
    # Crop the image, getting the Region of Interest
    img_ = img_[y['m']:y['M'], x['m']:x['M'], :]
    img = img_.copy()
    # Convert from RGB to Gray
    bg_g = cv.cvtColor(bg, cv.COLOR_RGB2GRAY)
    img_g = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
    # Substract the background from the image
    img_bg = cv.addWeighted(img_g, 1, bg_g, -1, 0)
    if(FLAGS['showSubstraction']):
        cv.imshow("Bg substracted from image", img_bg)
    # Binarize the image using a threshold
    mask = cv.inRange(img_bg, minThreshold, maxThreshold)
    if(FLAGS['showThreshold']):
        cv.imshow("Mask", mask)
    # Dilation
    kernal = np.ones((dilationKernalSize, dilationKernalSize), np.uint8)
    dilated_mask = cv.dilate(mask, kernal, 2)
    if(FLAGS['showDilatedMask']):
        cv.imshow("Dilated mask", dilated_mask)
    # Erosion
    kernal = np.ones((erosionKernalSize, erosionKernalSize), np.uint8)
    eroded_mask = cv.erode(dilated_mask, kernal, 1)
    if(FLAGS['showErodedMask']):
        cv.imshow("Eroded mask", eroded_mask)

    # Find contours from eroded mask
    contours, hierarchy = cv.findContours(eroded_mask, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    #print(len(contours))

    # Filter the contours
    contours_new = []
    for contour in contours:
        # Keeping contours that have an adequate area (tune this, resolution matters !)
        #print('area = ' + str(cv.contourArea(contour)))
        if cv.contourArea(contour) < minArea or cv.contourArea(contour) > maxArea:
            continue
        else:
            contours_new.append(contour)

    #print(len(contours_new))

    #img_contours = cv.drawContours(img, contours, -1, (255, 0, 255), 3)

    mask_3d = cv.cvtColor(mask, cv.COLOR_GRAY2RGB)
    img_mask = cv.bitwise_and(img, mask_3d)

    img = img_.copy()
    img_contours_new = cv.drawContours(img, contours_new, -1, (255, 0, 255), 2)

    img = img_.copy()
    #cv.imshow("img", img)

    Angle = []
    C = []
    cord = []
    for contour in contours_new:
        approx = cv.approxPolyDP(
            contour, 0.05*cv.arcLength(contour, True), True)
        # Drawing only contours that are Quadrilateral
        #print(len(approx))
        if len(approx) == 4:
            if(FLAGS['drawContours']):
                cv.drawContours(img_, [approx], 0, (0, 0, 0), 2)
            if(FLAGS['drawCenters']):
                M = cv.moments(contour)
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])
                C.append((cx, cy))
                cord.append(imageToRealWordCoord(cx, cy))
                #print("cx = %.0f, cy = %.0f" % (cx, cy))
                img = cv.circle(img_, (cx, cy), 5, (255, 0, 0), -1)

            #segmentProperties(approx)
            Angle.append(np.round(segmentProperties(approx)))
            #print(approx)
            #x, y, w, h = cv.boundingRect(approx)
            #print('x = %s, y = %s, w = %s, h = %s' % (x, y, w, h))

    countBox = len(Angle)

    if FLAGS['dispCountBoxes']:
        if countBox == 0:
            txt = 'no boxes are detected'
        elif countBox == 1:
            txt = str(countBox) + ' box is detected'
        else:
            txt = str(countBox) + ' boxes are detected'
        CoordTxt = (25, 15)
        cv.putText(img_, txt, CoordTxt, 0, 0.5, (0, 0, 0), 1, cv.LINE_AA)
        #print(C)

    if FLAGS['sendSocket']:
        command = ""
        for i in range(len(C)):
            cx = cord[i][0]/100 #mm ==> 1, cm ==> 10, m = 1000
            cy = cord[i][1]/100
            alpha = Angle[i]
            if(alpha > 0):
                alpha *= -1
            else:
                alpha *= -1
            command += str(cy) + ", " + str(cx) + ", " + str(alpha)
            if i != len(C)-1:
                command += "; "

        print(command)
        sendCommand(sock, UDP_IP, UDP_PORT, command, debug_var=False)

    img_ = cv.cvtColor(img_, cv.COLOR_RGB2BGR)
    cv.imshow('video', img_)

    # Handles Keybord events
    k = cv.waitKey(pause)
    if k == ord('q'):
        break
    elif k == ord('p'):
        pause = flagPause * 10
        flagPause = not flagPause
    elif k == ord(KEYS['dispHelp']):
        FLAGS['dispHelp'] = not FLAGS['dispHelp']
    elif k == ord(KEYS['showSubstraction']):
        FLAGS['showSubstraction'] = not FLAGS['showSubstraction']
        if not FLAGS['showSubstraction']:
            cv.destroyWindow('Bg substracted from image')
    elif k == ord(KEYS['showThreshold']):
        FLAGS['showThreshold'] = not FLAGS['showThreshold']
        if not FLAGS['showThreshold']:
            cv.destroyWindow('Mask')
    elif k == ord(KEYS['showDilatedMask']):
        FLAGS['showDilatedMask'] = not FLAGS['showDilatedMask']
        if not FLAGS['showDilatedMask']:
            cv.destroyWindow('Dilated mask')
    elif k == ord(KEYS['showErodedMask']):
        FLAGS['showErodedMask'] = not FLAGS['showErodedMask']
        if not FLAGS['showErodedMask']:
            cv.destroyWindow('Eroded mask')
    elif k == ord(KEYS['drawContours']):
        FLAGS['drawContours'] = not FLAGS['drawContours']
    elif k == ord(KEYS['drawCenters']):
        FLAGS['drawCenters'] = not FLAGS['drawCenters']
    elif k == ord(KEYS['dispCountBoxes']):
        FLAGS['dispCountBoxes'] = not FLAGS['dispCountBoxes']

# When everything done, release the capture
cap.release()
cv.destroyAllWindows()


