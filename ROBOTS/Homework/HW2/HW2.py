
#In another terminal run  roslaunch kobuki_node minimal.launch --screen

import rospy;
from geometry_msgs.msg import Twist;
import time;
from kobuki_msgs.msg import BumperEvent;

global move
global turn
move = 1
turn = 0

def MoveLeg():
        rate=5
        Pub = rospy.Publisher('mobile_base/commands/velocity' , Twist , queue_size=20)

        command = Twist()
        command.linear.x = .2
        command.linear.y = 0
        command.linear.z = 0
        command.angular.x = 0
        command.angular.y = 0
        command.angular.z = 0

        r = rospy.Rate(rate)
        for i in range(rate*2):
                Pub.publish(command)        
                r.sleep()
def MoveBack():
        rate=5
        Pub = rospy.Publisher('mobile_base/commands/velocity' , Twist , queue_size=20)

        command = Twist()
        command.linear.x = -.2
        command.linear.y = 0
        command.linear.z = 0
        command.angular.x = 0
        command.angular.y = 0
        command.angular.z = 0

        r = rospy.Rate(rate)
        for i in range(rate*2):
                Pub.publish(command)        
                r.sleep()
def Stop():
        rate=5
        Pub = rospy.Publisher('mobile_base/commands/velocity' , Twist , queue_size=20)

        command = Twist()
        command.linear.x = 0
        command.linear.y = 0
        command.linear.z = 0
        command.angular.x = 0
        command.angular.y = 0
        command.angular.z = 0

        r = rospy.Rate(rate)



def Turn():
        rate=10
        Pub = rospy.Publisher('mobile_base/commands/velocity' , Twist , queue_size=20)
        command = Twist()
        command.linear.x = 0
        command.linear.y = 0
        command.linear.z = 0
        command.angular.x = 0
        command.angular.y = 0
        command.angular.z = 1
        r = rospy.Rate(rate)
        for i in range(rate*2):
                Pub.publish(command)        
                r.sleep()
def Turn(dir,time):
        rate=time
        Pub = rospy.Publisher('mobile_base/commands/velocity' , Twist , queue_size=20)
        command = Twist()
        command.linear.x = 0
        command.linear.y = 0
        command.linear.z = 0
        command.angular.x = 0
        command.angular.y = 0
        command.angular.z = dir
        r = rospy.Rate(rate)
        for i in range(rate*2):
                Pub.publish(command)        
                r.sleep()

def doSquare():
        for i in range(4):
            MoveLeg()
            Turn(1)
                
def bumperCallback(data):
	if(data.state == BumperEvent.CENTER):
		global move
		global turn
		move = 0
		Stop()
		MoveBack()
		time.sleep(3)
		if turn == 0:
			Turn(1,10)
			turn = 90
			time.sleep(3)
			moveLeg()
			time.sleep(3)
			Turn(-1, 10)
			turn = 0
		time.sleep(3)
		move = 1

rospy.init_node('Driver')
rospy.Subscriber('/mobile_base/events/bumper',BumperEvent,bumperCallback)


while (True):
	if move == 1:
		print("going forward)")
		MoveLeg()
		time.sleep(3)





