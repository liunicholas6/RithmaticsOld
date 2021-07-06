class_name Arc
extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

# var rotation_angle = 50
# var angle_from = 75
# var angle_to = 195


# Called when the node enters the scene tree for the first time.
func _ready():
	_draw()





# extends Node2D

# export (Texture) var texture setget _set_texture

# func _set_texture(value):
#     # If the texture variable is modified externally,
#     # this callback is called.
#     texture = value  # Texture was changed.
#     update()  # Update the node's visual representation.

# func _draw():
#     draw_texture(texture, Vector2())


func draw_circle_arc_poly(center, radius, angle_from, angle_to, color):
	var nb_points = 32
	var points_arc = PoolVector2Array()
	points_arc.push_back(center)
	var colors = PoolColorArray([color])

	for i in range(nb_points + 1):
		var angle_point = deg2rad(angle_from + i * (angle_to - angle_from) / nb_points - 90)
		points_arc.push_back(center + Vector2(cos(angle_point), sin(angle_point)) * radius)
	draw_polygon(points_arc, colors)

# func _process(delta):
#     angle_from += rotation_angle * delta
#     angle_to += rotation_angle * delta

#     # We only wrap angles when both of them are bigger than 360.
#     if angle_from > 360 and angle_to > 360:
#         angle_from = wrapf(angle_from, 0, 360)
#         angle_to = wrapf(angle_to, 0, 360)
#     update()


func _draw():
	var center = Vector2(200, 200)
	var radius = 80
	var angle_from = 75
	var angle_to = 195
	var color = Color(1.0, 0.0, 0.0)
	draw_circle_arc_poly(center, radius, angle_from, angle_to, color)

# func _draw():
#     var center = Vector2(200, 200)
#     var radius = 80
#     var color = Color(1.0, 0.0, 0.0)

#     draw_circle_arc( center, radius, angle_from, angle_to, color )
