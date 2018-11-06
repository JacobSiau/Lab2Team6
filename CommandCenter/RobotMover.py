from CommandCenter.MQTTClient import MQTTClient
import time
from math import ceil, pi


class RobotMover:

    # experimentally found forward speed
    vlinear = 50.45 # cm/s
    # experimentally found time to turn 90 degrees
    tquarter = 0.26 # s
    # robot wheel radius
    r = 3.25 # cm

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
        # go straight in the x-direction
        __self__.goForward(__self__, dx, m)
        # turn right/left
        if dy < 0:
            __self__.turnRight(__self__, m)
        else:
            __self__.turnLeft(__self__, m)
        # go straight in the y-direction
        __self__.goForward(__self__, abs(dy), m)
    #################################################################################

    #################################################################################
    # goForward moves the rover forward a distance dx
    # it does this by publishing the correct motor direction and time
    @staticmethod
    def goForward(__self__, dx, m: MQTTClient):

        # # moves the robot forward a distance dx
        # # moves it at a speed of 50.45 cm/s
        #
        # # get the time to move the robot
        # dtnum = round(dx / __self__.vlinear, 2)
        # print("goForward called for: " + str(dx) + " cm")
        # print("Robot needs to move for " + str(dtnum) + "s to go " + str(dx) + " cm")
        # # check if within range
        # if dtnum < 0.0 or dtnum > 9.99:
        #     print("ERROR: Forward time > 9s!")
        #     return
        # # dt will be a string in the form <#>.<#> or <#>.<#><#> etc..
        # dt = str(float(dtnum))
        # print("Double checking time to send: " + dt)
        # # from here we know the str is 0.0 to 9.9 (numerically),
        # # now we concatenate its digits without the decimal
        # # to send to the robot
        # timestr = ""
        # for i in range(0, len(dt)):
        #     if i is not 1:
        #         timestr += dt[i]
        # if len(timestr) is 3:
        #     timestr += '0'
        # if len(timestr) is 2:
        #     timestr += '00'
        # print("Time string being sent: " + timestr)
        # # ready to send to robot
        # m.publish(m, "esp32/dir", "F" + timestr)
        # m.publish(m, "esp32/status", "forward")
        # time.sleep(ceil(dtnum))

        encount = str(round((40 * dx) / (2*pi*__self__.r)))
        print("Encoder count to send: " + encount)
        if len(encount) is 1:
            encount = '00' + encount
        elif len(encount) is 2:
            encount = '0' + encount
        if len(encount) > 3:
            print("Encoder count greater than 2 decimals!")
        print("Final encoder count: " + encount)
        m.publish(m, "esp32/testdir", "F" + encount)
        time.sleep(2)
    #################################################################################

    #################################################################################
    # turnLeft turns the rover left by 90 degrees (roughly)
    # it does this by sending a hardcoded turn time to the left
    @staticmethod
    def turnLeft(__self__, m: MQTTClient):
        # turns the robot left 90 degrees
        # will turn the robot for 0.26 seconds
        m.publish(m, "esp32/dir", "L0250")
        print("Turning Left!")
        # m.publish(m, "esp32/status", "left90")
        time.sleep(1)
    #################################################################################

    def turnLeftDegrees(self, deg, m: MQTTClient):
        encount = str(round((deg / 360) * 138.4))
        m.publish(m, "esp32/testdir", "K" + encount)
        print("Turning left " + str(deg) + " degrees, encount: " + str(encount))
        time.sleep(2)

    def turnRightDegrees(self, deg, m: MQTTClient):
        encount = str(round((deg / 360) * 138.4))
        m.publish(m, "esp32/testdir", "Q" + encount)
        print("Turning right " + str(deg) + " degrees, encount: " + str(encount))
        time.sleep(2)

    #################################################################################
    # turnRight turns the rover right by 90 degrees (roughly)
    # it does this by sending a hardcoded turn time to the right
    @staticmethod
    def turnRight(__self__, m: MQTTClient):
        # turns the robot right 90 degrees
        # will turn the robot for 0.26 seconds
        m.publish(m, "esp32/dir", "R0250")
        print("Turning Right!")
        # m.publish(m, "esp32/status", "right90")
        time.sleep(1)
    #################################################################################

    #################################################################################
    # stop stops the rover
    # it does this by sending the stop character to the ESP32
    @staticmethod
    def stop(__self__, m: MQTTClient):
        # stops the robot
        m.publish(m, "esp32/dir", "S0100")
        print("Stopping!")
        time.sleep(0.1)
    #################################################################################