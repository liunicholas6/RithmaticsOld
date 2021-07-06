class_name Cope





#*########################TODO:###########################
# Finish making lines eraseable
# Line::IdentifyLine() function
#? Hook up GUI for a menu (imgui, nuklear)
# Add options to menu
# Chalk color
# Add modes
# Tutorial
# Example duels
# Show named funcenses
# Practice mode
# Duel against a stationary funcense
# Analyze mode
# Draw without your lines doing anything, but have it analyze them and print your accuracy
# Helper mode
# Whenever you draw a circle, lines appear subdividing it for you so you don't have to intuit it
# (maybe change how many lines with the scrollwheel?)
# Survival Mode!!
# How long can YOU survive against a horde of wild chalklings?
# Autocorrect Mode
# Maybe consider folding this into another mode, but having the program, when you finish drawing a line,
#    delete it and replace it with the optimal version of that line.
# Add an AI to play against
# Add a starting screen
# Add networking
# Make lines of vigor shoot out
# Allow lines to get broken parts
# Allow lines to get pushed back
# Make it so when you draw, instead of regular lines, it looks like you're drawing chalk
# Add a beginner option, for people who just learned, are drawing with a mouse, or both
#/






func todo(featureName):
    print(featureName + 'hasn\'t been implemented yet!')





func percent(percentage):
    """ Usage:
        if (percent(50)):
            <code that has a 50% chance of running>
    """
    return randint(1, 100) < percentage



func closeEnough(a, b, tolerance):
    return a <= b + tolerance and a >= b - tolerance






func center(string):
    """ Centers a string for printing in the terminal
    """
    # from os import get_terminal_size
    for _ in range(int((get_terminal_size().columns - len(string)) / 2)): string = ' ' + string
    return string



func isPowerOf2(x):
    return (x != 0) and ((x & (x - 1)) == 0)



func isBetween(val, start, end, beginInclusive=False, endInclusive=False):
    return (val >= start if beginInclusive else val > start) and \
           (val <= end   if endInclusive   else val < end)



func collidePoint(topLeft: 'Point', size: Union[tuple, list, 'Size'], target, inclusive=True):
    return isBetween(target.x, topLeft.x, size[0], beginInclusive=inclusive, endInclusive=inclusive) and \
           isBetween(target.y, topLeft.y, size[1], beginInclusive=inclusive, endInclusive=inclusive)



func insertChar(string, index, char):
    return string[:index] + char + string[index+1:]



func constrain(val, low, high):
    return min(high, max(low, val))



func rgbToHex(rgb):
    """translates an rgb tuple of int to a tkinter friendly color code"""
    return f'#{int(rgb[0]):02x}{int(rgb[1]):02x}{int(rgb[2]):02x}'



func darken(rgb, amount):
    """ Make amount negative to lighten
    """

    return tuple([constrain(i+amount, 0, 255) for i in rgb])



func translate(value, fromStart, fromEnd, toStart, toEnd):
    return ((abs(value - fromStart) / abs(fromEnd - fromStart)) * abs(toEnd - toStart)) + toStart


func frange(start, stop, skip=1.0, accuracy=10000000000000000):
    return [x / accuracy for x in range(int(start*accuracy), int(stop*accuracy), int(skip*accuracy))]




func loadAsset(dir, name, extension='png'):
    return loadImage(dir + name + '.' + extension)



func getDist(a, b):
    return math.sqrt(((b.x - a.x)**2) + ((b.y - a.y)**2))
