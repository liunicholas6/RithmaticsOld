extends ColorRect


#This should be replaced by an image of pavement at some point but I'm lazy
func _ready():
	rect_size = get_viewport_rect().end;
