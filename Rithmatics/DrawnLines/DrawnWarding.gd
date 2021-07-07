extends Warding
class_name DrawnWarding
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
	
	#Finding Radius
	for pt in self._points:
		self._radius += pt.length()
	self._radius /= self._points.size()
	
	#Preventing Double Layer
	if curve.get_baked_length()/self._radius > 3 * PI:
		return null
		
	#Finding error
	for pt in self._points:
		_error += pow(pt.length()-self._radius, 2)	
	_error /= pow(self._radius, 2) * self._points.size()
	print(_error)
	return _error
