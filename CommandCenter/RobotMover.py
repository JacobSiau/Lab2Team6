from CommandCenter.MQTTClient import MQTTClient
import time
from math import ceil, pi


class RobotMover:

    wheel_radius = 3.25 # cm
    early_stop_x = 15 # cm
    tolerance_x = 15 # tolerance for x in cm
    tolerance_y = 10 # tolerance for y in cm
    move_topic = "esp32/m"
    slow_topic = "esp32/r"
    robot1_left90deg_msg = "K0029"
    robot1_right90deg_msg = "Q0028"
    robot1_timedleft90deg_msg = "K0342"
    robot1_timedright90deg_msg = "Q0352"

    #################################################################################
    # moveToPoint moves the rover from a point xo,yo to a point xf, yf
    # it does this by moving the rover straight, turning the rover left or right,
    # and then moving the rover straight again
    @staticmethod
    def moveToPoint(__self__, xo, yo, xf, yf, m: MQTTClient):
        # how far it needs to go
        dx = round(xf - xo, 2) - __self__.tolerance_x
        print("Robot needs to change x by: " + str(dx))
        dy = round(yf - yo, 2) - __self__.tolerance_y
        print("Robot needs to change y by: " + str(dy))


        # if dy < 0:
        #     __self__.turnRobot(__self__, robot_num=1, direction='R', m=m)
        #     time.sleep(3)
        #     __self__.goForward(__self__, robot_num=1, dx=dy, m=m)
        #     time.sleep(3)
        #     __self__.turnRobot(__self__, robot_num=1, direction='L', m=m)
        #     time.sleep(3)
        #     __self__.goForward(__self__, robot_num=1, dx=dx, m=m)
        #     time.sleep(3)
        # else:
        #     __self__.turnRobot(__self__, robot_num=1, direction='L', m=m)
        #     time.sleep(3)
        #     __self__.goForward(__self__, robot_num=1, dx=dy, m=m)
        #     time.sleep(3)
        #     __self__.turnRobot(__self__, robot_num=1, direction='R', m=m)
        #     time.sleep(3)
        #     __self__.goForward(__self__, robot_num=1, dx=dx, m=m)
        #     time.sleep(3)

    #################################################################################

    @staticmethod
    def goForwardNormally(__self__, robot_num, m: MQTTClient):
        print("goForwardNormally called")
        if robot_num != 1 and robot_num != 2:
            print("Invalid robot_num passed: " + str(robot_num))
            return
        temp = __self__.slow_topic + str(robot_num)
        m.publish(m, temp, "F")

    @staticmethod
    def goForwardSlowly(__self__, robot_num, m: MQTTClient):
        print("goForwardSlowly called")
        if robot_num != 1 and robot_num != 2:
            print("Invalid robot_num passed: " + str(robot_num))
            return
        temp = __self__.slow_topic + str(robot_num)
        m.publish(m, temp, "G")

    @staticmethod
    def stopRobot(__self__, robot_num, m: MQTTClient):
        print("stopRobot called")
        if robot_num != 1 and robot_num != 2:
            print("Invalid robot_num passed: " + str(robot_num))
            return
        temp = __self__.slow_topic + str(robot_num)
        m.publish(m, temp, "S")

    #################################################################################
    # goForward moves the rover forward a distance dx
    # it does this by publishing the correct motor direction and encoder count
    @staticmethod
    def goForward(__self__, robot_num, dx, m: MQTTClient):
        print("goForward called")
        if robot_num != 1 and robot_num != 2:
            print("Invalid robot_num passed: " + str(robot_num))
            return
        temp = __self__.move_topic + str(robot_num)
        print("dx: " + str(dx))
        print("temp: " + temp)
        # number of encoder counts to roll
        enc_count = str(round((40 * dx) / (2*pi*__self__.wheel_radius)))
        print("enc_count: " + enc_count)
        if len(enc_count) is 1:
            enc_count = '000' + enc_count
        elif len(enc_count) is 2:
            enc_count = '00' + enc_count
        if len(enc_count) is 3:
            enc_count = '0' + enc_count
        elif len(enc_count) > 4:
            print("Faulty enc_count!")
        print("Final enc_count: " + enc_count)
        m.publish(m, temp, "F" + enc_count)
    #################################################################################

    #################################################################################
    # turnRobot turns the robot left or right by 90 degrees (roughly)
    @staticmethod
    def turnRobot(__self__, robot_num, direction, m: MQTTClient):
        print("turnRobot called")
        if robot_num != 1 and robot_num != 2:
            print("Invalid robot_num passed: " + str(robot_num))
            return
        temp = ""
        if robot_num is 1:
            temp = "esp32/m1t"
        else:
            temp = "esp32/m2t"
        print("robot_num: " + str(robot_num) + ", topic: " + temp)
        print("direction: " + str(direction))
        if direction == 'L':
            m.publish(m, temp, __self__.robot1_timedleft90deg_msg)
        elif direction == 'R':
            m.publish(m, temp, __self__.robot1_timedright90deg_msg)
        else:
            print("Invalid direction passed: " + str(direction))
        time.sleep(4)
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
        m.publish(m, "esp32/m1", "B" + encount)
    #################################################################################
