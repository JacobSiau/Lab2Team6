from CommandCenter.RobotMover import RobotMover as r
from CommandCenter.MQTTClient import MQTTClient as m
from CommandCenter.SwarmData import SwarmData as s
import time


# def checkIntegrity(s: S, srcobj, destobj):
#     if S.oldpositions is None:
#         return False
#     si = S.oldpositions[srcobj]
#     oi = S.oldpositions[destobj]
#     s1 = S.positions[srcobj]
#     o1 = S.positions[destobj]
#     if s0[0]


if __name__ == '__main__':

    m.connectMQTT(m)

    # xf, yf = 116.68, 62.64

    color = input("What color object ")
    shape = input("What shape ")
    print("Robot is going to move to the " + str(color) + " " + str(shape))

    refresh = True
    while refresh is True:
        s.getData(s)
        print("-------------------")
        print("\n")
        s.displayBall(s)
        print("\n")
        s.displayBlue(s)
        print("\n")
        s.displayRed(s)
        print("-------------------")
        robot = s.positions['blue']['circle']
        object = s.positions[str(color)][str(shape)]
        rx, ry = robot[0], robot[1]
        ox, oy = object[0], object[1]
        print("Robot thinks its at " + str(rx) + ", " + str(ry))
        print("Robot needs to go to " + str(ox) + ", " + str(oy))
        i = input("Type 0 to move robot, anything else to refresh data")
        if i is '0':
            r.moveToPoint(r, rx, ry, ox, oy, m)
            refresh = False


    # # grabbing the data
    # s.getData(s)
    # # it will move to the center of the field
    # xf, yf = 116.68, 62.64
    # # our robot will be the blue circle
    # robot = s.positions['blue']['circle']
    # rx, ry = robot[0], robot[1]
    # print("Robot thinks its at " + str(rx) + ", " + str(ry))
    # print("Robot needs to go to " + str(xf) + ", " + str(yf))
    #
    # r.moveToPoint(r, rx, ry, xf, yf, m)

    # while True:
    #     time.sleep(1)
    #     s.getData(s)
    #     print("-------------------")
    #     print("\n")
    #     s.displayBall(s)
    #     print("\n")
    #     s.displayBlue(s)
    #     print("\n")
    #     s.displayRed(s)
    #     print("-------------------")
