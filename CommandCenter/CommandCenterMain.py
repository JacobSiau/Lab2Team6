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

    # our robot will be the blue circle
    # we want it to move to the given shape
    #
    # grabbing the data

    # s.getData(s)
    # xf, yf = 116.68, 62.64 # middle of field
    # robot = s.positions['blue']['circle']
    # rx, ry = robot[0], robot[1]
    # print(str(rx) + " " + str(ry))
    #
    # r.moveToPoint(r, rx, ry, xf, yf, m)

    while True:
        time.sleep(1)
        s.getData(s)
        print("-------------------")
        print("\n")
        s.displayBall(s)
        print("\n")
        s.displayBlue(s)
        print("\n")
        s.displayRed(s)
