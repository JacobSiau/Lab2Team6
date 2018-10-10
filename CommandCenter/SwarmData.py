import requests as req
import json


class SwarmData:

    url = "http://172.16.0.1:8001/FieldData/GetData"
    data = {}
    ball = ()
    corners = {}
    red = {}
    blue = {}
    xcoords = []
    ycoords = []

    @staticmethod
    def retrieveJSON(__self__):
        __self__.data = req.get(__self__.url).json()

    @staticmethod
    def updateData(__self__):
        # easier object names and access
        __self__.ball = (__self__.data['Ball']['Object Center']['X'],
                         __self__.data['Ball']['Object Center']['Y'])
        __self__.corners = __self__.data['Corners']
        __self__.red = {
            'circle': (__self__.data['Red Team Data']['Circle']['Object Center']['X'],
                       __self__.data['Red Team Data']['Circle']['Object Center']['Y']),
            'square': (__self__.data['Red Team Data']['Square']['Object Center']['X'],
                       __self__.data['Red Team Data']['Square']['Object Center']['Y']),
            'triangle': (__self__.data['Red Team Data']['Triangle']['Object Center']['X'],
                         __self__.data['Red Team Data']['Triangle']['Object Center']['Y'])}
        __self__.blue = {
            'circle': (__self__.data['Blue Team Data']['Circle']['Object Center']['X'],
                       __self__.data['Blue Team Data']['Circle']['Object Center']['Y']),
            'square': (__self__.data['Blue Team Data']['Square']['Object Center']['X'],
                       __self__.data['Blue Team Data']['Square']['Object Center']['Y']),
            'triangle': (__self__.data['Blue Team Data']['Triangle']['Object Center']['X'],
                         __self__.data['Blue Team Data']['Triangle']['Object Center']['Y'])}

    @staticmethod
    def updateCoords(__self__):
        # grabbing the x and y coordinates of the pairs in corners
        for d in __self__.corners:
            __self__.xcoords.append(d['X'])
            __self__.ycoords.append(d['Y'])

        # sort the coordinates ascending so we can find max, min, and range for x and y
        __self__.xcoords.sort()
        __self__.ycoords.sort()

    # pixelmap maps a pixel pair (px, py) to a real coordinate pair (rx, ry).
    # The ranges are 0<=rx<=8 and 0<=ry<=4
    @staticmethod
    def pixelmap(__self__, px, py):
        xmax = __self__.xcoords[3]
        xmin = __self__.xcoords[0]
        ymax = __self__.ycoords[3]
        ymin = __self__.ycoords[0]
        xrange = xmax - xmin
        yrange = ymax - ymin
        return 8 * (px - xmin) / xrange, 4 * (1 - (py - ymin) / yrange)


