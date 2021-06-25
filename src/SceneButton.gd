extends Button

# Annotations -- This is new to godot 4.0 (what I'm using because I had to build from source)
# https://docs.godotengine.org/en/latest/tutorials/scripting/gdscript/gdscript_basics.html#annotations
@export var scene: String
signal scene_recieved(scene)

func _ready():
    connect("pressed", self, "switch_scene")

func switch_scene():
    # Ugh, now I'm going to have to rewrite all my fancy goto code because they updated the language
    # *sigh*
    var s = yield(Cope.gotoScene(scene), "scene_ready")
    emit_signal("scene_recived", s)
