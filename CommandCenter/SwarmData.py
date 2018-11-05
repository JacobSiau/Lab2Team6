import requests as req
from pprint import pprint


class SwarmData:

    url = "http://172.16.0.1:8001/FieldData/GetData"
    positions = {
        'ball': (),
        'corners': {},
        'red': {
            'circle': (),
            'square': (),
            'triangle': ()
        },
        'blue': {
            'circle': (),
            'square': (),
            'triangle': ()
        }
    }
    oldpositions = None

    #################################################################################
    # pixelMap maps a pixel pair (px, py) to a real coordinate pair (rx, ry).
    # The ranges are 0<=rx<=244 and 0<=ry<=122
    @staticmethod
    def pixelMap(__self__, px, py):
        # getting max, min and range for x and y positions
        xmin = min(i['X'] for i in __self__.positions['corners'])
        xmax = max(i['X'] for i in __self__.positions['corners'])
        ymin = min(i['Y'] for i in __self__.positions['corners'])
        ymax = max(i['Y'] for i in __self__.positions['corners'])
        xrange = xmax - xmin
        yrange = ymax - ymin
        return round(244 * (px - xmin) / xrange, 2), \
               round(122 * (1 - (py - ymin) / yrange), 2)
    #################################################################################

    #################################################################################
    # getData is the overall data retrieval method for the SwarmData class
    # it calls the get request method from the HTTP library
    # it then fills up the member object variable with the data based on the keys
    @staticmethod
    def getData(__self__):
        # saving the old data before getting new data
        __self__.oldpositions = __self__.positions
        # getting the new data
        data = req.get(__self__.url).json()
        # filling up class member variables
        __self__.positions['corners'] = data['Corners']
        __self__.positions['ball'] = __self__.pixelMap(
            __self__,
            data['Ball']['Object Center']['X'],
            data['Ball']['Object Center']['Y'])
        __self__.positions['red']['circle'] = __self__.pixelMap(
            __self__,
            data['Red Team Data']['Circle']['Object Center']['X'],
            data['Red Team Data']['Circle']['Object Center']['Y'])
        __self__.positions['red']['square'] = __self__.pixelMap(
            __self__,
            data['Red Team Data']['Square']['Object Center']['X'],
            data['Red Team Data']['Square']['Object Center']['Y'])
        __self__.positions['red']['triangle'] = __self__.pixelMap(
            __self__,
            data['Red Team Data']['Triangle']['Object Center']['X'],
            data['Red Team Data']['Triangle']['Object Center']['Y'])
        __self__.positions['blue']['circle'] = __self__.pixelMap(
            __self__,
            data['Blue Team Data']['Circle']['Object Center']['X'],
            data['Blue Team Data']['Circle']['Object Center']['Y'])
        __self__.positions['blue']['square'] = __self__.pixelMap(
            __self__,
            data['Blue Team Data']['Square']['Object Center']['X'],
            data['Blue Team Data']['Square']['Object Center']['Y'])
        __self__.positions['blue']['triangle'] = __self__.pixelMap(
            __self__,
            data['Blue Team Data']['Triangle']['Object Center']['X'],
            data['Blue Team Data']['Triangle']['Object Center']['Y'])
    #################################################################################

    #################################################################################
    # displayBall displays the data for the ball object
    @staticmethod
    def displayBall(__self__):
        print("[Ball Data]\n")
        print("Ball: " + str(__self__.positions['ball']))
    #################################################################################

    #################################################################################
    # displayRed displays the data for the red team objects
    @staticmethod
    def displayRed(__self__):
        print("[Red Team Data]\n")
        print("Circle: " + str(__self__.positions['red']['circle']))
        print("Square: " + str(__self__.positions['red']['square']))
        print("Triangle: " + str(__self__.positions['red']['triangle']))
    #################################################################################

    #################################################################################
    # displayBlue displays the data for the blue team objects
    @staticmethod
    def displayBlue(__self__):
        print("[Blue Team Data]\n")
        print("Circle: " + str(__self__.positions['blue']['circle']))
        print("Square: " + str(__self__.positions['blue']['square']))
        print("Triangle: " + str(__self__.positions['blue']['triangle']))
    #################################################################################







