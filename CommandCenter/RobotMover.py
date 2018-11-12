from CommandCenter.MQTTClient import MQTTClient
import time
from math import ceil, pi


class RobotMover:

    # robot wheel radius
    r = 3 # cm

    #################################################################################
    # moveToPoint moves the rover from a point xo,yo to a point xf, yf
    # it does this by moving the rover straight, turning the rover left or right,
    # and then moving the rover straight again
    @staticmethod
    def moveToPoint(__self__, xo, yo, xf, yf, m: MQTTClient):
        # how far it needs to go
        dx = round(xf - xo, 2)
        print("Robot needs to change x by: " + str(dx))
        dy = round(yf - yo, 2)
        print("Robot needs to change y by: " + str(dy))
        # turn right/left
        if dy < 0:
            __self__.turnRight(__self__, m)
            time.sleep(2)
            __self__.goForward(__self__, abs(dy), m)
            time.sleep(2)
            __self__.turnLeft(__self__, m)
            time.sleep(2)
            __self__.goForward(__self__, dx, m)
        else:
            __self__.turnLeft(__self__, m)
            time.sleep(4)
            __self__.goForward(__self__, abs(dy), m)
            time.sleep(4)
            __self__.turnRight(__self__, m)
            time.sleep(4)
            __self__.goForward(__self__, dx, m)
            time.sleep(4)
    #################################################################################

    #################################################################################
    # goForward moves the rover forward a distance dx
    # it does this by publishing the correct motor direction and time
    @staticmethod
    def goForward(__self__, dx, m: MQTTClient):
        # moves the robot forward a distance dx
        print("goForward called for: " + str(dx) + " cm")
        encount = str(round((40 * (dx - 0.3*dx)) / (2*pi*__self__.r)))
        print("Encount: " + encount)
        if len(encount) is 1:
            encount = '000' + encount
        elif len(encount) is 2:
            encount = '00' + encount
        if len(encount) is 3:
            encount = '0' + encount
        elif len(encount) > 4:
            print("Faulty encount!")
        print("Final encount: " + encount)
        m.publish(m, "esp32/testdir", "F" + encount)
    #################################################################################

    #################################################################################
    @staticmethod
    def goBackward(__self__, dx, m: MQTTClient):
        print("goBackward called for: " + str(dx) + " cm")
        encount = str(round((40 * dx) / (2 * pi * __self__.r)))
        print("Encount: " + encount)
        if len(encount) is 1:
            encount = '000' + encount
        elif len(encount) is 2:
            encount = '00' + encount
        if len(encount) is 3:
            encount = '0' + encount
        elif len(encount) > 4:
            print("Faulty encount!")
        print("Final encount: " + encount)
        m.publish(m, "esp32/testdir", "B" + encount)
    #################################################################################

    #################################################################################
    # turnLeft turns the rover left by 90 degrees (roughly)
    @staticmethod
    def turnLeft(__self__, m: MQTTClient):
        # turns the robot left 90 degrees
        print("Turning 90 Left!")
        m.publish(m, "esp32/testdir", "K0029")
    #################################################################################

    #################################################################################
    # turnRight turns the rover right by 90 degrees (roughly)
    @staticmethod
    def turnRight(__self__, m: MQTTClient):
        # turns the robot right 90 degrees
        print("Turning 90 Right!")
        m.publish(m, "esp32/testdir", "Q0031")
    #################################################################################

    #################################################################################
    def turnLeftDegrees(self, deg, m: MQTTClient):
        encount = str(round((deg / 360) * 138.4))
        print("Turning left " + str(deg) + " degrees, encount: " + str(encount))
        m.publish(m, "esp32/testdir", "L" + encount)
    #################################################################################

    #################################################################################
    def turnRightDegrees(self, deg, m: MQTTClient):
        encount = str(round((deg / 360) * 138.4))
        print("Turning right " + str(deg) + " degrees, encount: " + str(encount))
        m.publish(m, "esp32/testdir", "R" + encount)
    #################################################################################

    #################################################################################
    # stop stops the rover
    # it does this by sending the stop character to the ESP32
    @staticmethod
    def stop(__self__, m: MQTTClient):
        # stops the robot
        print("Stopping!")
        m.publish(m, "esp32/testdir", "S000")
    #################################################################################
