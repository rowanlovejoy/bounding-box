# 2020-COMP3016-Coursework2

## Bounding Box: 3D First-Person Platformer Video Game

![Bouding Box Gameplay Demo](https://media.giphy.com/media/be0mPMUo3Ghkcd9TH7/giphy.gif)

### Rowan Lovejoy (10573747)

--------------------------------------------------------------------------------------------------
## Pitch

### Description of Proposal

The proposed project is to produce a simple 3D first-person platformer video game, inspired by the game Jumping Flash! (Exact and Ultra, 1995). In the proposed game, the player’s objective is to control their avatar to jump between platforms in order to reach a goal point; if the player fails to land on a platform, they are sent back to the beginning of the game. As the player is falling, their shadow appears on platforms below them to indicate where they will land. The player views the game from a first-person perspective; there is no visible avatar model. The game features a moving player-controlled character in a 3D environment with textured models, and static environment lighting and a dynamic avatar shadow. Audio is not planned at this point. The intention is to create a short, fun gameplay experience with simple, colourful 3D graphics. The first-person perspective is intended to make platforming more challenging for players, and more exhilarating by enhancing their sense of speed and motion.

### Libraries and Resources
- Windowing and input library: GLFW (https://www.glfw.org/)
- Extension management library: Epoxy (https://github.com/anholt/libepoxy)
- GUI library: Dear ImGui (https://github.com/ocornut/imgui)
- 3D Asset loading library: Assimp (https://assimp.org/index.php)
- Maths library: GLM (https://glm.g-truc.net/0.9.9/index.html)
- GLSL language integration Visual Studio extension (https://marketplace.visualstudio.com/items?itemName=DanielScherzer.GLSL)

### User Interaction

The user interacts with the game by controlling the movement and other actions of the game avatar, including its movement direction and distance, and when and how many times it jumps and in what direction. They also interact with a main menu in order to start playing the game after launching the game application and also to exit the application. Players input commands into the game using a keyboard and mouse.

### References

Exact and Ultra (1995) Jumping Flash! [Video Game]. Sony Computer Entertainment.

--------------------------------------------------------------------------------------------------

## Video

**Link to video hosted on YouTube:** https://youtu.be/BaaZ5Q5ExaA

## Report

## Opening the Visual Studio Solution

To open the project's Visual Studio solution, open the '2020-comp3016-cw2-rowanlovejoy' directory and then open the 'Coursework2.sln' file. **PLEASE NOTE:** Afer building the solution, copy the file 'assimp-vc142-mtd.dll' and the folders 'media' and 'shaders' from the project root into the output folder alongside the compiled Coursework2.exe file (if building in Debug mode with x64, this executable should by default be output to /x64/Debug). Failing to do this will result in the application failing to launch after building with an error about the missing .dll file.

### User Interaction and Launching

Bounding Box is a 3D first-person platformer video game for Windows, created using C++, OpenGL 4.5 Core, GLFW, GLEW, GLM, Assimp, and stb_image.

The player controls the game using a keyboard and mouse. The W, A, S, and D keys move the player character left, right, forwards, and backwards on a 2D plane. The space bar triggers a jump, launching the player character upwards. The mouse controls the player view, which determines the forward and backwards direction. Using these controls in combination, the player can navigate the game and complete its platforming challenges. At any time, the player may press the Escape key to instantly quit the game.

The player can launch the game by double left-clicking on its executable in the File Explorer, or right-clicking on it and selecting ‘Open’ from the Context Menu. Gameplay begins immediately upon running the executable, the mouse cursor being captured by the game window. 

Ensure the folders 'media' and 'shaders' and the 'assimp-vc142-mtd.dll' file are in the same folder as the built 'Coursework2.exe' file. These folders and file can be found in the '2020-comp3016-cw2-rowanlovejoy' The application will not launch otherwise.

### Program Class and Function Layout

The program’s main() function initialises the windowing system, GLFW, creating a window for OpenGL to render into and binding functions to receive input and screen resizing callbacks. The main game logic is contained within the Game class, which is instantiated with a global ‘gameInstance’ variable. The Game class defines the components of the core game loop – input, update, and display – but the actual loop that calls these functions resides in main. The game uses fixed time-step for input and update calls, limited to sixty updates per second. This was done as part of efforts to fix an issue where collision was being checked too rapidly, which affected the player character’s jumping ability. Rendering occurs as fast the player’s hardware allows.

The Game class initialises the game state, loading the relevant models and compiling the shader program, and creating and configuring the in-game platforms and the projection matrix. In update(), moving objects, including the player character, consume the velocity accumulated on them, adding it to their positions and thus causing them to move. Gravity is then applied to the relevant objects – negative Y-axis velocity is continually added the player -- collisions are checked, and then the game over state is checked, the player being returned to the start of game if they have fallen off a platform.

Before discussing collision detection and rendering, a brief overview of the gameplay framework. The base class for in-game entities is GameObject, which defines properties including position, collision size, and velocity, and methods for changing the object’s position, either directly or by consuming accumulated velocity. It also includes a virtual method, draw(), which is defined at this level to enable polymorphic handling of game objects, a derived class of which, VisualObject, overrides this draw() to implement rendering an associated 3D model. The player character has no associated model, so is a GameObject; the platforms and the skybox are VisualObjects. Thanks to polymorphism, these visible entities can grouped together into a single vector of GameObject unique pointers, enabling a simple render method().

The game includes two utility classes, Model and Shader, which define a drawable, textured 3D model, and an OpenGL shader program, respectively. Model uses the Assimp library to load a 3D scene from a file and extract the contained 3D meshes, drawing indices, and associated materials, with stb_image used for loading texture data. The extracted data is stored in Mesh class instances, themselves contained in the model class. Model also handles some of the necessary OpenGL bindings, including generating and binding textures. The Mesh class handles the OpenGL buffer bindings – including VAOs, VBOs, EBOs and attribute pointers – for the 3D meshes.

The Shader class uses C++ standard input to read in the contents of shader files, including a vertex and a fragment shader. The contents of each file are then compiled and linked into a shader program. The class provides a method for activating the shader for use and setting uniform values of various types.

In the Game class’s init() method, both the Model and Shader classes are demonstrated, loading in 3D models which are then used to initialise the skybox and platforms, and creating a shader program which is then activated and used to set the projection matrix for use in rendering.

The final area we will look at is collision handling and jumping, as these are integral to each other and the game as a whole. During the update phase of the loop, the doCollisions() method is called last, enabling it to respond to any movement of objects to due to input or gravity. The game uses sphere to Axis Aligned Bounding Box collision. GameObjects, namely the platforms, have a defined position and size, which is used to calculate their AABB. The player character is defined as a sphere, with a position and radius. Collision is checked sequentially between the player character and other GameObjects in the scene. First, the centre of the player is calculated by adding its radius to its current position vector. Then the distance from the player character’s centre to the object’s, that distance clamped to the object’s half-extends (half height, width, an depth) to find the closest position on the object to the player character. If the length of the vector from the player character to this point is less than or equal to the player’s size – their position plus their radius – a collision is registered. The dot product between the direction vector and the six defined sides of the objects is then taken to find the closest side the player. This information is then returned to doCollisions(), which resolves the collision by pushing the player character the penetration distance away from the penetrated side object. Much of this algorithm was based on example provided by LearnOpenGL.com for 2D collision detection; I extended it to work in 3D by incorporating the Z-axis into calculations, and the results are surprisingly good, collision functioning well, with issues occurring only when approaching the very edge of objects – this is probably due to the treatment of objects as cubes.

The doCollisions() method is also responsible for determining if the player character is “grounded”, that is, they are standing on the ground. To be considered grounded, the player character must collide for two collision checks, which is tracked by a counter. This is to deal with an issue where the player would be unable to jump due the action of jumping setting them as not grounded, but them instantly colliding the ground again and thereby becoming grounded, leading to a jittery, non-functional jump. Enforcing this requirement of two collisions helps to resolve this issue by preventing instant switching between grounded and non-grounded. Various minimum collision amounts were tried, with lower values being more responsive but very occasionally causing small glitches. Two was found to be suitable in most cases.

Jumping is the most complex movement ability in the game, requiring multiple variables and if statements. Jumping is initiated by pressing the Space bar and will succeed if the player character is currently grounded and the jump key is not currently pressed. The player cannot send another jump command until they release the jump key. A jump is an action that persists over multiple frames. While jumping, a pulse of upward velocity – positive Y-axis – is added to the player each tick, becoming small each time. Combined with the constant downwards force gravity, this provides a smooth upward motion with believable loss of velocity nearing the top of the jump. The number of pulses, the starting velocity, the velocity decay rate, and the strength of gravity were repeatedly tuned to provide a realistic and satisfying “space jump” effect, fitting the game’s theme. This is perhaps the feature I’m most proud of.

## What Makes This Project Unique and Starting Point

3D first-person platforms are somewhat uncommon in the modern gaming landscape. While first-person games due feature platforming – for example Doom (2016) and Doom Eternal – this is not primary focus. Bounding Box echoes the gameplay style and feel of games including Jumping Flash  – it’s primary inspiration in terms of gameplay and physics feel – and Metroid Prime, both which feature a heavy emphasis on complex platforming in 3D world from a first-person perspective. I wanted to create the feel of leaping high into the air in a low gravity environment, the first-person perspective intensifying the effect of leaping forwards and falling. Compare to Jumping Flash and Metroid Prime, I don’t feel I entirely succeeded in emulating the “powerful” feel of the jumps, but with further adjustment and improvement to the physics system, I feel I could get there.

Development of the project had two starting points: the Coursework 2 project provided for the module, which provided functions and variables and a basic configuration for rendering 3D graphics with OpenGL; and a learning project I had been creating from following tutorials on LearnOpenGL.com, which included classes for loading models and shaders, real-time transforming 3D models, and a flying camera that could be controlled with mouse and keyboard input. I merged my LearnOpenGL project into the Coursework 2 project, making use of the initial code structure and Nuget packages of the latter.

From there, I established the basic gameplay framework – the Game and GameObject classes – using tutorial from LearnOpenGL.com, adding the ability associate GameObjects and 3D models in the VisualObject class, incorporating the collision detection algorithm from the example 2D Breakout clone from LearnOpenGL.com and expanding it into 3D dimensions. Once collision detection was functional, I updated it again to support the jumping system, which was added shortly afterwards.

Though much of the starting code was derived from online tutorials, my combination of it into a 3D game is original, incorporating information and examples from a variety of sources. The Game class and GameObject hierarchy were updated to enable polymorphic drawing and movement of objects by adding and consuming velocity. The jumping physics, the extension of the collision detection and resolution algorithm into 3D, and the sky cube are also original. Other original functionality includes the ability to scale and offset of the position of 3D models in the VisualObject class, which was necessary resolve mismatches between visible size and collision size. Throughout there also numerous small improvements to the code to improve clarity and maintainability and to make use of modern C++ programming practices where possible, such as consistent use of const and constexpr, type inference, pass by constant reference, and employing smart pointers to enable polymorphic behaviour with memory safety.

### Changes to Software Used

In the original brief, I stated I would use the Lib Expoxy library (link: https://github.com/anholt/libepoxy) for extension management. I chose this library after reading about its error reporting functionality to report issues, which I felt would ease development by helping me identify and diagnose issues sooner. However, I ran into difficulty setting it up, and decided that due to the starting Coursework 2 project already being configured to use GLEW, an alternative extension management library, I decided to drop Lib Epoxy and continue with GLEW instead. Given my understanding of OpenGL and skills at the time, and the time preasure, I believe this was a good choice to save myself the effort implementing something that was not entirely necessary. 

Another change was not encorporating Dear ImGUI to provide a graphical user interface. This was left until late in development due to difficulty implementing other parts of the project such as collision detection, and by that point there was no time remaining. The lack of the GUI does not majorly affect the final product, as it has no gameplay features that require such as an interface. 
