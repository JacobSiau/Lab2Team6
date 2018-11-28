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
    # S.checkIfBallMoved(S)
    S.robot = S.positions['red']['triangle']
    S.ball = S.positions['ball']
    # print("R: " + str(S.robot[0]) + ", " + str(S.robot[1]))
    # print("S: " + str(S.ball[0]) + ", " + str(S.ball[1]))


if __name__ == '__main__':

    M.connectMQTT(M)
    getDataTimer = T(0.5, getDataJob) # schedules getDataJob

    last_turn = None
    x_dist = 10000
    y_dist = 10000
    x_tolerance = 15
    y_tolerance = 15

    # while True:
    #     print("x")
    #     sleep(10)

    done = False
    while done is False:
        getDataJob()
        print("Robot is at " + str(S.robot[0]) + ", " + str(S.robot[1]))
        print("Ball is at " + str(S.ball[0]) + ", " + str(S.ball[1]))
        i = input("Type 0 to go, anything else to refresh")
        if i is '0':

            R.goForward(R, robot_num=1, dx=20, m=M)
            # sleep(2)
            # R.turnRobot(R, robot_num=1, direction='R', m=M)
            # sleep(2)
            # R.turnRobot(R, robot_num=1, direction='R', m=M)
            # sleep(2)
            # R.goForward(R, robot_num=1, dx=20, m=M)
            sleep(2)
            M.publish(M, topic="esp32/r1", msg="B");
            sleep(2)
            R.stopRobot(R, robot_num=1, m=M)

            sleep(5)

            x_dist = round(abs(S.ball[0] - S.robot[0]), 2)
            y_dist = round(abs(S.ball[1] - S.robot[1]), 2)
            print("xdist: " + str(x_dist))
            print("ydist: " + str(y_dist))

            # initially facing in x direction

            # first turn

            # if we need to turn left/right initially
            if abs(S.ball[1] - S.robot[1]) > y_tolerance:
                if S.robot[1] > S.ball[1]:
                    last_turn = 'L'
                    R.turnRobot(R, robot_num=2, direction='L', m=M)
                # else we turn right
                else:
                    last_turn = 'R'
                    R.turnRobot(R, robot_num=2, direction='R', m=M)
                sleep(5)

                # moving the y dist

                R.goForward(R, robot_num=2, dx=y_dist - y_tolerance, m=M)
                # R.goForwardSlowly(R, robot_num=2, m=M)
                # while abs(S.ball[1] - S.robot[1]) > y_tolerance:
                #     pass
                # R.stopRobot(R, robot_num=2, m=M)

                sleep(5)

            # second turn

            # if we turned L, now R
            if last_turn is not None:
                if last_turn is 'L':
                    R.turnRobot(R, robot_num=2, direction='R', m=M)
                # else we turn L now
                else:
                    R.turnRobot(R, robot_num=2, direction='L', m=M)
            sleep(5)

            # moving the x dist

            R.goForward(R, robot_num=2, dx=x_dist - x_tolerance, m=M)
            # R.goForwardSlowly(R, robot_num=2, m=M)
            # while abs(S.ball[0] - S.robot[0]) > x_tolerance:
            #     pass
            # R.stopRobot(R, robot_num=2, m=M)
            sleep(5)

            # should be done here

            if abs(S.ball[1] - S.robot[1]) <= 8.0:
                M.publish(M, topic="esp32/s", msg="r2=ball?")
            else:
                M.publish(M, topic="esp32/s", msg="r2!=ball")

            done = True





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
