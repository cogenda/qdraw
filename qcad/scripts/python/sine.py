import qcad
import math

def start(str):
	n=0        # counter
	res=32     # resolution of sin
	
	# x-axis:
	qcad.rsPyAddLine(0, 0, 2*math.pi, 0)
	while n<2*math.pi:
		qcad.rsPyAddLine(n, 0, n, -0.05)
		n+=math.pi/4

	# y-axis:
	qcad.rsPyAddLine(0, -1, 0, 1)

	# sinus
	n=0.0
	step=(2*math.pi)/res
	while n<2*math.pi:
		qcad.rsPyAddLine(n, math.sin(n), n+step, math.sin(n+step))
		n+=step
	return(1)
