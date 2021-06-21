extends CollisionShape2D

func _ready():
	position = get_viewport_rect().end/2
	shape.extents = position+Vector2(50,50)
