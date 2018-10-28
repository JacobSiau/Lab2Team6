from CommandCenter.SwarmData import SwarmData as S
import time

while True:
    time.sleep(0.5)
    S.getData(S)
    print("\n")
    S.displayBall(S)
    S.displayRed(S)
    S.displayBlue(S)
