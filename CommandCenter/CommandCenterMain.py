from CommandCenter.SwarmData import SwarmData as S
import time

while True:
    time.sleep(0.5)
    S.retrieveJSON(S)
    S.updateData(S)
    S.updateCoords(S)
    print("\n")
    print("Blue Square: " + str(S.blue['square']))
    print("Blue Tri: " + str(S.blue['triangle']))
    print("Blue Circle: " + str(S.blue['circle']))
    print("Ball: " + str(S.ball))
