import requests as req


class SwarmData:

    url = "http://192.168.137.1:8001/FieldData/GetData"
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
    old_positions = None
    # change_tolerances = (0.5, 0.2)  # tolerance for change in x and y directions
    robot: ()
    ball: ()

    #################################################################################
    # pixelMap maps a pixel pair (px, py) to a real coordinate pair (rx, ry).
    # The ranges are 0<=rx<=244 and 0<=ry<=122
    @staticmethod
    def pixelMap(self, px, py):
        # getting max, min and range for x and y positions
        xmin = min(i['X'] for i in self.positions['corners'])
        xmax = max(i['X'] for i in self.positions['corners'])
        ymin = min(i['Y'] for i in self.positions['corners'])
        ymax = max(i['Y'] for i in self.positions['corners'])
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
    def getData(self):
        # saving the old data before getting new data
        self.old_positions = self.positions
        # getting the new data
        data = req.get(self.url).json()
        # filling up class member variables
        self.positions['corners'] = data['Corners']
        self.positions['ball'] = self.pixelMap(
            self,
            data['Ball']['Object Center']['X'],
            data['Ball']['Object Center']['Y'])
        self.positions['red']['circle'] = self.pixelMap(
            self,
            data['Red Team Data']['Circle']['Object Center']['X'],
            data['Red Team Data']['Circle']['Object Center']['Y'])
        self.positions['red']['square'] = self.pixelMap(
            self,
            data['Red Team Data']['Square']['Object Center']['X'],
            data['Red Team Data']['Square']['Object Center']['Y'])
        self.positions['red']['triangle'] = self.pixelMap(
            self,
            data['Red Team Data']['Triangle']['Object Center']['X'],
            data['Red Team Data']['Triangle']['Object Center']['Y'])
        self.positions['blue']['circle'] = self.pixelMap(
            self,
            data['Blue Team Data']['Circle']['Object Center']['X'],
            data['Blue Team Data']['Circle']['Object Center']['Y'])
        self.positions['blue']['square'] = self.pixelMap(
            self,
            data['Blue Team Data']['Square']['Object Center']['X'],
            data['Blue Team Data']['Square']['Object Center']['Y'])
        self.positions['blue']['triangle'] = self.pixelMap(
            self,
            data['Blue Team Data']['Triangle']['Object Center']['X'],
            data['Blue Team Data']['Triangle']['Object Center']['Y'])
    #################################################################################

    # #################################################################################
    # # checkIfBallMoved compares the current and old values of the ball positions
    # # and indicates whether it moved beyond a certain tolerance
    # @staticmethod
    # def checkIfBallMoved(self):
    #     if self.old_positions is not None:
    #         old = self.old_positions['ball']
    #         new = self.positions['ball']
    #         for i in range(0, 2):
    #             if abs(new[i] - old[i]) > self.change_tolerances[i]:
    #                 # return True
    #                 print("Ball moved")
    # #################################################################################

    #################################################################################
    # displayBall displays the data for the ball object
    @staticmethod
    def displayBall(self):
        print("[Ball Data]\n")
        print("Ball: " + str(self.positions['ball']))
    #################################################################################

    #################################################################################
    # displayRed displays the data for the red team objects
    @staticmethod
    def displayRed(self):
        print("[Red Team Data]\n")
        print("Circle: " + str(self.positions['red']['circle']))
        print("Square: " + str(self.positions['red']['square']))
        print("Triangle: " + str(self.positions['red']['triangle']))
    #################################################################################

    #################################################################################
    # displayBlue displays the data for the blue team objects
    @staticmethod
    def displayBlue(self):
        print("[Blue Team Data]\n")
        print("Circle: " + str(self.positions['blue']['circle']))
        print("Square: " + str(self.positions['blue']['square']))
        print("Triangle: " + str(self.positions['blue']['triangle']))
    #################################################################################







