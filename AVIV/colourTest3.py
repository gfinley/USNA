import numpy as np
import cv2
from matplotlib import pyplot as plt
MIN_MATCH_COUNT = 10

img1 = cv2.imread('b0.jpg',0)          # queryImage
img2 = cv2.imread('b1.jpg',0) # trainImage

# Initiate SIFT detector
orb = cv2.ORB()

# find the keypoints and descriptors with SIFT
kp1, des1 = orb.detectAndCompute(img1,None)
kp2, des2 = orb.detectAndCompute(img2,None)

# create BFMatcher object
bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

# Match descriptors.
matches = bf.match(des1,des2)


# Sort them in the order of their distance.
matches = sorted(matches, key = lambda x:x.distance)
img3 = cv2.drawKeypoints(img1,kp1,color=(0,255,0), flags=0)
img4 = cv2.drawKeypoints(img2,kp2,color=(0,255,0), flags=0)


cv2.imshow('image',img3)
cv2.imshow('image2',img4)
cv2.waitKey(0)


cv2.destroyAllWindows()

# store all the good matches as per Lowe's ratio test.
good = []

for m in matches:
    if m.distance < 0.7:
        good.append(m)

if len(good)>MIN_MATCH_COUNT:
    print "this is a valid match"
else:
    print "Not enough matches are found - %d/%d" % (len(good),MIN_MATCH_COUNT)
    matchesMask = None

    
# Draw first 10 matches.
#img3 = draw_matches(img1,kp1,img2,kp2,matches[:10])
