from CommandCenter.RobotMover import RobotMover as R
from CommandCenter.MQTTClient import MQTTClient as M
from CommandCenter.SwarmData import SwarmData as S
import time

if __name__ == '__main__':

    M.connectMQTT(M)
    last_turn = None
    xdist = 10000
    ydist = 10000
    x_separation = 10000
    y_separation = 10000

    done = False
    while done is False:
        S.getData(S)
        robot = S.positions['red']['circle']
        ball = S.positions['ball']
        print("Robot is at " + str(robot[0]) + ", " + str(robot[1]))
        print("Ball is at " + str(ball[0]) + ", " + str(ball[1]))
        i = input("Type 0 to go, anything else to refresh")
        if i is '0':
            # R.moveToPoint(R, robot[0], robot[1], ball[0], ball[1])

            ydist = abs(ball[1] - robot[1])
            xdist = abs(ball[0] - robot[0])

            # initially facing in x direction
            # if we need to turn left
            if robot[1] < ball[1]:
                last_turn = 'L'
                R.turnRobot(R, robot_num=2, direction='L', m=M)
            # else we turn right
            else:
                last_turn = 'R'
                R.turnRobot(R, robot_num=2, direction='R', m=M)
            time.sleep(4)
            # moving the y dist
            # go 3/4 the distance at normal speed
            R.goForwardNormally(R, robot_num=2, m=M)
            while y_separation > ydist * 0.75:
                # update current separation
                S.getData(S)
                robot = S.positions['red']['circle']
                ball = S.positions['ball']
                y_separation = abs(ball[1] - robot[1])
                time.sleep(0.1)
            R.stopRobot(R, robot_num=2, m=M)
            time.sleep(4)
            # at this point we've gone 3/4, now go the remaining 1/4 slowly
            R.goForwardSlowly(R, robot_num=2, m=M)
            while y_separation > ydist * 0.25:
                # update current separation
                S.getData(S)
                robot = S.positions['red']['circle']
                ball = S.positions['ball']
                y_separation = abs(ball[1] - robot[1])
                time.sleep(0.1)
            R.stopRobot(R, robot_num=2, m=M)
            time.sleep(4)
            # now turn to face x again
            # if we turned L, now R
            if last_turn is 'L':
                R.turnRobot(R, robot_num=2, direction='R', m=M)
            # else we turn L now
            else:
                R.turnRobot(R, robot_num=2, direction='L', m=M)
            time.sleep(4)
            # now go the x dist
            # go 3/4 the distance at normal speed
            R.goForwardNormally(R, robot_num=2, m=M)
            while x_separation > xdist * 0.75:
                # update current separation
                S.getData(S)
                robot = S.positions['red']['circle']
                ball = S.positions['ball']
                x_separation = abs(ball[0] - robot[0])
                time.sleep(0.1)
            R.stopRobot(R, robot_num=2, m=M)
            time.sleep(4)
            # at this point we've gone 3/4, now go the remaining 1/4 slowly
            R.goForwardSlowly(R, robot_num=2, m=M)
            while x_separation > xdist * 0.25:
                # update current separation
                S.getData(S)
                robot = S.positions['red']['circle']
                ball = S.positions['ball']
                y_separation = abs(ball[0] - robot[0])
                time.sleep(0.1)
            R.stopRobot(R, robot_num=2, m=M)
            time.sleep(4)
            M.publish(M, topic="esp32/s", msg="r2=ball?")
            done = True


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
