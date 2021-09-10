extends Vigor
class_name DrawnVigor

var _extrema:PoolVector2Array = []
var _ideal_points:PoolVector2Array = []
var _error:float = 0
var _timer = Timer.new()

func detect(curve:Curve2D):
	var global_points = curve.get_baked_points()
	
	#Setting Center
	for pt in global_points:
		position += pt
	position /= global_points.size()
	
	#Global to Local Transform (for position)
	for pt in global_points:
		_points.append(pt - position)
	
	#Getting Direction
	_dir = LSRL.get_dir(_points)
	var dtheta:float = 0.1
	for i in range(10):
		var up:float = get_height(_dir.rotated(dtheta))
		var down:float = get_height(_dir.rotated(-dtheta))
		dtheta /= 2
		_dir = _dir.rotated(dtheta if up < down else -dtheta)
	
	#Set First Element to +x
	if _points[0].rotated(-_dir.angle()).x < 0:
		_dir *= -1
		
	#Setting Rotation
	rotation = _dir.angle()
	
	#Global to Local Transform (for rotation)
	for i in range(_points.size()):
		_points[i] = _points[i].rotated(-_dir.angle())
	
	#Guessing peaks and troughs
	var potential:PoolVector2Array = []
	var center_peak:Vector2 = _points[0] #to be x=0 in the equaiton
	var left:float = 0
	var right:float = 0
	for i in range(3, _points.size()-4):
		for n in range(1, 3):
			left += _points[i-n].y
			right += _points[i+n].y
		left /= 3
		right /= 3
		if (left < _points[i].y) == (right < _points[i].y):
			potential.append(_points[i])
		elif potential.size() > 0:
			var extremum:Vector2 = potential[potential.size()/2]
			if abs(extremum.x) < abs(center_peak.x):
				 center_peak = extremum
			_extrema.append(extremum)
			potential = []
	
	#Avoiding Error
	if _extrema.size() == 0:
		return null

	#Guessing Amplitude
	_amp = get_height(Vector2.RIGHT)/2 * (1 if center_peak.y > 0 else -1)	
	
	#Guessing Omega
	_omega = PI * (_extrema.size()-1)/(_extrema[0].x-_extrema[-1].x)
	
	#2 Period Check
	if (4*PI/_omega > _points[0].x - _points[-1].x):
		return null
	
	#Guessing Phase
	_phase = _omega * center_peak.x
	
	#Generating Wave Points
	for pt in _points:
		var new_point:Vector2 = get_wave_point(pt.x)
		_error += pow(new_point.y - pt.y, 2)
		_ideal_points.append(new_point)
	
	_error /= _amp * _amp * _points.size()
	print(_error)
	return _error

func get_height(var dir:Vector2):
	var maximum = _points[0].cross(dir)
	var minimum = maximum
	for pt in _points:
		var temp = pt.cross(dir)
		maximum = max(maximum, temp)
		minimum = min(minimum, temp)
	return maximum - minimum
