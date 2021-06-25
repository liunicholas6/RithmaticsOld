#include "Gui.hpp"
#include "Arena.hpp"
#include "Geometry.hpp"
#include "Globals.hpp"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <iomanip>

// move #include "stb_rect_pack.h" before #include "stb_truetype.h"

// Nuklear includes
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define STB_TRUETYPE_IMPLEMENTATION
// #define STB_RECT_PACK_VERSION
#define STB_RECT_PACK_IMPLEMENTATION
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "dep/nuklear.h"
#include "dep/nuklear_sdl_gl3.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <string>
#include <vector>

// this line is intelligent
#define GLSL(src) "#version 330 core\n" #src 

#define addLine(player, where)         \
    Line newLine(where);               \
    (*arena.players)[player].lines->push_back(newLine);

// #define finishLine(player, where, data) \
//     (*arena.players)[player].lines->back().finish(where, data);

#define finishLine(player, where, identify) \
    (*arena.players)[player].lines->back().finish(where, identify);

#define addData(player, where) \
    (*arena.players)[player].lines->back().append(where);

#define USER ((*arena.players)["user"])
#define ROOT ((*arena.players)["root"])
    

void Gui::printMouseLoc(){
    std::cout << "Mouse is at ";
    print(mouseLoc);
    std::cout << std::endl;
}

void Gui::setSDL_GLAttributes(){
    // tell openGL I want a forward compatible context of my version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // Setup SDL for OpenGL
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    // SDL_SetHint(SDL_HINT_RENDER_OPENGL_SHADERS, “1”);
}

void Gui::initGLEW(){
    //-----------------------------------------GLEW----------------------------------------//
    g::log("Setting up GLEW...");

    // GLEW -- enables specific features of certain graphics cards (not strictly necissary)
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        exit(1);
    }
}

void Gui::initLines(){
    glGenBuffers(1, &linesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, linesVbo);
}

void Gui::connectLines(){
    glBindBuffer(GL_ARRAY_BUFFER, linesVbo);
    glBindVertexArray(vao);
}

void Gui::createLines(){
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arena.vertices->size(), arena.vertices->data(), GL_DYNAMIC_DRAW);
    //! glBufferSubData() - implement this, at least to allow me to delete lines
}

void Gui::updateLines(){
    arena.update();
}

void Gui::initSDL(std::string title){
    g::log("Initializing SDL...");

    // initalize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING))
        std::cerr << "Can't initialize SDL2" << std::endl;

    setSDL_GLAttributes();

    // Make the SDL window
    window = SDL_CreateWindow(title.c_str(), windowPosition.x, windowPosition.y, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
     /* other options here:
        in place of x and y: SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED
        in place of SDL_WINDOW_OPENGL: SDL_WINDOW_RESIZABLE, SDL_WINDOW_FULLSCREEN (optional)
     */
    // check that it worked 
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    g::log("Creating OpenGL context...", 5);
    // Create the OpenGL context (finally)
    context = SDL_GL_CreateContext(window);
    // SDL_GL_MakeCurrent(window, context);
    
}

void Gui::initNuklear(){
    ctx = nk_sdl_init(window);

    // Load Fonts: if none of these are loaded a default font will be used
    // Load Cursor: if you uncomment cursor loading please hide the cursor
    
    nk_sdl_font_stash_begin(&atlas);
    // struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);
    // struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);
    // struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);
    // struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);
    // struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);
    // struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);
    nk_sdl_font_stash_end();
    // nk_style_load_all_cursors(ctx, atlas->cursors);
    // nk_style_set_font(ctx, &roboto->handle);

    // style.c
    // set_style(ctx, THEME_WHITE);
    // set_style(ctx, THEME_RED);
    // set_style(ctx, THEME_BLUE);
    // set_style(ctx, THEME_DARK);
}

void Gui::createVAO(){
    g::log("Creating VAO...", 5);
    // Create Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

GLuint Gui::createVertexShader(){
    g::log("Creating vertex shader...", 5);
    // Create and compile the vertex shader
    const char* vertexSource = GLSL(
        in vec2 position;
        in vec4 color;
        out vec4 Color;
        
        void main() {
            Color = color;
            gl_Position = vec4(position, 0.0, 1.0);
        }
    );
    
    g::log("compiling vertex shader...", 5);
    // compile it
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // check if that actually worked
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status){
        char buffer[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, buffer);
        std::cerr << buffer << std::endl;
    }
    return vertexShader;
}

GLuint Gui::createFragmentShader(){
    g::log("Creating fragment shader...", 5);
    // Create and compile the fragment shader
    const char* fragmentSource = GLSL(
        uniform vec4 drawColor;
        out vec4 outColor;
        in vec4 Color;
        
        void main() {
            // outColor = drawColor;
            outColor = Color;
        }
    );

    g::log("Compiling fragment shader...", 5);
    // compile it
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // now check if it worked
    /*
    glGetFragmentiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status){
        char buffer[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, buffer);
        std::cerr << buffer << std::endl;
    }
    */
    return fragmentShader;
}

void Gui::compileShaders(GLuint vertex, GLuint fragment){
    g::log("Linking shaders...", 5);
    // Link the vertex and fragment shader into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glBindFragDataLocation(shaderProgram, 0, "outColor"); // Specifies which framebuffer to use
    glLinkProgram(shaderProgram);                         //  (right now there's only one)
    glUseProgram(shaderProgram);
}

void Gui::arrangeLines(){
    // specify how the data is organized within the array you just made

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);//      normalize
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);//      normalize
    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    //                                                          stride                 offset
}

void Gui::updateMouse(int x, int y){
    mouseLoc.x = x;
    mouseLoc.y = y;
}

void Gui::getGlState(){
    #define x(get, name, pos, show) std::pair<std::pair<int, bool>,  \
                                    std::pair<int, std::string>>(    \
                                    std::pair<int, bool>(get, show), \
                                    std::pair<int, std::string>(pos, name)),


    //* Add gets here
    // Yes this is nasty, but it works, and I don't have to touch it ever again
    //   
    std::array<std::pair<std::pair<int, bool>, std::pair<int, std::string>>, 256> gets = {
        //   The info to get           What to call it    index show
        x(GL_ARRAY_BUFFER_BINDING,  "Current array buffer", 0, true )
        x(GL_CULL_FACE,             "Cull face",            0, false)
        x(GL_CURRENT_PROGRAM,       "Current program",      0, true )
    };

    #undef x

    for(auto i: gets){
        if(i.first.second){
            GLint p;
            if (not i.second.first)
                glGetIntegerv(i.first.first, &p);
            else
                glGetIntegeri_v(i.first.first, i.second.first, &p);

            _log(i.second.second + " = " + std::to_string(p));
        }
    }
}

void Gui::closeMenu(bool& inMenu){
    inMenu = false;
    connectLines();
    // initLines();
    // createVAO();
    updateLines();
    createLines();
    // glViewport(0, 0, width, height);
    glViewport(0, 0, SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h);
    glScissor(0, 0, width, height);
    glUseProgram(shaderProgram);
    // nk_clear(ctx);
    nk_input_end(ctx);
}

/* Draw everything in the vector of things we need to draw
void Gui::draw(bool points){
    // Returns a vector of tuples
    auto arr = arena.getArrangement();
    
    for (auto itP: arr)
        if(std::get<0>(itP).data() and std::get<1>(itP).data())
            //                type                     offset                   count                    num primitives
            glMultiDrawArrays(points ? GL_POINTS : GL_LINE_STRIP_ADJACENCY, std::get<0>(itP).data(), std::get<1>(itP).data(), std::get<2>(itP));
}
*/

void Gui::draw(bool points){
    std::vector<GLint>   offsets;
    std::vector<GLsizei> lengths;
    int playerSpecificOffset = 0;
    int count  = 0;
    int offset = 0;

    // Iterate through all the players
    for (auto p: *arena.players){
        // Iterate through all that players lines'
        for (auto i: *(p.second.lines)){
            assert(offsets.size() == lengths.size());
            if (not i.isNull()){
                lengths.push_back(i.getDataLen());
                offsets.push_back(offset);
                ++count;
            }
            // Add an extra offset if the line is erased
            offset += i.getDataLen();
        }
    }
    //                                     type                     offset          count           num primitives
    glMultiDrawArrays(points ? GL_POINTS : GL_LINE_STRIP_ADJACENCY, offsets.data(), lengths.data(), count);
}

void Gui::drawOptionsMenu(){
    
        struct nk_colorf bg;
        struct nk_colorf userColor;
        // Fill the colors 
        bg.r = this->arena.background.r; bg.g = this->arena.background.g; bg.b = this->arena.background.b; bg.a = this->arena.background.a;
        userColor.r  = USER.drawColor.r; userColor.g  = USER.drawColor.g; userColor.b  = USER.drawColor.b; userColor.a  = USER.drawColor.a;
        
        //* Slider
            // nk_layout_row_dynamic(ctx, 22, 1);
            // nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);


        //* Mode Menu Options
            if (nk_tree_push(ctx, NK_TREE_TAB, "Mode", NK_MINIMIZED)){
                nk_layout_row_dynamic(ctx, 20, 3);
                if (nk_option_label(ctx, "Tutorial", mode == TUTORIAL)) mode = TUTORIAL;
                if (nk_option_label(ctx, "Practice", mode == PRACTICE)) mode = PRACTICE;
                if (nk_option_label(ctx, "Survival", mode == SURVIVAL)) mode = SURVIVAL;
                if (nk_option_label(ctx, "Singleplayer", mode == SINGLEPLAYER)) mode = SINGLEPLAYER;
                if (nk_option_label(ctx, "Multiplayer",  mode == MULTIPLAYER))  mode = MULTIPLAYER;
                nk_tree_pop(ctx);
            }


        //* State Menu Options
            if (nk_tree_push(ctx, NK_TREE_TAB, "Mode Options", NK_MINIMIZED)){
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_checkbox_label(ctx, "Analyze Lines"    , &analyze);
                nk_checkbox_label(ctx, "Show Bind Points ", &bindPointHelp);
                nk_checkbox_label(ctx, "Autocorrect"      , &autocorrect);
                nk_tree_pop(ctx);
            }


        //* Background Color Text
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "Background Color:", NK_TEXT_LEFT);

        //* Background Color Picker
            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))){
                nk_layout_row_dynamic(ctx, 120, 1);
                bg = nk_color_picker(ctx, bg, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
                bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
                bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
                bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
                nk_combo_end(ctx);
            }
            // Set bg back to the actual background
            this->arena.background.r = bg.r; this->arena.background.g = bg.g; this->arena.background.b = bg.b; this->arena.background.a = bg.a;


        //* User Draw Color Text
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "Draw Color:", NK_TEXT_LEFT);

        //* User Draw Color Picker
            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_combo_begin_color(ctx, nk_rgb_cf(userColor), nk_vec2(nk_widget_width(ctx),400))){
                nk_layout_row_dynamic(ctx, 120, 1);
                userColor = nk_color_picker(ctx, userColor, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                userColor.r = nk_propertyf(ctx, "#R:", 0, userColor.r, 1.0f, 0.01f,0.005f);
                userColor.g = nk_propertyf(ctx, "#G:", 0, userColor.g, 1.0f, 0.01f,0.005f);
                userColor.b = nk_propertyf(ctx, "#B:", 0, userColor.b, 1.0f, 0.01f,0.005f);
                userColor.a = nk_propertyf(ctx, "#A:", 0, userColor.a, 1.0f, 0.01f,0.005f);
                nk_combo_end(ctx);
            }
            // Set userColor back to the actual user's draw color
            USER.drawColor.r = userColor.r; USER.drawColor.g = userColor.g; USER.drawColor.b = userColor.b; USER.drawColor.a = userColor.a;

        //* Debug Menu Options
            if (nk_tree_push(ctx, NK_TREE_TAB, "Debug Options", NK_MAXIMIZED)){
                nk_layout_row_dynamic(ctx, 25, 1);
                // nk_layout_row_dynamic(ctx, 22, 1);
                // nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
                nk_tree_pop(ctx);
            }

        //* Debug Info
            if (nk_tree_push(ctx, NK_TREE_TAB, "Debug Info", NK_MINIMIZED)){
                nk_layout_row_dynamic(ctx, 20, 1);

                nk_tree_pop(ctx);
            }
    
    
}

void Gui::createOptionsMenu(){
    if (nk_begin(ctx, "Options", nk_rect(10, 10, width - 20, height - 20), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE)){
        drawOptionsMenu();    
    }
    nk_end(ctx);
}

int Gui::drawPauseMenu(bool &letItResume, bool& drawMenu){
    if (nk_begin(ctx, "Paused", nk_rect(0, 0, width, height), NK_WINDOW_NO_INPUT)){
        
        nk_layout_row_dynamic(ctx, 30, 1);

        nk_label(ctx, "PAUSED", NK_TEXT_CENTERED);
        

        nk_layout_row_dynamic(ctx, 40, 1);
        if (nk_button_label(ctx, "Resume") and letItResume){
            nk_end(ctx);
            return 1;
        }

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Exit to Desktop")){
            nk_end(ctx);
            cleanup(0, 0);
        }

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Options")){
            drawMenu = !drawMenu;
        }

        if (drawMenu)
            drawOptionsMenu();
    }
    nk_end(ctx);
    // return nk_window_is_closed(ctx, "Paused");
    return 0;
}

void Gui::addManualLines(Player player){
    Geometry creator;
    Point center(243, 150);
    Point origin(0, 0);
    Point opposite(width, height);
 
    //* Make a Straight Line
    Point randEnd(125, 185);
    auto straightData = creator.genOptLine(center, randEnd);
    Line optStraight(center, randEnd, straightData);

    //* Make a Circle
    int radius = 50;
    auto circleData = creator.genOptCircle(center, radius, true);
    Line optCircle(Point(center.x + radius, center.y), Point(center.x + radius, center.y), circleData);

    //* Make a Sine Wave
    double amplitude  = 20.;
    double wavelength = 40.;
    double cycles     = 4. ;
    double phaseShift = 7. ;
    Point someStart(300, 100);
    // Point someEnding(center.x + 50, center.y);
    // Point someEnding(375, center.y + 20);
    Point someEnding(someStart.x + 115, someStart.y + 115);
    // auto  sineData = creator.genOptSine(center, someEnding, wavelength, amplitude, cycles, phaseShift, false);
    // Line  optSine(center, someEnding, sineData);

    //* Analyze the straight line
    // optStraight.identifyLine();
    // optStraight.printAccDebug();

    //* Analyze the cirlce
    // optCircle.identifyLine();
    // optCircle.printAccDebug();

    //* Analyze the sine wave
    // double detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift;
    // Geometry analyzer(&optSine);
    // analyzer.getSineData(detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift);

    // analyzer.getSineData(detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift);
    // std::cout << g::getDebugCount() << ": " << std::fixed << std::setprecision(5)
    //           << "Wavelength = " << wavelength << "\n"
    //           << "Detected Wavelength = " << detectedWavelength << "\n"
    //           << "Amplitude = " << amplitude << "\n"
    //           << "Detected Amplitude = " << detectedAmplitude << '\n'
    //           << "Cycles = " << cycles << '\n'
    //           << "Detected Cycles = " << detectedCycles << '\n'
    //           << "Phase Shift = " << phaseShift << '\n'
    //           << "Detected Phase Shift = " << detectedPhaseShift << '\n';

    // if(detectedAmplitude == NAN or detectedCycles == NAN or detectedWavelength == NAN or detectedPhaseShift == NAN)
    //     return;

    // std::vector<Point> optSine = genOptSine(line->start, line->end, detectedWavelength, detectedAmplitude, detectedCycles, detectedPhaseShift);
    
    // optSine.identifyLine();
    // optSine.printAccDebug();

    //* Add the lines
    // player.lines->push_back(optStraight);
    // player.lines->push_back(optCircle);
    // player.lines->push_back(optSine);

    // Update the lines
    updateLines();
    createLines();
}

void Gui::run(){
    g::log("Starting window proper...");

    Geometry creator;
    Point someStart(280, 120);

    SDL_Event event;
    bool fullscreen = false, run = true, trackMouse = false, drawPoints = true;
    bool paused = false, inOptionsMenu = false, drawOptionsInPause = false; // don't touch the last one
    bool firstDraw = true, letItResume = true; // If you only just opened the menu
    bool identify = true, ran = false;
    bool shift = false, ctrl = false, alt = false;
    Uint32 windowFlags = 0; // fudge variable
    unsigned int previousTime, currentTime, lastOutput = 0;
    unsigned int halfSecondDelay = 1;
    (*arena.players)["root"] = Player("root", Color(255, 0, 0));
    
    addManualLines(ROOT);

    while (run){
        // Update the mouse location - now done as needed
        // SDL_GetMouseState(&mouseLoc.x, &mouseLoc.y);

        // The event loop
        if (paused or inOptionsMenu)
            nk_input_begin(ctx);
        while (SDL_PollEvent(&event)){
            if (paused or inOptionsMenu)
                nk_sdl_handle_event(&event);

            switch(event.type){
                case SDL_QUIT:
                    run = false; break;
                case SDL_KEYDOWN:
                    // logVal(SDL_GetScancodeName(event.key.keysym.scancode)) // physical key
                    // logVal(SDL_GetKeyName(event.key.keysym.sym))           // simulated key
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            if (inOptionsMenu)
                                closeMenu(inOptionsMenu);
                            else if (paused)
                                closeMenu(paused);
                            else
                                run = false;
                            break;
                        case 'F':
                        case 'f':
                            fullscreen = !fullscreen;
                            if (fullscreen)
                                SDL_SetWindowFullscreen(window, windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                            else
                                SDL_SetWindowFullscreen(window, windowFlags);
                            break;
                        case 'c':
                            arena.clear();
                            trackMouse = false;
                            ran = false; //! This is only here for debugging
                            break;
                        case SDLK_RCTRL:
                        case SDLK_LCTRL:
                            ctrl = true;
                            break;
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            shift = true;
                            break;
                        case SDLK_RALT:
                        case SDLK_LALT:
                            alt = true;
                            break;
                        case 'z':
                            if(ctrl){
                                _log("Ctrl + z pressed");
                                USER.lines->back().erase();
                            }
                            break;
                        case SDLK_TAB:
                            drawPoints = !drawPoints; break;
                        case 'M':
                        case 'm':
                            if (inOptionsMenu)
                                closeMenu(inOptionsMenu);
                            else{
                                inOptionsMenu = true;
                                firstDraw = true;
                            }
                            break;
                        case 'P':
                        case 'p':
                            if (paused)
                                closeMenu(paused);
                            else{
                                paused = true;
                                // letItResume = true;
                            }
                            break;
                        case 'D':
                        case 'd':
                            // if (shift)
                            // Run the "Run once" code again
                                ran = false; break;
                        // case '':

                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym){
                        case SDLK_RCTRL:
                        case SDLK_LCTRL:
                            ctrl = false;
                            break;
                        case SDLK_LSHIFT:
                        case SDLK_RSHIFT:
                            shift = false;
                            break;
                        case SDLK_RALT:
                        case SDLK_LALT:
                            alt = false;
                            break;
                        }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    updateMouse(event.button.x, event.button.y);
                    if (event.button.button == SDL_BUTTON_LEFT and not (paused or inOptionsMenu)) {
                        g::log("Mouse pressed", 5);
                        // lastOutput = SDL_GetTicks();

                        if (not trackMouse){
                            trackMouse = true;

                            addLine("user", mouseLoc);

                            updateLines();
                        }
                        break;
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT and not (paused or inOptionsMenu)){
                        break;
                    }
                    else if (event.button.button == SDL_BUTTON_MIDDLE){
                        _log("The mouse is at " + std::to_string(event.button.x) + ", " + std::to_string(event.button.y));
                        break;
                    }
                    break;
                // When mouse leaves the window or the window is resized
                case 512:
                    if (not trackMouse)
                        break;
                    identify = false;
                case SDL_MOUSEBUTTONUP:
                    updateMouse(event.button.x, event.button.y);
                    if ((event.button.button == SDL_BUTTON_LEFT or event.type == 512) and not (paused or inOptionsMenu)){
                        g::log("Mouse released", 5);
                        if (trackMouse){
                            // if(USER.lines->back().lineData->size()){
                            finishLine("user", mouseLoc, identify);
                            identify = true;

                            // assert(trackMouse);
                            trackMouse = false;
                            
                            updateLines();
                            createLines();
                        }
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT and not (paused or inOptionsMenu)){
                        break;
                    }
                    else if (event.button.button == SDL_BUTTON_MIDDLE and not (paused or inOptionsMenu)){
                        break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    updateMouse(event.button.x, event.button.y);
                    if (trackMouse and not (paused or inOptionsMenu)){
                        
                        addData("user", mouseLoc);

                        updateLines();
                        createLines();

                        g::log("Mouse is moving, and I'm tracking it.", 5);
                        
                        // lastOutput = SDL_GetTicks();
                    /*
                        // Behold my superb debugging skillz
                        logVal(arena.vertices->size() / 6);
                        logVal((*arena.players)["user"].lines->size());
                        // for (auto i: *(*arena.players)["user"].lines){
                        for (auto i = (*arena.players)["user"].lines->begin(); i != (*arena.players)["user"].lines->end(); ++i){
                            g::log("--- " + std::to_string(i - (*arena.players)["user"].lines->begin()) + " ---");
                            logVal(i->lineData->size());
                            logVal(i->vertices->size() / 6);
                            logVal(i->isFinished);
                        }
                        _endl; g::log("--------------");
                    */
                    }
                    break;
                case SDLK_PRINTSCREEN:
                    break;
                default:
                    // std::cout << "Unknown event type: " << event.type << std::endl;
                    break;
            }
        }
        if (paused or inOptionsMenu)
            nk_input_end(ctx);


        //* Runs Once
        if (not ran){
            if (USER.lines->size() and USER.lines->back().isFinished){
                // _print(rotating...) _endl
                // creator.rotateStraight(USER.lines->back());
                ROOT.lines->push_back(Line());
                if (ROOT.lines->size() < 2){
                    _print(creating a new line...) _endl
                    ROOT.lines->back() = USER.lines->back();
                }
                else
                    ROOT.lines->back() = *(ROOT.lines->end() - 1);
                

                // creator.rotateStraight(ROOT.lines->back());
                // creator.rotate(ROOT.lines->back(), 30);

                updateLines();
                createLines();
                ran = true;
            }
        }

        if(not (halfSecondDelay % (MAX_FPS / 2))){
            // logVal(ran)
            // getGlState();
            // logVal(nk_window_is_closed(ctx, "Options"))
            // logVal(nk_window_is_closed(ctx, "Paused"))

            // double amplitude  = 20.;
            // double wavelength = 40.;
            // double cycles     = 4. ;
            
            // ++someStart.x;
            // ++someStart.y;

            // // Point someEnding(center.x, center.y + 50);
            // // Point someEnding(375, center.y + 20);
            // Point someEnding(someStart.x + 115, someStart.y + 115);
            // auto  sineData = creator.genOptSine(someStart, someEnding, wavelength, amplitude, cycles, true);
            // Line  optSine(someStart, someEnding, sineData);
            // // ROOT.lines->back(). = optSine; // sineData;
            // // ROOT.lines->pop_back();
            // ROOT.lines->push_back(optSine);            

            if (USER.lines->size() and USER.lines->back().isFinished){
                // Line* l = new Line(USER.lines->back());
                // Geometry geo(USER.lines->back());
                // USER.lines->back().start.print("Start from user");
                // USER.lines->back().end.print("End from user");

                //* I DO NOT KNOW HOW THIS IS POSSIBLE

                //* Line tmpLine = USER.lines->back();
                // std::cout << &USER.lines->back(); _endl

                // USER.lines->end()->end.print("Gui");
                // tmpLine.end.print("tmpLine");
                
                //* tmpLine.lineData->push_back(USER.lines->back().end);
                
                //* tmpLine.end = USER.lines->back().end;

                // USER.lines->back().print("Gui");
                // tmpLine.print("tmpLine");

                // USER.lines->back().lineData->push_back(USER.lines->back().end);
                //* std::vector<Point> tmp = creator.getAmplitude1(tmpLine);
                // USER.lines->back().print("Gui2");
                //* ROOT.lines->push_back(Line(tmp.front(), tmp.back(), tmp));
                // tmp.front().print("Start");
                // tmp.back().print("End");

                // Geometry analyzer(&USER.lines->back());

                /*
                double detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift;

                

                analyzer.getSineData(detectedAmplitude, detectedWavelength, detectedCycles, detectedPhaseShift);

                std::cout << g::getDebugCount() << ": " << std::fixed << std::setprecision(5)
                        // << "Wavelength = " << wavelength << "\n"
                        << "Detected Wavelength = " << detectedWavelength << "\n"
                        // << "Amplitude = " << amplitude << "\n"
                        << "Detected Amplitude = " << detectedAmplitude << '\n'
                        // << "Cycles = " << cycles << '\n'
                        << "Detected Cycles = " << detectedCycles << '\n'
                        // << "Phase Shift = " << phaseShift << '\n'
                        << "Detected Phase Shift = " << detectedPhaseShift << '\n';

                if(detectedAmplitude != NAN and detectedCycles != NAN and detectedWavelength != NAN and detectedPhaseShift != NAN){
                    std::vector<Point> optSine = analyzer.genOptSine(USER.lines->back().start, USER.lines->back().end, detectedWavelength, detectedAmplitude, detectedCycles, detectedPhaseShift);

                    Line sineLine(USER.lines->back().start, USER.lines->back().end, optSine);

                    ROOT.lines->push_back(sineLine);

                    updateLines();
                    createLines();
                }
                */

                
            }

        } ++halfSecondDelay;

        // Sets vsync
        SDL_GL_SetSwapInterval(USE_VSYNC);

        // Limit the FPS
        previousTime = currentTime;
        currentTime = SDL_GetTicks();

        //* if (currentTime - lastOutput >= 1000)
        //*    lastOutput = currentTime;
        if (currentTime - previousTime < 1000 / MAX_FPS)
            SDL_Delay((1000 / MAX_FPS) - (currentTime - previousTime));

        // Clear the screen and show the background color
        glClearColor(arena.background.r, arena.background.g, arena.background.b, arena.background.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update and syncronize the window width and height
        // int h, w;
        // SDL_GetWindowSize(window, &h, &w);
        // height = h; width = w; g::windowHeight = height; g::windowWidth = width;

        // "Show what you need to shooow, show what you need to shoow..."
        // glViewport(0, 0, width, height);
        // glScissor(0, 0, width, height);

        if (not (paused or inOptionsMenu))
            draw(drawPoints);

        if (inOptionsMenu){
            if (nk_window_is_closed(ctx, "Options") and not firstDraw){
                _print(Menu closed!) _endl
                // nk_window_close(ctx, "Options");
                closeMenu(inOptionsMenu);
            }
            else{
                createOptionsMenu();
                firstDraw = false;
            }
        }

        if (paused){
            if(drawPauseMenu(letItResume, drawOptionsInPause)){
                letItResume = false;
                // nk_window_close(ctx, "Paused");
                nk_clear(ctx);
                closeMenu(paused);
            }
            else 
                letItResume = true;
        }
        
        if (inOptionsMenu or paused)
            nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

        // Swap the buffers so everything you just did is now being shown.
        SDL_GL_SwapWindow(window);
    }
}

void Gui::cleanup(GLuint vertexShader, GLuint fragmentShader){
    g::log("Cleaning Up...");

    // delete everything and clean up
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    // for(auto& i: stuffToDraw)
        // i->deleteVBOs();
    // stuffToDraw[0]->deleteVBOs();
    // firstLine.deleteVBOs();
    // glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
}

void Gui::init(std::string title){
    g::windowWidth = width; 
    g::windowHeight = height;

    // drawColor = (*arena.players)["user"].drawColor;

    initSDL(title);
    initGLEW();
    initNuklear();
    initLines();
    // createLines();
    createVAO();
    GLuint vertexShader   = createVertexShader();
    GLuint fragmentShader = createFragmentShader();
    compileShaders(vertexShader, fragmentShader);

    colorLoc = glGetUniformLocation(shaderProgram, "drawColor");
    glUniform4f(colorLoc, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

    arrangeLines();
    run();
    cleanup(vertexShader, fragmentShader);
}

Gui::~Gui() {}