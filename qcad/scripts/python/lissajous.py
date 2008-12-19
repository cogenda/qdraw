import math
import qcad

def start(str):
	x_m = 100.0   # x amplitude
	y_m = 100.0   # y amplidute
	w_x = 0.9     # x angle frequency
	w_y = 0.7     # y angle frequency
	t = 0.0       # time (counter)
	x = 0.0       # current pos
	y = 0.0
	ox = 32000    # old pos
	oy = 32000
	res = 128     # resolution (number of lines)
	
	# lissajous
	while t<math.pi*10:
		x = x_m * math.cos(w_x * t)
		y = y_m * math.cos(w_y * t)
		if ox!=32000:
			qcad.rsPyAddLine(ox, oy, x, y)
		ox = x
		oy = y
		t += 0.1
	return(1)
