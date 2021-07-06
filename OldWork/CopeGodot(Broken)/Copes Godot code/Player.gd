class_name Player
extends Node2D

var drawColor = Color(200, 200, 200, 255)
var breached = false
var lines = []
# export var chalkTexture: Image
var chalkTexture = load("res://ChalkTexture-02.png")
var Geometry = load("res://Geometry.gd")
# var chalkTexture = get_texture()
#var name = ''
#* How random the line can be
var stocasticness = .80

#! ADD add_child()!!

var trackMouse = false

func percent(chance):
#    randomize()
    return rand_range(0, 100) < chance


func _ready():
    randomize()
    # print(type)
    # chalkTexture.shrink_x2()

    var data = []
    for i in 50:
        data.append(Vector2(i, i))
    addLine(Line.new(Vector2(0, 0), Vector2(50, 50), data))


func drawLine(data):
#    draw_polyline(data, drawColor, 1, true)
    draw_texture_rect_region()

    # var spacing = round((Geometry.getLineLen(Line.new(null, null, data)) / chalkTexture.get_width()) / stocasticness)
    # print('spacing: ', spacing)
    # print('line lenghth: ', Geometry.getLineLen(Line.new(null, null, data)))
    # var sinceLastDraw = 0
    # for i in data:
    #     sinceLastDraw += 1
    #     if sinceLastDraw >= spacing:
    #         if percent(100 * stocasticness / 2):
    #             sinceLastDraw = 0
    #             draw_texture(chalkTexture, i)

func _draw():
   #draw_circle(Vector2(100, 100), 15, Color(255, 0, 100))
    for line in lines:
        drawLine(line.points)


func addLine(line):
    self.lines.append(line)


func _process(delta):
    #* If you press esc, it closes the program
    if Input.is_action_pressed('ui_cancel'):
        get_tree().quit()

    # if Input.is_mouse_button_pressed(1):
    #     pass

    # update()
        # print('mousePressed!')


func _input(event):
    update()
    print(lines.size())
    print(lines)
    # Mouse in viewport coordinates.
    if event is InputEventMouseButton:
        if Input.is_mouse_button_pressed(1):
            print('mouse pressed')
            var l = Line.new(event.position)
            lines.append(l)
            add_child(lines[-1])
        else:
            print('mouse released')
            lines[-1].finish(event.position, null, false)


    elif event is InputEventMouseMotion:
        # print('mouse moved')
        # print("Mouse Motion at: ", event.position)
        if Input.is_mouse_button_pressed(1):
            lines[-1].append(event.position)
