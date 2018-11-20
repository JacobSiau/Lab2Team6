from CommandCenter.RobotMover import RobotMover as R
from CommandCenter.MQTTClient import MQTTClient as M
from CommandCenter.SwarmData import SwarmData as S
import time
from scheduler import Scheduler
sched = Scheduler()
sched.daemon = False

robot = ()
ball = ()


def updateRobotAndBall():
    S.getData(S)
    robot = S.positions['red']['circle']
    ball = S.positions['ball']


if __name__ == '__main__':

    M.connectMQTT(M)
    sched.schedule_task(1, updateRobotAndBall)
    sched.start()
    last_turn = None

    time.sleep(1.5)
    done = False
    while done is False:
        # robot = S.positions['red']['circle']
        # ball = S.positions['ball']
        print("Robot is at " + str(robot[0]) + ", " + str(robot[1]))
        print("Ball is at " + str(ball[0]) + ", " + str(ball[1]))
        i = input("Type 0 to go, anything else to refresh")
        if i is '0':
            if robot[1] < ball[1]:
                last_turn = 'L'
                R.turnRobot(R, robot_num=1, direction='L', m=M)
            else:
                last_turn = 'R'
                R.turnRobot(R, robot_num=1, direction='R', m=M)
            while abs(robot[1] - ball[1]) > 10:
                R.goForwardSlowly(R, robot_num=1, m=M)
            R.stopRobot(R, robot_num=1, m=M)
            if last_turn == 'L':
                R.turnRobot(R, robot_num=1, direction='R', m=M)
            else:
                R.turnRobot(R, robot_num=1, direction='L', m=M)
            while abs(robot[0] - ball[0]) > 15:
                R.goForwardSlowly(R, robot_num=1, m=M)
            R.stopRobot(R, robot_num=1, m=M)

    # refresh = True
    # while refresh is True:
    #     S.getData(S)
    #     print("-------------------")
    #     print("\n")
    #     S.displayBall(S)
    #     print("\n")
    #     S.displayBlue(S)
    #     print("\n")
    #     S.displayRed(S)
    #     print("-------------------")
    #     robot = S.positions['red']['circle']
    #     ball = S.positions['ball']
    #     goal = (244, 61)
    #     print("Robot thinks its at " + str(robot[0]) + ", " + str(robot[1]))
    #     print("Ball thinks its at " + str(ball[0]) + ", " + str(ball[1]))
    #     print("Goal is at " + str(goal[0]) + ", " + str(goal[1]))
    #     i = input("Type 0 to move robot, anything else to refresh data")
    #     if i is '0':
    #         R.moveToPoint(R, robot[0], robot[1], ball[0], ball[1], M)
    #         refresh = False




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
