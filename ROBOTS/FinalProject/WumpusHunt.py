#this is for the wompus hunt
#functions are goint to include
#http://wiki.ros.org/navigation/Tutorials/SendingSimpleGoals
#send goal to base
#http://answers.ros.org/question/45556/simple-movement-goal-problem/
#move to base function

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
    
    
#the program will know the size of the room so we can build a internal model of the maze.
#each maze room will have a struct