extends Node2D

var _mouse_down = false
var _mouse_pos
var _points = []
var _curve = Curve2D.new()

func _input(event):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT:
			_mouse_down = event.pressed
			if event.pressed == false:
				add_child(pickLine())
				_points = []
				_curve = Curve2D.new()

func _process(_delta):
	_mouse_pos = get_viewport().get_mouse_position()
	if _mouse_down && (_points.size() == 0 || (_points.back() - _mouse_pos).length()>5):
		_points.append(_mouse_pos)
	if !_mouse_down:
		pass
	_curve.clear_points()
	if _points.size()>2:
		_curve.add_point(_points[0])
		for i in range(1 , _points.size() - 1):
			setHandles(_points[i-1], _points[i], _points[i+1])
		_curve.add_point(_points[_points.size() - 1])
	update()

func setHandles(prev, point, next, t = 0.5):
	var d1 = (point - prev).length()
	var d2 = (next - point).length()
	var s1 = t * d1 / (d1 + d2)
	var s2 = t * d2 / (d1 + d2)
	var p_in = -s1 * (next - prev)
	var p_out = s2 * (next - prev)
	_curve.add_point(point, p_in, p_out)

func pickLine():
	var line_types = [DrawnWarding, DrawnForbiddance, DrawnVigor]
	#Use Array[float] in Godot 4
	var error_thresholds:Array = [0.01, 0.02, 0.4]
	for i in range(line_types.size()):
		var line = line_types[i].new()
		var error = line.detect(_curve)
		if ! error == null and error < error_thresholds[i]:
			print("Line index " + String(i) + " error " + String(error))
			return line
	return Invalid.new()

func _draw():
	if _curve.get_baked_points().size()>2:
		draw_polyline(_curve.get_baked_points(), Color(255,255,255), 5.0)
