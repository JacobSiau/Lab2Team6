from CommandCenter.SwarmData import SwarmData as Swarm
import time
from pprint import pprint


# print("Original Ball: " + str(ball))
# print("Coords: " + str(xcoords) + " " + str(ycoords))
# print("Max (x,y): " + str(xmax) + " " + str(ymax))
# print("Min (x,y): " + str(xmin) + " " + str(ymin))
# print("Range (x,y): " + str(xrange) + " " + str(yrange))
#
#
# ballpos = pixelmap(ball[0], ball[1])
# rtrianglepos = pixelmap(redteam['triangle'][0], redteam['triangle'][1])
# print("Real Ball Position: " + str(ballpos))
# print("Red Triangle Position: " + str(rtrianglepos))

while True:
    time.sleep(5)
    Swarm.retrieveJSON(Swarm)
    Swarm.updateData(Swarm)
    print("\n")
    print("Blue Square: " + str(Swarm.blueteam['square']))
    print("Blue Tri: " + str(Swarm.blueteam['triangle']))
    print("Ball: " + str(Swarm.ball))


