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
    S.robot1 = S.positions['blue']['triangle']
    S.ball = S.positions['ball']
    # print("R: " + str(S.robot[0]) + ", " + str(S.robot[1]))
    # print("S: " + str(S.ball[0]) + ", " + str(S.ball[1]))


if __name__ == '__main__':

    M.connectMQTT(M)
    M.subscribe(M, topic="esp32/s")
    getDataTimer = T(0.5, getDataJob) # schedules getDataJob

    last_turn = None
    x_tolerance = 15
    y_tolerance = 15

    done = False
    while done is False:
        getDataJob()
        print("Robot1 is at " + str(S.robot1[0]) + ", " + str(S.robot1[1]))
        print("Robot2 is at " + str(S.robot2[0]) + ", " + str(S.robot2[1]))
        print("Ball is at " + str(S.ball[0]) + ", " + str(S.ball[1]))
        i = input("Type 0 to go, anything else to refresh")
        if i is '0':

            r1_x_dist = round(abs(S.ball[0] - S.robot1[0]), 2)
            r1_y_dist = round(abs(S.ball[1] - S.robot1[1]), 2)
            print("r1 xdist: " + str(r1_x_dist))
            print("r1 ydist: " + str(r1_y_dist))

            # first turn r1

            # if we need to turn left/right initially
            if abs(S.ball[1] - S.robot1[1]) > y_tolerance:
                if S.robot1[1] > S.ball[1]:
                    last_turn = 'L'
                    R.turnRobot(R, robot_num=1, direction='L', m=M)
                # else we turn right
                else:
                    last_turn = 'R'
                    R.turnRobot(R, robot_num=1, direction='R', m=M)
                sleep(5)
                # moving the y dist
                R.goForward(R, robot_num=1, dx=r1_y_dist - y_tolerance, m=M)
                sleep(5)

            # if we turned L, now R
            if last_turn is not None:
                if last_turn is 'L':
                    R.turnRobot(R, robot_num=1, direction='R', m=M)
                # else we turn L now
                else:
                    R.turnRobot(R, robot_num=1, direction='L', m=M)
            sleep(5)

            R.goForward(R, robot_num=1, dx=r1_x_dist - x_tolerance, m=M)
            sleep(5)

            # # r1 go to ball then turn out of way
            #
            # R.goForward(R, robot_num=1, dx=40, m=M)
            # sleep(5)
            # R.turnRobot(R, robot_num=1, direction='R', m=M)
            # sleep(2)
            # R.turnRobot(R, robot_num=1, direction='R', m=M)
            # sleep(2)
            # R.turnRobot(R, robot_num=1, direction='R', m=M)
            # sleep(2)
            # R.goForward(R, robot_num=1, dx=40, m=M)
            # # sleep(2)
            # # M.publish(M, topic="esp32/r1", msg="B")
            # sleep(3)
            # R.stopRobot(R, robot_num=1, m=M)
            #
            # sleep(5)

            # R2 MOTION

            # r2_x_dist = round(abs(S.ball[0] - S.robot2[0]), 2)
            # r2_y_dist = round(abs(S.ball[1] - S.robot2[1]), 2)
            # print("r2 xdist: " + str(r2_x_dist))
            # print("r2 ydist: " + str(r2_y_dist))
            #
            # # initially facing in x direction
            #
            # # first turn
            #
            # # if we need to turn left/right initially
            # if abs(S.ball[1] - S.robot2[1]) > y_tolerance:
            #     if S.robot2[1] > S.ball[1]:
            #         last_turn = 'L'
            #         R.turnRobot(R, robot_num=2, direction='L', m=M)
            #     # else we turn right
            #     else:
            #         last_turn = 'R'
            #         R.turnRobot(R, robot_num=2, direction='R', m=M)
            #     sleep(5)
            #     # moving the y dist
            #     R.goForward(R, robot_num=2, dx=r2_y_dist - y_tolerance, m=M)
            #     sleep(5)
            #
            # # second turn
            #
            # # if we turned L, now R
            # if last_turn is not None:
            #     if last_turn is 'L':
            #         R.turnRobot(R, robot_num=2, direction='R', m=M)
            #     # else we turn L now
            #     else:
            #         R.turnRobot(R, robot_num=2, direction='L', m=M)
            # sleep(5)
            #
            # M.publish(M, topic="esp32/fb2", msg="go")
            # sleep(5)
            # foundBall = False
            # if M.last_status is not None:
            #     foundBall = (M.last_status == "2bT")
            #
            # print("foundBall: " + str(foundBall))
            # done = True

