extends Label

func _ready():
	pass

func _process(_delta):
	visible = Singleton.get_Debug()
