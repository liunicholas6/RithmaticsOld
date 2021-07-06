class_name Line
extends Line2D

var start = Vector2()
var end = Vector2()
var accuracy = null
# var points = PoolVector2Array() setget ,getData
# var points =
var finished = false
var type
# var texture = null

enum{
    LINE_FORBIDDENCE,
    LINE_MAKING,
    LINE_REVOCATION,
    LINE_DOT,
    LINE_VIGOR,
    LINE_WARDING,
}

static func getTypeName(type):
    match type:
        LINE_DOT:
            return "dot"
        LINE_MAKING:
            return "Line of Making"
        LINE_REVOCATION:
            return "Line of Revocation"
        LINE_VIGOR:
            return "Line of Vigor"
        LINE_WARDING:
            return "Line of Warding"
        LINE_FORBIDDENCE:
            return "Line of Forbiddence"
        _:
            return "~~~ Something has gone horribly, horribly wrong. ~~~"


func _init(start, end=null, data=null):
    self.start = start
    self.end = end
    if data:
        self.points = data
    self.width = 10
    # self.texture = load("res://ChalkTexture-02.png")
    # tile, stretch, none
    # self.texture_mode = LINE_TEXTURE_TILE
    # sharp, bevel, round
    # self.joint_mode = LINE_JOINT_ROUND
    # box, round
    # self.begin_cap_mode = LINE_CAP_ROUND
    # self.end_cap_mode = LINE_CAP_ROUND


func identify():
    return Geometry.identify()


#func _draw():
#    return
#    draw_polygon(self.points, drawColor)


func finish(end, data=null, identify=true):
    self.end = end
    finished = true

    if not points.size():
        points.append(start)
        type = LINE_DOT
    elif identify:
        self.identify()
        printAcc()

    if data:
        points = data


#func erase():
#    queue_free()


func append(point: Vector2):
    self.points.append(point)


func printAcc():
    var name = getTypeName(type)

    if accuracy >= 50.0:
        print("I'm  " + accuracy + "% sure that is a " + name + ".")
    elif type == null:
        print("I have no idea what that is supposed to be.")
    else:
        assert(false)
        # std::cout << g::getDebugCount() << ": " << "That is not a " << name << ". (I'm " << std::setprecision(1) << std::fixed << 50.f - accuracy << "% sure)\n"
        # # std::cout << g::getDebugCount() << ": " << "That is not a " << name << ". (" << std::setprecision(1) << std::fixed << accuracy << "%)\n"


# func getData():
    # check if start/end are empty, if so, add them
    # if start:
    #     tmp.insert(tmp.begin(), start)
    #     if(end.isNull or isFinished)
    #         tmp.append(end)
    # }
    # Because I don't know how to do a deep copy
    # return ([start] if start else []) + points + ([end] if end else [])


func update():
    pass
    """
    assert(false)
    # I hate operator overloads
    if (this.lineColor != playersColor){
    # if (this. lineColor.r != playersColor.r or this. lineColor.g != playersColor.g or this. lineColor.b != playersColor.b or this. lineColor.a != playersColor.a){
        this.lineColor = playersColor
        vertices.clear()
        # this.vertices = new std::vector<float>(reUpdate(*points, playersColor))
        # return this.vertices
        # return vertices
    }

    if (isDot) lineType = LINE_DOT
    # if (isFinished){
        # identifyLine()
        # printAcc()
    # }

    int startSize = vertices.size()

    # if (isFinished and not points.size()){
    #     assert(start.x == end.x and start.y == end.y)
    #     points.append(start)
    # }

    if(!(isFinished) and (vertices.size() != (points.size() * 6))){
        for(auto it = points.begin() + (vertices.size() / 6) it != points.end() ++it){
            addVertices(getVector(*it))
        }
    }
    else if (isFinished){
        # addVertices(start.getVector())
        if (points.size() > 1){
            for(auto it = points.begin() + (vertices.size() / 6) it < points.end() ++it){
                addVertices(getVector(*it))
            }
        }
        # size == 1
        else if(points.size()){
            addVertices(getVector(start))
            # addVertices(Point(start.x    , start.y + 1).getVector())
            # addVertices(Point(start.x + 1, start.y + 1).getVector())
            # addVertices(Point(start.x + 1, start.y    ).getVector())
        }
        # addVertices(end.getVector())
    }

    # logVal(points.size())
    # logVal(vertices.size())
    # logVal(isFinished)

    # Create a new subvector of the stuff we just added
    # Cream: Just scraping the good stuff off the top
    std::vector<float>* cream = new std::vector<float>

    # This loop was WAY harder than it should have been.
    for (int i = startSize i < vertices.size() ++i)
        cream.append((*vertices)[i])

    return cream
    """



"""
class LINE_FORBIDDENCE:
    pass


class LINE_VIGOR:
    pass


class LINE_WARDING:
    pass


class LINE_MAKING:
    pass


class LINE_REVOCATION:
    pass


class LINE_DOT:
    pass
"""
