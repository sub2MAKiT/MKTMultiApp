t = float(input("t: "))
px = 0
py = -1
Pxp = 1
Pyp = 0
Px = 0
Py = 1
print("x: {} y: {}".format((1-t)*(1-t)*px+2*t*(1-t)*Pxp+t*t*Px,(1-t)*(1-t)*py+2*t*(1-t)*Pyp+t*t*Py))