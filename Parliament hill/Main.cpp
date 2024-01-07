//Included Databases, note glew at the start due to it having to be activated first for all over GL liberies to function
#include <GL/glew.h>
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <iostream>
#include "header.h"
#include "ShadersHeader.h"
#include "FastNoiseLite.h"
//Defined Variables for PGC
#define RENDER_DISTANCE 124
#define MAP_SIZE RENDER_DISTANCE * RENDER_DISTANCE 
//Namespaces, mainly to make the code easier to read
using namespace std;
using namespace glm;
//Window Dimensions
int windowWidth = 1280;
int windowHeight = 720;
//Arrays such as VectorArrayObjects
enum VAO_IDs { Triangles, Indices, Colours, Textures, Vertices, NumVAOs = 3 };
GLuint VAOs[NumVAOs];
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
GLuint Buffers[NumBuffers];
//Variable Bank
bool firstMouse = true;
float lastpostX = 800.f / 2.0f;
float lastposty = 600.f / 2.0f;
const int RowNum = RENDER_DISTANCE - 1;
const int TPS = 2;
const int triGrid = RowNum * RowNum * TPS;
//Seeding Bank
FastNoiseLite TerrainNoise;
FastNoiseLite ColourNoise;
//Class calls
Player NewPlayer;
Camera NewCamera;
int main()
{
    StageRunning* NewStage = new StageRunning;

    NewStage->Seeding();

    glfwInit();
    //Create Window for the prototype
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Parliament Hill by Rhys Pascoe", NULL, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (window == NULL)
    {
        cout << "GLFW Window did not instantiate\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();
    //Establishes Shaders
    ShaderInfo shaders[] =
    {
        { GL_VERTEX_SHADER, "shaders/vertexShader.vert" },
        { GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
        { GL_NONE, NULL }
    };
    //Loads sahders and enables Depth as well as Establishing the mose
    program = LoadShaders(shaders);
    glUseProgram(program);
    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, Mouse);
    //Goes to the Terrain Creation Area
    NewStage->Arrays();
    //Establishes Model and Projection
    mat4 model = mat4(1.0f);
    model = scale(model, vec3(1.0f, 1.0f, 1.0f));
    model = rotate(model, radians(-5.0f), vec3(0.1f, 0.0f, 0.0f));
    model = translate(model, vec3(0.0f, 3.0f, 2.0f));
    mat4 projection = perspective(radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    int mvpLoc = glGetUniformLocation(program, "mvpIn");
    while (glfwWindowShouldClose(window) == false)
    {
        //Gets TIme
        NewPlayer.Times();

        UserInput(window);
        //Gets Colours
        NewPlayer.Colors();
        //Estbalihses View
        mat4 view = NewCamera.View();
        //Establishes MVP
        mat4 mvp = projection * view * model;
        //Renders MVP
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(mvp));
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, MAP_SIZE * 32, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
void StageRunning::Seeding() {
    //Generates the height of the terrian
    TerrainNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    TerrainNoise.SetFrequency(0.02f);
    int terrainSeed = 1;
    TerrainNoise.SetSeed(terrainSeed);
    //Generates the color of the terrain
    ColourNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    ColourNoise.SetFrequency(0.08f);
    int ColourSeed = 1;
    TerrainNoise.SetSeed(ColourSeed);
}
void StageRunning::Arrays() {
    //Vertices variable bank
    GLfloat tVertices[MAP_SIZE][6];
    float StartPosition = 1.0f; //start position for rendering
    float CVOffset = StartPosition; //Establishment of Collumn offset
    float RVOffset = StartPosition; //Establishment of Row offset
    int i = 0;
    int RowNumberInVoid = 0; //Row Number for within the void
    for (int y = 0; y < RENDER_DISTANCE; y++) {

        for (int x = 0; x < RENDER_DISTANCE; x++) {
            tVertices[i][1] = TerrainNoise.GetNoise((float)x, (float)y);//Applies Terrian noise to the map
            float colorValue = ColourNoise.GetNoise((float)x, (float)y);//Applies Colour noise to the map
            i++;
            if (colorValue <= -0.75) {// If the ColourValue in the seeding is less or more then 0.75 the the colour is green
                tVertices[i][3] = 0.40f;
                tVertices[i][4] = 0.85f;
                tVertices[i][5] = 0.45f;

            }
            else { //Else the colour is a dark Brown
                tVertices[i][3] = 0.40f;
                tVertices[i][4] = 0.00f;
                tVertices[i][5] = 0.00f;

            }
        }
    }

    for (int i = 0; i < MAP_SIZE; i++)
    {
        tVertices[i][0] = CVOffset;
        tVertices[i][2] = RVOffset;


        CVOffset = CVOffset + -0.0425f; // Increases offset

        RowNumberInVoid++;
        if (RowNumberInVoid == RENDER_DISTANCE) //Checks if the render distance has been reached
        {
            RowNumberInVoid = 0; // Resets the void row number for indices
            CVOffset = StartPosition; // Changes the offset for start posision
            RVOffset = RVOffset + -0.0425f; // Changes the rowoffset by  -0.0425f
        }
    }
    GLuint tIndices[triGrid][3]; //Establishes the indices for the terrian
    int CIoffset = 0; //Establish Colunm offset
    int RIOffset = 0;//Establish row offset
    for (int i = 0; i < triGrid - 1; i += 2)
    {
        //Establihses Indicies for all areas of the model
        tIndices[i][0] = CIoffset + RIOffset;
        tIndices[i][2] = RENDER_DISTANCE + CIoffset + RIOffset;
        tIndices[i][1] = 1 + CIoffset + RIOffset;

        tIndices[i + 1][0] = 1 + CIoffset + RIOffset;
        tIndices[i + 1][2] = RENDER_DISTANCE + CIoffset + RIOffset;
        tIndices[i + 1][1] = 1 + RENDER_DISTANCE + CIoffset + RIOffset;
        //Shifts Offset
        CIoffset = CIoffset + 1;
        //Adds the rows to the number of rows
        RowNumberInVoid++;
        //Checks if row is generated
        if (RowNumberInVoid == RowNum)
        {
            RowNumberInVoid = 0; // Establishes Row number as zero
            CIoffset = 0;//Resets CIoffset
            RIOffset = RIOffset + RENDER_DISTANCE; //Moves Y position
        }
    }

    //Sets the Indexes and establishes the buffers
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[0]);
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
    //Adds Vertices to Buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(tVertices), tVertices, GL_STATIC_DRAW);
    //Adds Indices to Buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tIndices), tIndices, GL_STATIC_DRAW);
    //Indexes the model and allows for it to be rendered
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Indexes the Textures and allows for the Textures to be rendered
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //UNbinds all
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void Mouse(GLFWwindow* window, double xposIn, double yposIn) {
   //Establishes X and Y posisions
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    //Sets where the first mouse location is located
    if (firstMouse)
    {
        lastpostX = xpos;
        lastposty = ypos;
        firstMouse = false;
    }
    //Establishes X and Y Offset
    float Xoffset = xpos - lastpostX;
    float Yoffset = lastposty - ypos;
    //Changes the last posision of the mouse
    lastpostX = xpos;
    lastposty = ypos;

    NewCamera.MouseMovement(Xoffset, Yoffset);
}
void UserInput(GLFWwindow* WindowIn)
{
    //Up movement
    if (glfwGetKey(WindowIn, GLFW_KEY_W) == GLFW_PRESS)
    {
        NewCamera.Movement(1);
    }
    //Down movement
    if (glfwGetKey(WindowIn, GLFW_KEY_S) == GLFW_PRESS)
    {
        NewCamera.Movement(2);

    }
    //Left movement
    if (glfwGetKey(WindowIn, GLFW_KEY_A) == GLFW_PRESS)
    {
        NewCamera.Movement(3);

    }
    //Right movement
    if (glfwGetKey(WindowIn, GLFW_KEY_D) == GLFW_PRESS)
    {
        NewCamera.Movement(4);
    }
    if (glfwGetKey(WindowIn, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(WindowIn, true);
    }
};