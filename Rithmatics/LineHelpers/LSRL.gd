#Finds a line of best fit for an array of Vector2s
class_name LSRL
static func get_dir(points:PoolVector2Array):
	var dx:float = 0
	var dy:float = 0
	for pt in points:
		dx += pt.x
		dy += pt.y
	if dy == 0:
		return Vector2.RIGHT
	if dx == 0:
		return Vector2.DOWN
	var a:float = 0
	var b:float = 0
	for pt in points:
		a += pow(pt.x, 2) - pow(pt.y, 2)
		b += pt.x * pt.y
	var angle:float = 0.5 * atan(2 * b / a)
	if a < 0:
		angle += PI/2
	return Vector2(cos(angle), sin(angle))
