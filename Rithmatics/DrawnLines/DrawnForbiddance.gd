extends Forbiddance
class_name DrawnForbiddance

var _error:float = 0

func detect(curve:Curve2D):
	var global_points = curve.get_baked_points()
	
	#Setting Center
	for pt in global_points:
		self.position += pt
	self.position /= global_points.size()
	
	#Global to Local Transform
	for pt in global_points:
		self._points.append(pt - self.position)
	
	#Finding Direction
	self._dir = LSRL.get_dir(self._points)
	
	#Preventing Double Layer (will do eventually)
		
	#Finding error
	for pt in self._points:
		_error += pow(self._dir.cross(pt), 2)	
	_error /= self. _points.size() * \
			abs( (self._dir.dot(self._points[0])) - \
			self._dir.dot(self._points[-1]))
	print(_error)
	return _error
