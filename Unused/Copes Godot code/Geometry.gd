# class_name Geo
extends Object

const LINE_FORBIDDENCE_PRECISION = 100.0
# In getExponentialAvergeDeviation(), use this value to punish distace
const DISTANCE_PENALTY_EXPONENT = 1.5
# How many radius samples are taken (the greater the number, the more accurate, but slower it's going to be)
#  Put line.points.size() for max accuracy
# const MAX_RADIUS_SAMPLES = line.points.size()
# Defines how many points forward or backward to check when assuming the circle's radius based on it's size
#  Used in efficent getAverageRadius()
const OFFSET_CHECK_DISTANCE = 10
# Defines the step of the slow way of calculating getFurthestVector2s() (smaller is slower, but more accurate)
const FIND_RADIUS_SLOW_EFFICENCY = 1



static func guessType(line):
    assert(line.finished)

    if line.points.size() == 1:
        return Line.LINE_DOT

    # 8 is arbitrary
    elif getDist(line.start, line.end) < getLineLen(line) / 8:
        return Line.LINE_WARDING

    # The length of the line of a sine wave is approx. sqrt(pow(amp, 2) + pow(wav / 4, 2))
    # 1.1 is arbitrary, but pretty good
    elif getDist(line.start, line.end) < getLineLen(line) / 1.1:
        return Line.LINE_VIGOR

    elif closeEnough(getDist(line.start, line.end) / getLineLen(line), 1.0, 0.3):
        return Line.LINE_FORBIDDENCE

    elif false:
        return Line.LINE_REVOCATION

    elif false:
        return Line.LINE_MAKING

    else:
        return null


static func getAccuracy(line, typeGuess):
    match typeGuess:
        Line.LINE_DOT, Line.LINE_UNKNOWN, Line.LINE_MAKING, Line.LINE_REVOCATION:
            return null
        Line.LINE_VIGOR:
            var detectedData = analyzeSine(line)
            var detectedAmplitude  = detectedData[0]
            var detectedWavelength = detectedData[1]
            var detectedCycles     = detectedData[2]
            var detectedPhaseShift = detectedData[3]
            var maxAmpDev          = detectedData[4]
            #            = *detectedData

            print(# << "Wavelength = " << wavelength << "\n"
                    "Detected Wavelength = " + detectedWavelength + "\n"
                    # << "Amplitude = " << amplitude << "\n"
                    + "Detected Amplitude = " + detectedAmplitude + '\n'
                    # << "Cycles = " << cycles << '\n'
                    + "Detected Cycles = " + detectedCycles << '\n'
                    + "Detected Phase Shift = " + detectedPhaseShift + '\n'
                    + "Detected Maximum Amplitude Deviation = " + maxAmpDev + '\n'
            )

            if detectedAmplitude == null or detectedCycles == null or detectedWavelength == null or detectedPhaseShift == null:
                # return null
                return 0

            var optSine = genOptSine(line.start, line.end, detectedWavelength, detectedAmplitude, detectedCycles, detectedPhaseShift, true)
            #                                            neither is this value ||
            #     For sine, this isn't always super accurate \/                \/
            var answer = 100 - (getDeviation(optSine, line.points)[1] / 1.5)# - (maxAmpDev / 3)
            return answer
            # return answer < 0 ? 0 : answer # If it's negative, just say 0.

        Line.LINE_WARDING:
            var radius = getAverageRadius(false)
            var center = getCenter(line, radius)

            var optCircle = genOptCircle(center, radius, false)

            var answer = 100 - getDeviation(optCircle, line.points)[1] - (getDeviation(optCircle, line.points)[2] * 2)
            return 0 if answer < 0 else answer # If it's negative, just say 0.

        Line.LINE_FORBIDDENCE:
            # getAverageDeviation() here can be approximated with getDist(start, end) / getLineLen().
            var optLine = genOptLine(line.start, line.end)
            # return Line.LINE_FORBIDDENCE_PRECISION - getAverageDeviation(optLine) * Line.LINE_FORBIDDENCE_PRECISION / 10

            # logVal(getExponentialAverageDeviation(optLine))
            # logVal(getGreatestDeviation(optLine).second)
            # logVal(((getExponentialAverageDeviation(optLine) * -1) - getGreatestDeviation(optLine).second) * -1)

            var answer = 100 - getDeviation(optLine, line.points, DISTANCE_PENALTY_EXPONENT)[1] - (getDeviation(optLine, line.points)[2] * 2)

            return 0 if answer < 0 else answer # If it's negative, just say 0.

        _:
            return


# Used for circle and maybe ellipse
static func getAverageRadius(line): #, efficent=true):
    var radii = []
    # var samples = MAX_RADIUS_SAMPLES

    # The smarter way of doing things
    """
    if efficent and false:
        # _log("Effeciently calculating radius...")
        var sampler = line.points.begin()
        var antiSampler = line.points.begin() + (points.size() / 2)

        #todo this doesnt work...
        while (samples){
            double finalDist = getDist(*sampler, *antiSampler)

            if (getDist(*sampler, *antiSampler) == finalDist)
                break
            elif (getDist(*sampler, *(antiSampler + 1)) > finalDist)
                while(getDist(*sampler, *(++antiSampler)) > finalDist) { }
            elif (getDist(*sampler, *(antiSampler + 1)) > finalDist)
                while(getDist(*sampler, *(--antiSampler)) > finalDist) { }

            radii.append(getDist(*sampler, *antiSampler) / 2)

            --samples
        }
    }
    """
    # The easy way of doing things
    # else:
        # _log("Ineffeciently calculating radius...")
    for i in line.points: #(auto it = line.points.begin() it < line.points.end() it += (line.points.size() / MAX_RADIUS_SAMPLES))
        radii.append(findFurthestPoint(i, line.points)[1] / 2)

    return getAverage(radii)


# Get the most likely center based on the radius of the circle
static func getCenter(line, radius):
    var bottommost = getBottommostPoint(line.points)
    var rightmost  = getRightmostPoint(line.points)
    var leftmost   = getLeftmostPoint(line.points)
    var topmost    = getTopmostPoint(line.points)
    var rightSays  = Vector2(rightmost.x - radius, rightmost.y)
    var leftSays   = Vector2(leftmost.x  + radius, leftmost.y)
    var topSays    = Vector2(topmost.x,    topmost.y    + radius)
    var bottomSays = Vector2(bottommost.x, bottommost.y - radius)

    return getAverage([rightSays, leftSays, topSays, bottomSays])


#! WARNING
#  This fuction does not clamp the length of the sine wave between the start and end points. They are there to get the proper angle
#  of the wave. You have to input the correct number of cycles yourself to get the right length.
static func genOptSine(start, end, wavelength, amplitude, cycles=2.0, phaseShift=7.0, niceLooking=false):
    #**************** Formula of a Sine Wave *****************
    # @p A amplitude
    # @p f cycles
    # @p t time
    # @p shift phase shift
    #
    #* y(t) = A * sin(2 * PI * f * t + shift)
    #                         or...
    #* y(time) = amplitude * sin(2 * PI * cycles * time + phaseShift)
    # P.S. This is wrong it doesn't work.
    #*********************************************************

    # I love the internet so much
    # https:#gamedev.stackexchange.com/questions/121478/how-to-rotate-a-2d-line

    var opt = Vector2()
    var x
    var y
    var dx
    var dy
    var newX
    var newY
    var v = atan2(end.y - start.y, end.x - start.x) - (PI / 2)

    # Create the actual sine wave, then change the angle of that sine wave
    for x in range(start.x, wavelength * cycles + start.x):
        # Calculate y based on x, via the sine equation
        y = amplitude * sin(((2 * PI) * (x + phaseShift)) / wavelength) + start.y

        # Calculate how far off you are from the center
        dx = x - start.x
        dy = y - start.y

        # Rotate around the starting point
        newX = dx * cos(v) - dy * sin(v) + start.x
        newY = dx * sin(v) + dy * cos(v) + start.y

        # Add that point to the new vector
        opt.append(Vector2(newX, newY))

    # This should NOT have been this difficult to figure out.
    """ #? This way works, so I want to keep it, but the other way is more efficent
        double x, y
        double verticalShift = start.y

        double v = atan2(end.y - start.y, end.x - start.x) - (PI / 2)

        y = amplitude * sin(((2 * PI) * (start.x + phaseShift)) / wavelength) + verticalShift
        int newY = (start.x *  cos(v)) + (y * sin(v))
        int newX = (start.x * -sin(v)) + (y * cos(v))

        int xOffset = start.x - newX + 7
        int yOffset = start.y - newY


        # Create the actual sine wave, then change the angle of that sine wave
        for(x = start.x x < (wavelength * cycles) + start.x ++x){
            y = amplitude * sin(((2 * PI) * (x + phaseShift)) / wavelength) + verticalShift
            int newY = (x *  cos(v)) + (y * sin(v))
            int newX = (x * -sin(v)) + (y * cos(v))
            opt.append(Vector2(newX + xOffset, newY + yOffset))
        }
    """

    """ #! Dont delete this unless youve gone over it first
    # Because that only generates one point per x, draw lines between the points
    #  (actually looks slightly worse when drawn with OpenGL this way)
    if (not niceLooking){
        std::vector<Vector2> tmpOpt = opt
        opt.clear()
        assert(tmpOpt.size() > 2)
        for(auto it = tmpOpt.begin() + 1 it < tmpOpt.end() ++it){
            # opt.append(*(it - 1))
            std::vector<Vector2> tmp = genOptLine(*it, *(it - 1))
            appendVector(opt, tmp)
            # opt.append(*it)
        }
    }
    """
    return opt


# The ordered flag toggles whether to order the points for nice drawing or not
static func genOptCircle(center, radius, ordered=false):
    var opt
    # main, Bottom Right, Bottom Left, Top Left: starts at the Top Right
    var optTR1
    var optBR1
    var optBL1
    var optTL1
    var optTR2
    var optBR2
    var optBL2
    var optTL2

    # Mid-Vector2 Circle Drawing Algorithm -- Works, but draws the points out of order, making OpenGL draw them funny
    var x = radius
    var y = 0

    opt.append(Vector2(center.x + radius, center.y))
    if ordered: # To make OpenGL draw the starting gap
        opt.append(Vector2(center.x + radius, center.y + 1))

    # When radius is zero only a single point will be printed
    if radius > 0:
        pass
        # opt.append(Vector2(x + center.x, -y + center.y))
        # opt.append(Vector2(y + center.x, x + center.y))
        # opt.append(Vector2(-y + center.x, x + center.y))

    # Initialising the value of P
    var P = 1 - radius
    while x > y:
        ++y

        # Mid-point is inside or on the perimeter
        if P <= 0:
            P = P + 2*y + 1
        # Mid-point is outside the perimeter
        else:
            --x
            P = P + 2*y - 2*x + 1

        # All the perimeter points have already been printed
        if x < y:
            break

        # Printing the generated point and its reflection
        # in the other octants after translation
        if ordered:
            optTR1.append(Vector2(x + center.x, y + center.y))
            optTL1.append(Vector2(-x + center.x, y + center.y))
            optBR1.append(Vector2(x + center.x, -y + center.y))
            optBL1.append(Vector2(-x + center.x, -y + center.y))
        else:
            opt.append(Vector2(x + center.x, y + center.y))
            opt.append(Vector2(-x + center.x, y + center.y))
            opt.append(Vector2(x + center.x, -y + center.y))
            opt.append(Vector2(-x + center.x, -y + center.y))

        # If the generated point is on the line x = y then the perimeter points have already been printed
        if x != y:
            if ordered:
                optTR2.append(Vector2(y + center.x, x + center.y))
                optTL2.append(Vector2(-y + center.x, x + center.y))
                optBR2.append(Vector2(y + center.x, -x + center.y))
                optBL2.append(Vector2(-y + center.x, -x + center.y))

            else:
                opt.append(Vector2(y + center.x, x + center.y))
                opt.append(Vector2(-y + center.x, x + center.y))
                opt.append(Vector2(y + center.x, -x + center.y))
                opt.append(Vector2(-y + center.x, -x + center.y))




    if ordered:
        # Now assemble all those points properly
        # This ordering works, but it looks messed up.
        #  I think I messed up the octant naming or something.
        for it in optBR1:
            opt.append(it)
        for it in optBR2.invert():
            opt.append(it)
        for it in optBL2:
            opt.append(it)
        for it in optBL1.invert():
            opt.append(it)

        opt.append(Vector2(center.x - radius, center.y))

        for it in optTL1:
            opt.append(it)
        for it in optTL2.invert():
            opt.append(it)
        for it in optTR2:
            opt.append(it)
        for it in optTR1.invert():
            opt.append(it)

    else:
        opt.append(Vector2(center.x - radius, center.y))

    if ordered:
        opt.append(Vector2(center.x + radius, center.y - 1))

    # opt.append(Vector2(center.x + radius, center.y))

    """
        errors between octants:
        TL1/TR1 and TL1/TL2

    */


    /*
    8            1               TR 1
            xxx
    7    x       x   2           TR 2
       x           x
       x           x
       x           x
    6    x       x   3           BR 1
            xxx
    5             4              BR 2
    """

    """ Bresenham’s Algorithm -- Works, same problem
    var x = 0
    var y = radius
    var d = 3 - 2 * radius

    opt.append(Vector2(center.x + x, center.y + y))
    opt.append(Vector2(center.x - x, center.y + y))
    opt.append(Vector2(center.x + x, center.y - y))
    opt.append(Vector2(center.x - x, center.y - y))
    opt.append(Vector2(center.x + y, center.y + x))
    opt.append(Vector2(center.x - y, center.y + x))
    opt.append(Vector2(center.x + y, center.y - x))
    opt.append(Vector2(center.x - y, center.y - x))

    while (y >= x){
        # for each pixel we will draw all eight pixels
        x++

        # check for decision parameter and correspondingly update d, x, y
        if (d > 0){
            y--
            d = d + 4 * (x - y) + 10
        }
        else
            d = d + 4 * x + 6

        opt.append(Vector2(center.x + x, center.y + y))
        ^
        opt.append(Vector2(center.x + x, center.y - y))
        opt.append(Vector2(center.x - x, center.y - y))
        opt.append(Vector2(center.x + y, center.y + x))
        opt.append(Vector2(center.x - y, center.y + x))
        opt.append(Vector2(center.x + y, center.y - x))
        opt.append(Vector2(center.x - y, center.y - x))
    }
    """
    """
    /* Organize the points to be in order
    std::vector<Vector2> organizedOpt
        xxx
      x       x
    x           x
    x           x
    x           x
      x       x
        xxx
        ^
    start here

    # Get the bottommost point
    assert(opt.size())
    Vector2 bottomLeft
    for(auto it: opt)
        if(it.y > bottomLeft.y)
            if(it.x < bottomLeft.x)
                bottomLeft = it

    organizedOpt.append(bottomLeft)
    """

    return opt


static func genOptLine(start, end):
    var opt

    """ Native line drawing algorithm - doesnt currently work
    int dx = end.x - start.x
    int dy = end.y - start.y

    bool positiveSlope = start.x > end.x

    for (int x = start.x positiveSlope ? (x >= end.x) : (x <= end.x) positiveSlope ? --x : ++x){
        int y = ((start.y + dy) * (x - start.x)) / dx
        _log(std::to_string(x) + ", " + std::to_string(y))
        opt.append(Vector2(x, y))
    }
    """

    # https:#en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)

    var step
    var x1 = start.x
    var y1 = start.y
    var x2 = end.x
    var y2 = end.y
    var x = x1
    var y = y1


    var dx = x2 - x1
    var dy = y2 - y1

    if abs(dx) >= abs(dy):
        step = abs(dx)
    else:
        step = abs(dy)

    dx = dx / step
    dy = dy / step


    var i = 1
    while i <= step:
        # putpixel(x, y, 5)
        opt.append(Vector2(x, y))
        x = x + dx
        y = y + dy
        i = i + 1

    return opt


static func genOptEllipse():
    pass


# Returns a list in the order of amplitude, wavelength, cycles, phaseShift, maxAmpDev
# todo clamp between start and end distances somehow
# todo clamp between start and end distances somehow
# todo clamp between start and end distances somehow
static func analyzeSine(line):
    line = rotatedStraight(line, true)

    var amplitude
    var wavelength
    var cycles
    var phaseShift
    var maxAmpDev

    #* IMPORTANT: Remember that origin is the top left, so y is inverted from intuition


    var humpData = getHumps(line.points)
    var goesUpFirst = humpData[0]
    var topHumps = humpData[1]
    var bottomHumps = humpData[2]

    # logVal(topHumps.size())
    # logVal(bottomHumps.size())

    if not topHumps.size() or not bottomHumps.size():
        amplitude  = null
        wavelength = null
        cycles     = null
        phaseShift = null
        maxAmpDev  = null
        return [amplitude, wavelength, cycles, phaseShift, maxAmpDev]


    #* Calculate Cycles
    # The meat of the matter: the number of points from the first hump to the last hump
    # Get the indecies of the first and last humps
    var meatCountBegin = topHumps.front() if goesUpFirst else bottomHumps.front()
    var meatCountEnd   = topHumps.back() if topHumps.back() > bottomHumps.back() else bottomHumps.back()
    var meatCount      = meatCountEnd - meatCountBegin

    var totalHumps = topHumps.size() + bottomHumps.size()

    # How many points aren't accounted for in the meat
    var totalFractionalCyclePoints = line.points.size() - meatCount

    # How many points are in each cycles, on average?
    var averagePointsPerCycle = meatCount / totalHumps

    # How many partial cycles (beginning and the ending) are there?
    var fractionalCycles = totalFractionalCyclePoints / averagePointsPerCycle

    cycles = (totalHumps + fractionalCycles) / 2


    #* Calculate Phase Shift
    var preCyclePoints  = meatCountBegin
    var postCyclePoints = line.points.size() - meatCountEnd

    # if ((preCycleVector2s + postCycleVector2s) )

    var preCycles  = preCyclePoints  / averagePointsPerCycle
    var postCycles = postCyclePoints / averagePointsPerCycle

    phaseShift = --preCycles if preCycles >= 1 else preCycles

    # Since we're calculating each induvidual hump seperately, we need to detect if the
    #  partial part is partially going up or down. The sign represents the slope of the line.
    # if (not goesUpFirst)
    #     phaseShift *= -1

    if not goesUpFirst:
        phaseShift += averagePointsPerCycle * 2


    #* Calculate Amplitude
        # Calculate out all the amplitudes
        var topHumpAverageAmp = 0
        var bottomHumpAverageAmp = 0
        var averageAmp = 0
        var topHumpTotalAmp = 0
        var bottomHumpTotalAmp = 0
        var totalAmp = 0

        # Calculate the top amplitudes
        # for (auto i: topHumps){
        #     topHumpTotalAmp += Line.LINEDATA[i].y - line.start.y
        # }

        # Calculate the bottom amplitudes
        # for (auto i: bottomHumps){
        #     bottomHumpTotalAmp += Line.LINEDATA[i].y - line.start.y
        # }

        # logVal(totalHumps)

        var currentAmp = -1.0
        var penultimateAmpDev = 0.0
        for i in range(0, totalHumps, 2): #int i = 0 i < totalHumps i += 2){
            currentAmp = bottomHumps[i] - topHumps[i]

            totalAmp += currentAmp
            if currentAmp > maxAmpDev:
                # if (currentAmp != -1.)
                    # penultimateAmpDev = maxAmpDev
                maxAmpDev = currentAmp

        #todo Something to add for the future: add top vs bottom average amplitude comparison into the main algorithm

        # averageAmp = (topHumpTotalAmp + bottomHumpTotalAmp) /  totalHumps
        averageAmp = totalAmp / totalHumps
        amplitude = abs(averageAmp) # abs() here shouldn't be necissary.


    #* Calculate Maximum Amplitude Deviation
        # logVal(maxAmpDev)
        # logVal(penultimateAmpDev)

        maxAmpDev = maxAmpDev - amplitude


    #* Calculate Wavelength
        wavelength = line.points.size() / cycles
    #undef Line.LINEDATA

    return [amplitude, wavelength, cycles, phaseShift, maxAmpDev]


static func identify(line):
    var guessedType = guessType(line)

    var acc = getAccuracy(line, guessedType)

    line.type = guessedType
    line.accuracy = acc

    # logVal(getDist(line.start, line.end) / getLineLen())

    return guessedType if acc > 50.0 else null


static func getLineLen(line):
    var total = 0.0

    for i in range(1, line.points.size()):
        total += getDist((line.points)[i], (line.points)[i - 1])

    return total + 1


static func getAverage(list):
    var total = Vector2()
    for i in list:
        total.x += i.x
        total.y += i.y

    return Vector2(total.x / list.size(), total.y / list.size())


static func getDist(a, b):
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2))
    # return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2))

# Gets the angle between the line that goes between the 2 points, and the right hand side x axis
#* Returns in radians
static func getSlope(a, b):
    var angle = -atan2(b.y - a.y, b.x - a.x)
    if angle < 0:
        angle += 2 * PI# + abs(angle)
    # return giveRadians ? angle: glm::degrees(angle)

    # You both suck.
    # return (double(b.y) - double(a.y)) / (double(b.x) - double(a.x)) # The traditional slope equation
    # return (double(a.y) - double(b.y)) / (double(b.x) - double(a.x)) # The slope equation for origin at top left corner

# For regular values
static func closeEnough(a, b, tolerance):
    return a <= b + tolerance and a >= b - tolerance

# For points specifically
static func nearEnough(a: Vector2, b: Vector2, threshold):
    return abs(getDist(a, b)) < threshold



static func getLeftmostPoint(data):
    var index
    var smallestX = 10000
    for i in data.size():
        if data[i].x < smallestX:
            smallestX = data[i].x
            index = i
    return index



static func getRightmostPoint(data):
    var index
    var largestX = 0
    for i in data.size():
        if data[i].x > largestX:
            largestX = data[i].x
            index = i
    return index



static func getTopmostPoint(data):
    var index
    var smallestY = 10000
    for i in data.size():
        if data[i].y < smallestY:
            smallestY = data[i].y
            index = i
    return index



static func getBottommostPoint(data):
    var index
    var largestY = 0
    for i in data.size():
        if data[i].y > largestY:
            largestY = data[i].y
            index = i
    return index




# Gets the points furthest from each other in the vector
#* This assumes it's at least vaugely a circle shape
# static func findFurthestPoint(data, efficency):
#     assert(false)
#     var a
#     var b
#     var i = 0

#     for i in range(0, data.size(), efficency): #( i < data.size() i += efficency){

#     return std::pair<int, int>(a, b)


# Finds the furthest point in the list to what it's given
static func findFurthestPoint(target, comparator):
    var index = 0
    var finalDist = 0
    var current

    for i in comparator:
        current = getDist(target, i)
        if current > finalDist:
            finalDist = current
            index = i

    return [target[index], finalDist]


# Finds the closest point in the list to what it's given
static func findClosestPoint(target, comparator):
    var index = 0
    var finalDist = 10000
    var current

    # return min( )

    for i in comparator:
        current = getDist(target, i)
        if current < finalDist:
            finalDist = current

    # logVal(finalDist)
    return [target[index], finalDist]


# Compare 2 vectors of points and get how much on average they're different
# Exactly the same as getAverageDeviation(), but punishes distance from the line exponentially
# Returns the [total deviation, average deviation, max deviation]
static func getDeviation(opt, subOpt, exponent=1):
    var netDist = 0.0
    var currentDist = 0.0
    var finalDist = 0.0

    for i in subOpt:
        netDist += pow(abs(findClosestPoint(i, opt)[1]), exponent)
        if currentDist > finalDist:
            finalDist = currentDist
            # index = i

    return [netDist, netDist / subOpt.size(), finalDist] # The average distance of the closest points


# Uses radians only
static func rotate(line, angle):
    assert(line.finished)

    var angleAdj = []

    #* The GLM Method
    # Create the rotation matrix
    # glm::mat4 rotateMat= glm::mat4(1.0f)
    # rotateMat = glm::rotate(rotateMat, float(angle), glm::vec3(0.0f, 0.0f, 1.0f))

    # Translate the points
    # glm::vec4 rotated = rotateMat * glm::vec4(line.start.x, line.start.y * -1, 0.0f, 1.0f)

    var dx
    var dy
    var newX
    var newY

    # I love the internet so much
    # https:#gamedev.stackexchange.com/questions/121478/how-to-rotate-a-2d-line

    #* Actually do the moving
    for i in line.points:
        dx = i.x - line.start.x
        dy = i.y - line.start.y
        newX = dx * cos(angle) - dy * sin(angle) + line.start.x
        newY = dx * sin(angle) + dy * cos(angle) + line.start.y

        # glm::vec4 rotated = rotateMat * glm::vec4(i.x, i.y * -1, 0.0f, 1.0f)

        angleAdj.append(Vector2(newX, newY))

    line.points = angleAdj
    line.end = line.points[-1]
    # line.end.print("Generated end point")
    # assert(line.start.x == line.points.front().x and line.start.y == line.points.front().y)


# Rotates any line to be straight along the x axis
#todo This algorithm funcinitely needs some work, but it works for now.
static func rotatedStraight(line, isSine=false):
    # float angle = atan2(line.end.y - line.start.y, line.end.x - line.start.x) - (PI / 2)
    var angle = getSlope(line.start, line.end)
    rotate(line, angle)

    if isSine:

        var humpData = getHumps(line.points)
        var firstHumpHigh = humpData[0]
        var topHumps = humpData[1]
        var bottomHumps = humpData[2]

        # logVal(topHumps.size())
        # logVal(bottomHumps.size())
        # logVal(bottomHumps.size())

        var totalBottomHumpSlopes  = 0
        var averageBottomHumpSlope = 0
        var totalTopHumpSlopes  = 0
        var averageTopHumpSlope = 0

        # getTopmostPoint(*line.points)

        for i in topHumps: #(auto it = topHumps.begin() + 1 it < topHumps.end() ++it){
            # Get the slope between the first hump, and all the other humps
            totalTopHumpSlopes += getSlope((line.points)[topHumps[0]], (line.points)[i])

        # logVal(totalTopHumpSlopes)
        # logVal(ave)

        averageTopHumpSlope = totalTopHumpSlopes / topHumps.size()
        """
        for(auto i: bottomHumps){
            # Get the slope between the first hump, and all the other humps
            totalBottomHumpSlopes += getSlope((*line.points)[bottomHumps[0]], (*line.points)[i])
        }
        averageBottomHumpSlope = averageBottomHumpSlope / double(bottomHumps.size())
        """

        # logVal(totalBottomHumpSlopes)

        # logVal(averageTopHumpSlope)
        # logVal(averageBottomHumpSlope)

        if firstHumpHigh:
            averageTopHumpSlope = (2 * PI) - averageTopHumpSlope
            # averageBottomHumpSlope = (2 * PI) - averageBottomHumpSlope
        else:
            averageTopHumpSlope -= PI
            # averageBottomHumpSlope -= PI

        # logVal(averageTopHumpSlope)
        # logVal(averageBottomHumpSlope)

        # double averageSlope = (averageTopHumpSlope + averageBottomHumpSlope) / 2

        rotate(line, -averageTopHumpSlope)


# Returns true if the first hump goes up instead of down
# Returns [if first hump goes up, topHumps, bottomHumps]
static func getHumps(line):
    var prevY
    var topHumps
    var bottomHumps
    var index = 0
    var goesUpFirst = true
    var itPrev = 0
    var it = 1
    var noDoWhileLoopHack = true
    var startGoingDown = false
    # var itPrev = line.points.begin()
    # var it = line.points.begin() + 1

    #* First, figure out if the sine is going up or down first
    while line.points[itPrev] == line.points[it] or noDoWhileLoopHack:
        noDoWhileLoopHack = false
        # You're going up
        if line.points[it].y < line.points[itPrev].y:
            prevY = 100000

        # You're going down
        if line.points[it].y > line.points[itPrev].y:
            prevY = 0
            goesUpFirst = false
            # Starts looking for the lowest point first, instead of the highest point first
            # This is one instance where a goto is actually a very reasonable choice.
            startGoingDown = true
            # goto startGoingDown




    # Go up until you start going down, and mark that index
    # Then go down until you start going up, and mark that index
    while index < line.points.size() - 1 or startGoingDown:

        #* Go up...
        if not startGoingDown:
            while line.points[index].y <= prevY:
                prevY = line.points[index].y
                ++index

                if index >= line.points.size():
                    return [goesUpFirst, topHumps, bottomHumps]

            #* For robustness (i.e. if a point goes forward then immediately back)
            if line.points[index - 1].y >= line.points[index + 1].y:
                topHumps.append(index - 1)

        #* And go down...
        startGoingDown = false
        while line.points[index].y >= prevY:
            prevY = line.points[index].y
            ++index

            if index >= line.points.size():
                return [goesUpFirst, topHumps, bottomHumps]

        #* For robustness (i.e. if a point goes forward then immediately back)
        if line.points[index - 1].y <= line.points[index + 1].y:
            bottomHumps.append(index - 1)

    # The logic should never get to this point
    assert(false)
    # return [goesUpFirst, topHumps, bottomHumps]




# These arent used, so I dont feel like porting them.
"""
static func findClosestPoint(target, comparatorList):
    "" Finds the closest point in the list to what it's given
    ""
    finalDist = 1000000

    for i in comparatorList:
        current = getDist(target, i)
        if current < finalDist:
            finalDist = current

    return finalDist



static func findClosestXPoint(target, comparatorList, offsetIndex=0):
    finalDist = 1000000
    result = 0

    # for i in range(len(comparatorList) - offsetIndex):
    for current in comparatorList:
        # current = comparatorList[i + offsetIndex]
        currentDist = abs(target.x - current.x)
        if currentDist < finalDist:
            result = current
            finalDist = currentDist

    return result



static func getPointsAlongLine(p1, p2):
    p1 = Vector2i(p1)
    p2 = Vector2i(p2)

    returnMe = []

    dx = p2.x - p1.x
    dy = p2.y - p1.y

    for x in range(p1.x, p2.x):
        y = p1.y + dy * (x - p1.x) / dx
        returnMe.append(Vector2f(x, y))

    return returnMe



static func rotatePoint(p, angle, pivotVector2, radians=False):
    if not radians:
        angle = math.radians(angle)
    # p -= pivotVector2
    # tmp = pygame.math.Vector2(p.data()).normalize().rotate(amount)
    # return Vector2f(tmp.x, tmp.y) + pivotVector2

    dx = p.x - pivotVector2.x
    dy = p.y - pivotVector2.y
    newX = dx * math.cos(angle) - dy * math.sin(angle) + pivotVector2.x
    newY = dx * math.sin(angle) + dy * math.cos(angle) + pivotVector2.y

    return Vector2f(newX, newY)



static func getMidPoint(p1, p2):
    assert type(p1) == type(p2)
    # return Vector2f((p1.x + p2.x) / 2, (p1.y + p2.y) / 2)
    return p1._initCopy((p1.x + p2.x) / 2, (p1.y + p2.y) / 2)
"""
