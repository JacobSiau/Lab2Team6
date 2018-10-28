import requests as req


class SwarmData:

    url = "http://172.16.0.1:8001/FieldData/GetData"
    positions = {
        'ball': (),
        'corners': {},
        'red': {
            'circle',
            'square',
            'triangle'
        },
        'blue': {
            'circle',
            'square',
            'triangle'
        }
    }
    oldpositions = {}

    # pixelMap maps a pixel pair (px, py) to a real coordinate pair (rx, ry).
    # The ranges are 0<=rx<=8 and 0<=ry<=4
    @staticmethod
    def pixelMap(__self__, px, py):
        # getting max, min and range for x and y positions
        xmin = min(__self__.positions.corners.X)
        xmax = max(__self__.positions.corners.X)
        ymin = min(__self__.positions.corners.Y)
        ymax = max(__self__.positions.corners.Y)
        xrange = xmax - xmin
        yrange = ymax - ymin
        return 8 * (px - xmin) / xrange, 4 * (1 - (py - ymin) / yrange)

    @staticmethod
    def getData(__self__):
        # saving the old data before getting new data
        __self__.oldpositions = __self__.positions
        # getting the new data
        data = req.get(__self__.url).json()
        # easier object names and access
        __self__.positions.ball = (data['Ball']['Object Center']['X'],
                                   data['Ball']['Object Center']['Y'])
        __self__.positions.corners = data['Corners']
        __self__.positions.red = {
            'circle': __self__.pixelMap(
                data['Red Team Data']['Circle']['Object Center']['X'],
                data['Red Team Data']['Circle']['Object Center']['Y']
            ),
            'square': __self__.pixelMap(
                data['Red Team Data']['Square']['Object Center']['X'],
                data['Red Team Data']['Square']['Object Center']['Y']
            ),
            'triangle': __self__.pixelMap(
                data['Red Team Data']['Triangle']['Object Center']['X'],
                data['Red Team Data']['Triangle']['Object Center']['Y']
            )
        }
        __self__.positions.blue = {
            'circle': __self__.pixelMap(
                data['Blue Team Data']['Circle']['Object Center']['X'],
                data['Blue Team Data']['Circle']['Object Center']['Y']
            ),
            'square': __self__.pixelMap(
                data['Blue Team Data']['Square']['Object Center']['X'],
                data['Blue Team Data']['Square']['Object Center']['Y']
            ),
            'triangle': __self__.pixelMap(
                data['Blue Team Data']['Triangle']['Object Center']['X'],
                data['Blue Team Data']['Triangle']['Object Center']['Y']
            )
        }




