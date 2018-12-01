from CommandCenter.RobotMover import RobotMover as R
from CommandCenter.MQTTClient import MQTTClient as M
from CommandCenter.SwarmData import SwarmData as S
from CommandCenter.RepeatedTimer import RepeatedTimer as T
from time import sleep
from math import sqrt


# getDataJob is implemented by schedule to run at a certain time interval
# it updates the CV data in the SwarmData object
def getDataJob():
    # print("Getting new CV data...")
    S.getData(S)
    S.robot2 = S.positions['blue']['circle']
    S.robot1 = S.positions['blue']['square']
    S.ball = S.positions['ball']
    # print("R: " + str(S.robot[0]) + ", " + str(S.robot[1]))
    # print("S: " + str(S.ball[0]) + ", " + str(S.ball[1]))


if __name__ == '__main__':

    M.connectMQTT(M)
    M.subscribe(M, topic="esp32/s")
    getDataTimer = T(0.5, getDataJob) # schedules getDataJob

    last_turn = None
    next_turn = None
    x_tolerance = 20
    y_tolerance = 12

    done = False
    while done is False:
        getDataJob()
        print("Robot1 is at " + str(S.robot1[0]) + ", " + str(S.robot1[1]))
        print("Robot2 is at " + str(S.robot2[0]) + ", " + str(S.robot2[1]))
        print("Ball is at " + str(S.ball[0]) + ", " + str(S.ball[1]))
        i = input("Type 0 to go, anything else to refresh")
        if i is '0':

            # r1_x_dist = round(abs(S.robot1[0] - S.ball[0]), 2)
            # r1_y_dist = round(abs(S.robot1[1] - S.ball[1]), 2)
            # r1_goal_dist = round(abs(S.robot1[0])) + x_tolerance
            #
            # print("r1 xdist: " + str(r1_x_dist))
            # print("r1 ydist: " + str(r1_y_dist))
            #
            #
            # ############################################################
            # # R1 initially in +y direction, R1 initially in -y direction
            # # R1 goes to the ball and scores, then R2 touches ball
            #
            # # R1 go forward y dist
            # R.goForward(R, robot_num=1, dx=r1_y_dist, m=M)
            # sleep(5)
            #
            # # R1 turn towards ball
            # R.turnRobot(R, robot_num=1, direction='L', m=M)
            # sleep(5)
            #
            # # R1 goes all the way to the goal?
            # R.goForward(R, robot_num=1, dx=r1_goal_dist, m=M)
            # sleep(10)
            #
            # # R1 backs up a little
            # R.goBackward(R, robot_num=1, dx=50, m=M)
            # sleep(5)
            #
            # # R1 turns left
            # R.turnRobot(R, robot_num=1, direction='L', m=M)
            # sleep(5)
            #
            # # R1 goes to the edge of the field
            # R.goForward(R, robot_num=1, dx=100, m=M)
            # sleep(5)
            #
            # r2_x_dist = round(abs(S.robot2[0] - S.ball[0]), 2)
            # r2_y_dist = round(abs(S.robot2[1] - S.ball[1]), 2)
            # r2_goal_dist = round(abs(S.robot2[0]))
            # print("r2 xdist: " + str(r2_x_dist))
            # print("r2 ydist: " + str(r2_y_dist))
            #
            # # R2 goes forward y dist
            # R.goForward(R, robot_num=2, dx=r2_y_dist, m=M)
            # sleep(5)
            #
            # # R2 turns toward ball
            # R.turnRobot(R, robot_num=2, direction='R', m=M)
            # sleep(5)
            #
            # # R2 takes it all the way
            # R.goForward(R, robot_num=2, dx=r2_goal_dist, m=M)
            # sleep(10)

            ##############################################################
            ##############################################################
            ##############################################################
            ##############################################################
            ##############################################################
            ##############################################################
            ##############################################################


            r1_x_dist = round(abs(S.robot1[0] - S.ball[0]), 2)
            r1_y_dist = round(abs(S.robot1[1] - S.ball[1]), 2)
            #r1_goal_dist = round(abs(S.robot1[0])) + x_tolerance

            print("r1 xdist: " + str(r1_x_dist))
            print("r1 ydist: " + str(r1_y_dist))

            ############################################################
            # R1 initially in +y direction, R1 initially in -y direction
            # R1 goes and touches the ball, R2 scores

            # R1 go forward y dist
            R.goForward(R, robot_num=1, dx=r1_y_dist - y_tolerance, m=M)
            sleep(5)

            # R1 turn towards ball
            R.turnRobot(R, robot_num=1, direction='L', m=M)
            sleep(5)

            # R1 goes and taps the ball
            R.goForward(R, robot_num=1, dx=r1_x_dist - x_tolerance, m=M)
            sleep(5)

            # R1 backs up
            R.goBackward(R, robot_num=1, dx=r1_x_dist - x_tolerance, m=M)
            sleep(10)

            if round(S.ball[1]) < 61:
                R.turnRobot(R, robot_num=1, direction='R', m=M)
            else:
                R.turnRobot(R, robot_num=1, direction='L', m=M)
            sleep(5)

            # R1 goes to the edge of the field
            R.goForward(R, robot_num=1, dx=50, m=M)
            sleep(5)

            r2_x_dist = round(abs(S.robot2[0] - S.ball[0]), 2)
            r2_y_dist = round(abs(S.robot2[1] - S.ball[1]), 2)
            r2_goal_dist = round(abs(S.robot2[0]))
            print("r2 xdist: " + str(r2_x_dist))
            print("r2 ydist: " + str(r2_y_dist))

            # R2 goes forward y dist
            R.goForward(R, robot_num=2, dx=r2_y_dist - y_tolerance, m=M)
            sleep(5)

            # R2 turns toward ball
            R.turnRobot(R, robot_num=2, direction='R', m=M)
            sleep(5)

            # R2 takes it all the way
            R.goForward(R, robot_num=2, dx=r2_goal_dist, m=M)
            sleep(10)