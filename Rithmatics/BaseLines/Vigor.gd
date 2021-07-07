extends Node2D
class_name Vigor

var _points:PoolVector2Array
var _drawn_points:PoolVector2Array = []
var _dir:Vector2

var _amp:float = 0 #amplitude
var _omega:float = 0 #multiplier for x
var _phase:float = 0 #phase shift
var _shift:float = 0 #vertical shift

var _unformed:bool = true
enum State {REFLECT_IN, MORPH, REFLECT_OUT}

func get_wave_point(x:float):
	return Vector2(x, _amp * cos(_omega * x - _phase) + _shift)

func _draw():
	_drawn_points = _points
	if _drawn_points.size() >= 2:
		draw_polyline(_drawn_points, Color.white, 5)
