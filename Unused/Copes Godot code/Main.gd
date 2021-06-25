extends Node

enum Mode {
    TUTORIAL,
    PASSIVE_WATCH,
    # EXAMPLE_DUELS,
    # SHOW_NAMED_DEFENSES,
    PRACTICE,
    SURVIVAL,
    SINGLEPLAYER,
    MULTIPLAYER
}


var arena
# var drawColor
var width
var height

func addLine(player, start):
    player.lines.append(Line(start))

func finishLine(player, point, data=null, identify=true):
    player.lines[-1].finish(point, data, identify)

func addData(player, point):
    player.lines[-1].append(point)


func draw(points):
    for i in players:
        for k in i.lines:
            draw_polygon(k.getData(), i.drawColor)




func addManualLines(player):
    return
    assert(false)
    Vector2 center(243, 150)
    Vector2 origin(0, 0)
    Vector2 opposite(width, height)

    #* Make a Straight Line
    Vector2 randEnd(125, 185)
    auto straightData = creator.genOptLine(center, randEnd)
    Line optStraight(center, randEnd, straightData)

    #* Make a Circle
    int radius = 50
    auto circleData = creator.genOptCircle(center, radius, true)
    Line optCircle(Point(center.x + radius, center.y), Point(center.x + radius, center.y), circleData)

    #* Make a Sine Wave
    double amplitude  = 20.
    double wavelength = 40.
    double cycles     = 4.
    double phaseShift = 7.
    Vector2 someStart(300, 100)
    # Vector2 someEnding(center.x + 50, center.y)
    # Vector2 someEnding(375, center.y + 20)
    Vector2 someEnding(someStart.x + 115, someStart.y + 115)
    # auto  sineData = creator.genOptSine(center, someEnding, wavelength, amplitude, cycles, phaseShift, false)
    # Line  optSine(center, someEnding, sineData)

    #* Analyze the straight line
    # optStraight.identifyLine()
    # optStraight.printAccDebug()

    #* Analyze the cirlce
    # optCircle.identifyLine()
    # optCircle.printAccDebug()

    #* Analyze the sine wave
    # double detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift
    # Geometry analyzer(&optSine)
    # analyzer.getSineData(detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift)

    # analyzer.getSineData(detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift)
    # std::cout << g::getDebugCount() << ": " << std::fixed << std::setprecision(5)
    #           << "Wavelength = " << wavelength << "\n"
    #           << "Detected Wavelength = " << detectedWavelength << "\n"
    #           << "Amplitude = " << amplitude << "\n"
    #           << "Detected Amplitude = " << detectedAmplitude << '\n'
    #           << "Cycles = " << cycles << '\n'
    #           << "Detected Cycles = " << detectedCycles << '\n'
    #           << "Phase Shift = " << phaseShift << '\n'
    #           << "Detected Phase Shift = " << detectedPhaseShift << '\n'

    # if(detectedAmplitude == NAN or detectedCycles == NAN or detectedWavelength == NAN or detectedPhaseShift == NAN)
    #     return

    # std::vector<Point> optSine = genOptSine(line->start, line->end, detectedWavelength, detectedAmplitude, detectedCycles, detectedPhaseShift)

    # optSine.identifyLine()
    # optSine.printAccDebug()

    #* Add the lines
    # player.lines->push_back(optStraight)
    # player.lines->push_back(optCircle)
    # player.lines->push_back(optSine)

    # Update the lines
    updateLines()
    createLines()


func run():

    Geometry creator
    Vector2 someStart(280, 120)

    SDL_Event event
    bool fullscreen = false, run = true, trackMouse = false, drawPoints = true
    bool paused = false, inOptionsMenu = false, drawOptionsInPause = false # dont touch the last one
    bool firstDraw = true, letItResume = true # If you only just opened the menu
    bool identify = true, ran = false
    bool shift = false, ctrl = false, alt = false
    Uint32 windowFlags = 0 # fudge variable
    unsigned int previousTime, currentTime, lastOutput = 0
    unsigned int halfSecondDelay = 1
    (*arena.players)["root"] = Player("root", Color(255, 0, 0))

    addManualLines(ROOT)

    while (run){
        # Update the mouse location - now done as needed
        # SDL_GetMouseState(&mouseLoc.x, &mouseLoc.y)

        # The event loop
        if (paused or inOptionsMenu)
            nk_input_begin(ctx)
        while (SDL_PollEvent(&event)){
            if (paused or inOptionsMenu)
                nk_sdl_handle_event(&event)

            switch(event.type){
                case SDL_QUIT:
                    run = false break
                case SDL_KEYDOWN:
                    # logVal(SDL_GetScancodeName(event.key.keysym.scancode)) # physical key
                    # logVal(SDL_GetKeyName(event.key.keysym.sym))           # simulated key
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            if (inOptionsMenu)
                                closeMenu(inOptionsMenu)
                            else if (paused)
                                closeMenu(paused)
                            else
                                run = false
                            break
                        case 'F':
                        case 'f':
                            fullscreen = !fullscreen
                            if (fullscreen)
                                SDL_SetWindowFullscreen(window, windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP)
                            else
                                SDL_SetWindowFullscreen(window, windowFlags)
                            break
                        case 'c':
                            arena.clear()
                            trackMouse = false
                            ran = false #! This is only here for debugging
                            break
                        case SDLK_RCTRL:
                        case SDLK_LCTRL:
                            ctrl = true
                            break
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            shift = true
                            break
                        case SDLK_RALT:
                        case SDLK_LALT:
                            alt = true
                            break
                        case 'z':
                            if(ctrl){
                                _log("Ctrl + z pressed")
                                USER.lines->back().erase()
                            }
                            break
                        case SDLK_TAB:
                            drawPoints = !drawPoints break
                        case 'M':
                        case 'm':
                            if (inOptionsMenu)
                                closeMenu(inOptionsMenu)
                            else{
                                inOptionsMenu = true
                                firstDraw = true
                            }
                            break
                        case 'P':
                        case 'p':
                            if (paused)
                                closeMenu(paused)
                            else{
                                paused = true
                                # letItResume = true
                            }
                            break
                        case 'D':
                        case 'd':
                            # if (shift)
                            # Run the "Run once" code again
                                ran = false break
                        # case '':

                    }
                    break
                case SDL_KEYUP:
                    switch (event.key.keysym.sym){
                        case SDLK_RCTRL:
                        case SDLK_LCTRL:
                            ctrl = false
                            break
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            shift = false
                            break
                        case SDLK_RALT:
                        case SDLK_LALT:
                            alt = false
                            break
                        }
                    break
                case SDL_MOUSEBUTTONDOWN:
                    updateMouse(event.button.x, event.button.y)
                    if (event.button.button == SDL_BUTTON_LEFT and not (paused or inOptionsMenu)) {
                        g::log("Mouse pressed", 5)
                        # lastOutput = SDL_GetTicks()

                        if (not trackMouse){
                            trackMouse = true

                            addLine("user", mouseLoc)

                            updateLines()
                        }
                        break
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT and not (paused or inOptionsMenu)){
                        break
                    }
                    else if (event.button.button == SDL_BUTTON_MIDDLE){
                        _log("The mouse is at " + std::to_string(event.button.x) + ", " + std::to_string(event.button.y))
                        break
                    }
                    break
                # When mouse leaves the window or the window is resized
                case 512:
                    if (not trackMouse)
                        break
                    identify = false
                case SDL_MOUSEBUTTONUP:
                    updateMouse(event.button.x, event.button.y)
                    if ((event.button.button == SDL_BUTTON_LEFT or event.type == 512) and not (paused or inOptionsMenu)){
                        g::log("Mouse released", 5)
                        if (trackMouse){
                            # if(USER.lines->back().points->size()){
                            finishLine("user", mouseLoc, identify)
                            identify = true

                            # assert(trackMouse)
                            trackMouse = false

                            updateLines()
                            createLines()
                        }
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT and not (paused or inOptionsMenu)){
                        break
                    }
                    else if (event.button.button == SDL_BUTTON_MIDDLE and not (paused or inOptionsMenu)){
                        break
                    }
                    break
                case SDL_MOUSEMOTION:
                    updateMouse(event.button.x, event.button.y)
                    if (trackMouse and not (paused or inOptionsMenu)){

                        addData("user", mouseLoc)

                        updateLines()
                        createLines()

                        g::log("Mouse is moving, and I'm tracking it.", 5)

                        # lastOutput = SDL_GetTicks()
                    /*
                        # Behold my superb debugging skillz
                        logVal(arena.vertices->size() / 6)
                        logVal((*arena.players)["user"].lines->size())
                        # for (auto i: *(*arena.players)["user"].lines){
                        for (auto i = (*arena.players)["user"].lines->begin() i != (*arena.players)["user"].lines->end() ++i){
                            g::log("--- " + std::to_string(i - (*arena.players)["user"].lines->begin()) + " ---")
                            logVal(i->points->size())
                            logVal(i->vertices->size() / 6)
                            logVal(i->isFinished)
                        }
                        _endl g::log("--------------")
                    */
                    }
                    break
                case SDLK_PRINTSCREEN:
                    break
                default:
                    # std::cout << "Unknown event type: " << event.type << std::endl
                    break
            }
        }
        if (paused or inOptionsMenu)
            nk_input_end(ctx)


        #* Runs Once
        if (not ran){
            if (USER.lines->size() and USER.lines->back().isFinished){
                # _print(rotating...) _endl
                # creator.rotateStraight(USER.lines->back())
                ROOT.lines->push_back(Line())
                if (ROOT.lines->size() < 2){
                    _print(creating a new line...) _endl
                    ROOT.lines->back() = USER.lines->back()
                }
                else
                    ROOT.lines->back() = *(ROOT.lines->end() - 1)


                # creator.rotateStraight(ROOT.lines->back())
                # creator.rotate(ROOT.lines->back(), 30)

                updateLines()
                createLines()
                ran = true
            }
        }

        if(not (halfSecondDelay % (MAX_FPS / 2))){
            # logVal(ran)
            # getGlState()
            # logVal(nk_window_is_closed(ctx, "Options"))
            # logVal(nk_window_is_closed(ctx, "Paused"))

            # double amplitude  = 20.
            # double wavelength = 40.
            # double cycles     = 4.

            # ++someStart.x
            # ++someStart.y

            # # Vector2 someEnding(center.x, center.y + 50)
            # # Vector2 someEnding(375, center.y + 20)
            # Vector2 someEnding(someStart.x + 115, someStart.y + 115)
            # auto  sineData = creator.genOptSine(someStart, someEnding, wavelength, amplitude, cycles, true)
            # Line  optSine(someStart, someEnding, sineData)
            # # ROOT.lines->back(). = optSine # sineData
            # # ROOT.lines->pop_back()
            # ROOT.lines->push_back(optSine)

            if (USER.lines->size() and USER.lines->back().isFinished){
                # Line* l = new Line(USER.lines->back())
                # Geometry geo(USER.lines->back())
                # USER.lines->back().start.print("Start from user")
                # USER.lines->back().end.print("End from user")

                #* I DO NOT KNOW HOW THIS IS POSSIBLE

                #* Line tmpLine = USER.lines->back()
                # std::cout << &USER.lines->back() _endl

                # USER.lines->end()->end.print("Gui")
                # tmpLine.end.print("tmpLine")

                #* tmpLine.points->push_back(USER.lines->back().end)

                #* tmpLine.end = USER.lines->back().end

                # USER.lines->back().print("Gui")
                # tmpLine.print("tmpLine")

                # USER.lines->back().points->push_back(USER.lines->back().end)
                #* std::vector<Point> tmp = creator.getAmplitude1(tmpLine)
                # USER.lines->back().print("Gui2")
                #* ROOT.lines->push_back(Line(tmp.front(), tmp.back(), tmp))
                # tmp.front().print("Start")
                # tmp.back().print("End")

                # Geometry analyzer(&USER.lines->back())

                /*
                double detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift



                analyzer.getSineData(detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift)

                std::cout << g::getDebugCount() << ": " << std::fixed << std::setprecision(5)
                        # << "Wavelength = " << wavelength << "\n"
                        << "Detected Wavelength = " << detectedWavelength << "\n"
                        # << "Amplitude = " << amplitude << "\n"
                        << "Detected Amplitude = " << detectedAmplitude << '\n'
                        # << "Cycles = " << cycles << '\n'
                        << "Detected Cycles = " << detectedCycles << '\n'
                        # << "Phase Shift = " << phaseShift << '\n'
                        << "Detected Phase Shift = " << detectedPhaseShift << '\n'

                if(detectedAmplitude != NAN and detectedCycles != NAN and detectedWavelength != NAN and detectedPhaseShift != NAN){
                    std::vector<Point> optSine = analyzer.genOptSine(USER.lines->back().start, USER.lines->back().end, detectedWavelength, detectedAmplitude, detectedCycles, detectedPhaseShift)

                    Line sineLine(USER.lines->back().start, USER.lines->back().end, optSine)

                    ROOT.lines->push_back(sineLine)

                    updateLines()
                    createLines()
                }
                */


            }

        } ++halfSecondDelay

        # Sets vsync
        SDL_GL_SetSwapInterval(USE_VSYNC)

        # Limit the FPS
        previousTime = currentTime
        currentTime = SDL_GetTicks()

        #* if (currentTime - lastOutput >= 1000)
        #*    lastOutput = currentTime
        if (currentTime - previousTime < 1000 / MAX_FPS)
            SDL_Delay((1000 / MAX_FPS) - (currentTime - previousTime))

        # Clear the screen and show the background color
        glClearColor(arena.background.r, arena.background.g, arena.background.b, arena.background.a)
        glClear(GL_COLOR_BUFFER_BIT)

        # Update and syncronize the window width and height
        # int h, w
        # SDL_GetWindowSize(window, &h, &w)
        # height = h width = w g::windowHeight = height g::windowWidth = width

        # "Show what you need to shooow, show what you need to shoow..."
        # glViewport(0, 0, width, height)
        # glScissor(0, 0, width, height)

        if (not (paused or inOptionsMenu))
            draw(drawPoints)

        if (inOptionsMenu){
            if (nk_window_is_closed(ctx, "Options") and not firstDraw){
                _print(Menu closed!) _endl
                # nk_window_close(ctx, "Options")
                closeMenu(inOptionsMenu)
            }
            else{
                createOptionsMenu()
                firstDraw = false
            }
        }

        if (paused){
            if(drawPauseMenu(letItResume, drawOptionsInPause)){
                letItResume = false
                # nk_window_close(ctx, "Paused")
                nk_clear(ctx)
                closeMenu(paused)
            }
            else
                letItResume = true
        }

        if (inOptionsMenu or paused)
            nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY)

        # Swap the buffers so everything you just did is now being shown.
        SDL_GL_SwapWindow(window)
    }






'''
void Gui::drawOptionsMenu(){
        struct nk_colorf bg
        struct nk_colorf userColor
        # Fill the colors
        bg.r = this->arena.background.r bg.g = this->arena.background.g bg.b = this->arena.background.b bg.a = this->arena.background.a
        userColor.r  = USER.drawColor.r userColor.g  = USER.drawColor.g userColor.b  = USER.drawColor.b userColor.a  = USER.drawColor.a

        #* Slider
            # nk_layout_row_dynamic(ctx, 22, 1)
            # nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1)


        #* Mode Menu Options
            if (nk_tree_push(ctx, NK_TREE_TAB, "Mode", NK_MINIMIZED)){
                nk_layout_row_dynamic(ctx, 20, 3)
                if (nk_option_label(ctx, "Tutorial", mode == TUTORIAL)) mode = TUTORIAL
                if (nk_option_label(ctx, "Practice", mode == PRACTICE)) mode = PRACTICE
                if (nk_option_label(ctx, "Survival", mode == SURVIVAL)) mode = SURVIVAL
                if (nk_option_label(ctx, "Singleplayer", mode == SINGLEPLAYER)) mode = SINGLEPLAYER
                if (nk_option_label(ctx, "Multiplayer",  mode == MULTIPLAYER))  mode = MULTIPLAYER
                nk_tree_pop(ctx)
            }


        #* State Menu Options
            if (nk_tree_push(ctx, NK_TREE_TAB, "Mode Options", NK_MINIMIZED)){
                nk_layout_row_dynamic(ctx, 20, 1)
                nk_checkbox_label(ctx, "Analyze Lines"    , &analyze)
                nk_checkbox_label(ctx, "Show Bind Points ", &bindPointHelp)
                nk_checkbox_label(ctx, "Autocorrect"      , &autocorrect)
                nk_tree_pop(ctx)
            }


        #* Background Color Text
            nk_layout_row_dynamic(ctx, 20, 1)
            nk_label(ctx, "Background Color:", NK_TEXT_LEFT)

        #* Background Color Picker
            nk_layout_row_dynamic(ctx, 30, 1)
            if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))){
                nk_layout_row_dynamic(ctx, 120, 1)
                bg = nk_color_picker(ctx, bg, NK_RGBA)
                nk_layout_row_dynamic(ctx, 25, 1)
                bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f)
                bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f)
                bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f)
                bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f)
                nk_combo_end(ctx)
            }
            # Set bg back to the actual background
            this->arena.background.r = bg.r this->arena.background.g = bg.g this->arena.background.b = bg.b this->arena.background.a = bg.a


        #* User Draw Color Text
            nk_layout_row_dynamic(ctx, 20, 1)
            nk_label(ctx, "Draw Color:", NK_TEXT_LEFT)

        #* User Draw Color Picker
            nk_layout_row_dynamic(ctx, 30, 1)
            if (nk_combo_begin_color(ctx, nk_rgb_cf(userColor), nk_vec2(nk_widget_width(ctx),400))){
                nk_layout_row_dynamic(ctx, 120, 1)
                userColor = nk_color_picker(ctx, userColor, NK_RGBA)
                nk_layout_row_dynamic(ctx, 25, 1)
                userColor.r = nk_propertyf(ctx, "#R:", 0, userColor.r, 1.0f, 0.01f,0.005f)
                userColor.g = nk_propertyf(ctx, "#G:", 0, userColor.g, 1.0f, 0.01f,0.005f)
                userColor.b = nk_propertyf(ctx, "#B:", 0, userColor.b, 1.0f, 0.01f,0.005f)
                userColor.a = nk_propertyf(ctx, "#A:", 0, userColor.a, 1.0f, 0.01f,0.005f)
                nk_combo_end(ctx)
            }
            # Set userColor back to the actual users draw color
            USER.drawColor.r = userColor.r USER.drawColor.g = userColor.g USER.drawColor.b = userColor.b USER.drawColor.a = userColor.a

        #* Debug Menu Options
            if (nk_tree_push(ctx, NK_TREE_TAB, "Debug Options", NK_MAXIMIZED)){
                nk_layout_row_dynamic(ctx, 25, 1)
                # nk_layout_row_dynamic(ctx, 22, 1)
                # nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1)
                nk_tree_pop(ctx)
            }

        #* Debug Info
            if (nk_tree_push(ctx, NK_TREE_TAB, "Debug Info", NK_MINIMIZED)){
                nk_layout_row_dynamic(ctx, 20, 1)

                nk_tree_pop(ctx)
            }


int Gui::drawPauseMenu(bool &letItResume, bool& drawMenu){
    if (nk_begin(ctx, "Paused", nk_rect(0, 0, width, height), NK_WINDOW_NO_INPUT)){

        nk_layout_row_dynamic(ctx, 30, 1)

        nk_label(ctx, "PAUSED", NK_TEXT_CENTERED)


        nk_layout_row_dynamic(ctx, 40, 1)
        if (nk_button_label(ctx, "Resume") and letItResume){
            nk_end(ctx)
            return 1
        }

        nk_layout_row_dynamic(ctx, 30, 1)
        if (nk_button_label(ctx, "Exit to Desktop")){
            nk_end(ctx)
            cleanup(0, 0)
        }

        nk_layout_row_dynamic(ctx, 30, 1)
        if (nk_button_label(ctx, "Options")){
            drawMenu = !drawMenu
        }

        if (drawMenu)
            drawOptionsMenu()
    }
    nk_end(ctx)
    # return nk_window_is_closed(ctx, "Paused")
    return 0

'''