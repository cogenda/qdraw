import math
import qcad

def start(str):
	t=0.0      # time (counter)
	res=256.0  # resolution of curve
	x_m=100.0  # amplitude
	t_m=400.0  # max t value
	beta=0.04  # attenuation
	m=1.0      # mass
	w_d=0.05   # angular frequency without attenuation
	f=0.0      # function value
	fo=32000   # old function value
	step=t_m/res
	
	# x-axis:
	qcad.rsPyAddLine(0, 0, t_m, 0)
	#while t<2*math.pi:
	#	qcad.rsPyAddLine(n, 0, n, -0.05)
	#	t+=math.pi/4

	# y-axis:
	qcad.rsPyAddLine(0, -x_m, 0, x_m)

	# oscillation
	fo=32000
	t=0.0
	while t<t_m:
		f = x_m * math.exp((-beta/(2.0*m))*t) * math.sin(w_d * t)
		if (fo!=32000):
			qcad.rsPyAddLine(t-step, fo, t, f)
		t+=step
		fo=f
	
	# x_m:
	fo=32000
	t=0.0
	while t<t_m:
		f = x_m * math.exp((-beta/(2.0*m))*t)
		if (fo!=32000):
			qcad.rsPyAddLine(t-step, fo, t, f)
			qcad.rsPyAddLine(t-step, -fo, t, -f)
		t+=step
		fo=f
		
	return(1)
