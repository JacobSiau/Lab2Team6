from CommandCenter.MQTTClient import MQTTClient
import time
from math import ceil


class RobotMover:

    debug = True
    vlinear = 50.45 # cm/s
    tquarter = 0.26 # s

    @staticmethod
    def moveToPoint(__self__, xo, yo, xf, yf, m: MQTTClient):
        dx = round(xf - xo, 2)
        print("dx: " + str(dx))
        dy = round(yf - yo, 2)
        print("dy: " + str(dy))
        __self__.goForward(__self__, dx, m)
        if dy < 0:
            __self__.turnRight(__self__, m)
        else:
            __self__.turnLeft(__self__, m)
        __self__.goForward(__self__, dy, m)

    @staticmethod
    def goForward(__self__, dx, m: MQTTClient):

        # moves the robot forward a distance dx
        # moves it at a speed of 50.45 cm/s

        # get the time to move the robot
        dtnum = round(dx / __self__.vlinear, 2)
        print("dtnum: " + str(dtnum))
        # check if within range
        if dtnum < 0.0 or dtnum > 9.99:
            print("ERROR: Forward time > 9s!")
            return
        # dt will be a string in the form <#>.<#> or <#>.<#><#> etc..
        dt = str(float(dtnum))
        print("dt: " + dt)
        # from here we know the str is 0.0 to 9.9 (numerically),
        # now we concatenate its digits without the decimal
        # to send to the robot
        timestr = ""
        for i in range(0, len(dt)):
            if i is not 1:
                timestr += dt[i]
        if len(timestr) is 3:
            timestr += '0'
        print("timestr " + timestr)
        # ready to send to robot
        m.publish(m, "esp32/dir", "F" + timestr)
        if __self__.debug is True:
            print("Going Forward! Distance: " + str(dx))
            m.publish(m, "esp32/status", "forward")
        time.sleep(ceil(dtnum))

    @staticmethod
    def turnLeft(__self__, m: MQTTClient):
        # turns the robot left 90 degrees
        # will turn the robot for 0.26 seconds
        m.publish(m, "esp32/dir", "L0200")
        if __self__.debug is True:
            print("Turning Left!")
            m.publish(m, "esp32/status", "left_turn")
        time.sleep(1)

    @staticmethod
    def turnRight(__self__, m: MQTTClient):
        # turns the robot right 90 degrees
        # will turn the robot for 0.26 seconds
        m.publish(m, "esp32/dir", "R0200")
        if __self__.debug is True:
            print("Turning Right!")
            m.publish(m, "esp32/status", "right_turn")
        time.sleep(1)