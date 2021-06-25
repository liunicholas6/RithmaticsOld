class_name Arena
extends Node2D

#var background = Color(.2, .2, .2, .7)
var players = []

func _ready():
#    var data = []
#    for i in 5:
#        data.append(Vector2(10*i, 10*1))
#    $Root.addLine(Line.new(Vector2(0, 0), Vector2(50, 50), data))
    _draw()


func _draw():
    draw_circle(Vector2(10, 10), 15, Color(255, 0, 100))
#    for player in players:
#        for line in player.lines:
#            draw_polygon(line.points, player.drawColor)


func _process(delta):
    #* If you press esc, it closes the program
    if Input.is_action_pressed('ui_cancel'):
        get_tree().quit()
