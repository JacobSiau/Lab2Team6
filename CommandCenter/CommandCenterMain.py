from CommandCenter.RobotMover import RobotMover as R
from CommandCenter.MQTTClient import MQTTClient as M
from CommandCenter.SwarmData import SwarmData as S
import time


if __name__ == '__main__':

    # M.connectMQTT(M)
    #
    # # xf, yf = 116.68, 62.64
    #
    # color = input("What color object ")
    # shape = input("What shape ")
    # print("Robot is going to move to the " + str(color) + " " + str(shape))
    #
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
    #     robot = S.positions['blue']['circle']
    #     dest = S.positions[str(color)][str(shape)]
    #     rx, ry = robot[0], robot[1]
    #     ox, oy = dest[0], dest[1]
    #     print("Robot thinks its at " + str(rx) + ", " + str(ry))
    #     print("Robot needs to go to " + str(ox) + ", " + str(oy))
    #     i = input("Type 0 to move robot, anything else to refresh data")
    #     if i is '0':
    #         R.moveToPoint(R, rx, ry, ox, oy, M)
    #         refresh = False

    M.connectMQTT(M)

    testdist = 40 # cm

    # forward testdist, turn 360, backward testdist, turn 360
    R.goForward(R, testdist, M)
    R.turnLeftDegrees(R, 360, M)
    R.turnRightDegrees(R, 360, M)

    #################################################################################
    # # Testing for CV data grabbing at a rate of 1s
    # while True:
    #     time.sleep(1)
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
