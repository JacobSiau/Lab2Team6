from CommandCenter.MQTTClient import MQTTClient
import time
from math import ceil, pi


class RobotMover:

    wheel_radius = 3.25 # cm
    robot1_avg_speed = 46 # cm/s
    robot2_avg_speed = 68.5  # cm/s
    move_topic = "esp32/m"
    slow_topic = "esp32/r"
    robot1_timedleft90deg_msg = "K0290"
    robot1_timedright90deg_msg = "Q0290"
    robot2_timedleft90deg_msg = "K0285"
    robot2_timedright90deg_msg = "Q0325"

    #################################################################################
    # goForward moves the rover forward a distance dx
    # it does this by publishing the correct motor direction and encoder count
    @staticmethod
    def goForward(__self__, robot_num, dx, m: MQTTClient):
        if robot_num != 1 and robot_num != 2:
            print("goForward called for invalid robot_num: " + str(robot_num))
            return
        if dx < 0 or dx > 244:
            print("goForward called for out of range dx: " + str(dx))
            return
        print("goForward called for robot " + str(robot_num) + ", dx: " + str(dx))
        time_count = 0
        topic = None
        if robot_num is 1:
            time_count = str(round(1000 * (dx / __self__.robot1_avg_speed)))
            topic = "esp32/m1t"
        else:
            time_count = str(round(1000 * (dx / __self__.robot2_avg_speed)))
            topic = "esp32/m2t"
        print("time_count: " + time_count)
        if len(time_count) is 1:
            time_count = '000' + time_count
        elif len(time_count) is 2:
            time_count = '00' + time_count
        if len(time_count) is 3:
            time_count = '0' + time_count
        elif len(time_count) > 4:
            print("Error with time_count!")
        print("Final time_count: " + time_count)
        m.publish(m, topic, "F" + time_count)
    #################################################################################

    #################################################################################
    # turnRobot turns the robot left or right by 90 degrees (roughly)
    @staticmethod
    def turnRobot(__self__, robot_num, direction, m: MQTTClient):
        if robot_num != 1 and robot_num != 2:
            print("turnRobot called for invalid robot: " + str(robot_num))
            return
        if direction != 'L' and direction != 'R':
            print("turnRobot called with invalid direction: " + str(direction))
        print("turnRobot called for robot " + str(robot_num) + ", dir: " + str(direction))
        if robot_num == 1:
            if direction == 'L':
                m.publish(m, "esp32/m1t", __self__.robot1_timedleft90deg_msg)
            elif direction == 'R':
                m.publish(m, "esp32/m1t", __self__.robot1_timedright90deg_msg)
        else:
            if direction == 'L':
                m.publish(m, "esp32/m2t", __self__.robot2_timedleft90deg_msg)
            elif direction == 'R':
                m.publish(m, "esp32/m2t", __self__.robot2_timedright90deg_msg)
    #################################################################################

    #################################################################################
    @staticmethod
    def goBackward(__self__, robot_num, dx, m: MQTTClient):
        if robot_num != 1 and robot_num != 2:
            print("goBackward called for invalid robot: " + str(robot_num))
            return
        if dx < 0 or dx > 244:
            print("goBackward called for out of range dx: " + str(dx))
            return
        print("goBackward called for dx: " + str(dx))
        time_count = 0
        topic = None
        if robot_num is 1:
            time_count = str(round(1000 * (dx / __self__.robot1_avg_speed)))
            topic = "esp32/m1t"
        else:
            time_count = str(round(1000 * (dx / __self__.robot2_avg_speed)))
            topic = "esp32/m2t"
        print("time_count: " + time_count)
        if len(time_count) is 1:
            time_count = '000' + time_count
        elif len(time_count) is 2:
            time_count = '00' + time_count
        if len(time_count) is 3:
            time_count = '0' + time_count
        elif len(time_count) > 4:
            print("Error with time_count!")
        print("Final time_count: " + time_count)
        m.publish(m, topic, "B" + time_count)
    #################################################################################

    #################################################################################
    @staticmethod
    def stopRobot(__self__, robot_num, m: MQTTClient):
        if robot_num != 1 and robot_num != 2:
            print("stopRobot called with invalid robot_num: " + str(robot_num))
            return
        print("stopRobot called for robot_num: " + str(robot_num))
        topic = ""
        if robot_num is 1:
            m.publish(m, "esp32/r1", "S")
        else:
            m.publish(m, "esp32/r2", "S")
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