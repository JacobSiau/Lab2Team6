from CommandCenter.RobotMover import RobotMover as R
from CommandCenter.MQTTClient import MQTTClient as M
from CommandCenter.SwarmData import SwarmData as S
import time


if __name__ == '__main__':

    M.connectMQTT(M)

    print("Time for the robot to score a goal")

    refresh = True
    while refresh is True:
        S.getData(S)
        print("-------------------")
        print("\n")
        S.displayBall(S)
        print("\n")
        S.displayBlue(S)
        print("\n")
        S.displayRed(S)
        print("-------------------")
        robot = S.positions['blue']['circle']
        ball = S.positions['ball']
        goal = (244, 61)
        print("Robot thinks its at " + str(robot[0]) + ", " + str(robot[1]))
        print("Ball thinks its at " + str(ball[0]) + ", " + str(ball[1]))
        print("Robot needs to go to " + str(goal[0]) + ", " + str(goal[1]))
        i = input("Type 0 to move robot, anything else to refresh data")
        if i is '0':
            R.moveToPoint(R, robot[0], robot[1], ball[0], ball[1], M)
            refresh = False



    #################################################################################
    # Testing for CV data grabbing at a rate of 1s
    # while True:
    #     time.sleep(0.2)
    #     S.getData(S)
    #     print("-------------------")
    #     print("\n")
    #     S.displayBall(S)
    #     print("\n")
    #     S.displayBlue(S)
    #     print("\n")
    #     S.displayRed(S)
    #     print("-------------------")
    #################################################################################
