#this is for the wompus hunt
#functions are goint to include
#http://wiki.ros.org/navigation/Tutorials/SendingSimpleGoals
#send goal to base
#http://answers.ros.org/question/45556/simple-movement-goal-problem/
#move to base function
import rospy
import numpy as np
import cv2, cv_bridge
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image

global currImage

#test for a commit

class Follower:
    
    def __init__(self):
        self.bridge = cv_bridge.CvBridge()
        cv2.namedWindow("window",1)
        self.image_sub = rospy.Subscriber("/camera/rgb/image_raw",Image, self.image_callback)

    def image_callback(self,msg):
        self.image = self.bridge.imgmsg_to_cv2(msg,desired_encoding="bgr8")
        cv2.imshow("window", image)
        cv2.waitKey(3)
        
        
    def get_Image(self):
        return self.image
                
#inital set up of camera
rospy.init_node("follower")
follower = Follower()

#cap = cv2.VideoCapture(CV_CAP_OPENNI_ASUS)
#params = cv2.SimpleBlobDetector_Params()
#params.minThreshold = 10;
#params.maxThreshold = 200;

#set up color recognition for green
detector = cv2.SimpleBlobDetector_create() 
greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)


#cv2.waitKey(0)

#image_sub = rospy.Subscriber("/camera/rgb/image_raw",Image,self.callback)



def goToRoom(room):
    rospy.init_node('GotoRoom')
    #Simple Action Client
    sac = actionlib.SimpleActionClient('move_base', MoveBaseAction )
    #create goal
    goal = MoveBaseGoal()
    #use self?
    #set goal
    goal.target_pose.pose.position.x =room.x
    goal.target_pose.pose.position.y =room.y
    goal.target_pose.pose.orientation.w = 1.0
    goal.target_pose.header.frame_id = 'first_move'
    goal.target_pose.header.stamp = rospy.Time.now()
    #start listner
    sac.wait_for_server()
    #send goal
    sac.send_goal(goal)
    #finish
    sac.wait_for_result()
    #print result
    print sac.get_result()

def getColorInRoom():
    #this function will identify the color seen in the room if any
    #probably utilize opencv
    print("hello")
    
def pickNextRoom():
     print("nothing")
        
while(True):
    
    #image = follower.get_Image()
    #grab the image input
    #ret, frame = cap.read()
    
    #blurr the frame make blog detection easier
    #blurred = cv2.GaussianBlur(image, (11, 11), 0)
    
    #convert the frame to hsv
    #hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    
    #do the green mask and print to screen
    #mask = cv2.inRange(blurred, greenLower, greenUpper)
    #mask = cv2.erode(mask, None, iterations=2)
    #mask = cv2.dilate(mask, None, iterations=2)
    #skip the keypoint and contour recognition and print the image to see if it is working
    
    cv2.imshow("window", follower.get_Image())
    print("hello\n")
    
#the program will know the size of the room so we can build a internal model of the maze.
#each maze room will have a struct