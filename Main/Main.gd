extends Node

var surface = preload("res://Main/MainSurfaceScripts/Surface.tscn")

func _ready():
	add_child(surface.instance())
func _input(event):
	if event.is_action_pressed("ui_accept"):
		get_tree().paused = !get_tree().paused
	if event is InputEventKey:
		if event.pressed and event.scancode == KEY_D:
			Singleton.set_Debug(!Singleton.get_Debug())
