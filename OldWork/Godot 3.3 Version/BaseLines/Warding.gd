extends RigidBody2D
class_name Warding

var _points:PoolVector2Array = []
var _radius:float = 0
var collision:CollisionShape2D
var _damageFactor:float = 0.5

func _draw():
	for i in range(_points.size()-1):
		draw_line(_points[i], _points[i+1], Color.white, 5)
