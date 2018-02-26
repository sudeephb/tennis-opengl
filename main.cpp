// model -> bat1 , model2 ->bat2

#include <string>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 1440, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;
float pos1X = 0.0f,pos1Y = -0.06f,pos1Z = 2.8f;
float pos2X = 0.0f,pos2Y = 0.06f,pos2Z = 2.6f;


float ballPosX = 0.0f;
float ballPosY = -0.05f;
float ballPosZ = 2.7f;

bool movebal = false;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
bool inBetween(float p,float a, float b);


void moveBall(float vx,float vy, float vz);

// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;





int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    //  glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Tennis", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    
    
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    // glfwSetCursorPosCallback( window, MouseCallback );
    
    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    
    
    
    //<ground>
    
    // Setup and compile our ground shaders
    Shader groundShader( "resources/shaders/ground.vs", "resources/shaders/ground.frag" );
 //   Shader netShader( "resources/shaders/net", "resources/shaders/net.frag" );
    
    GLfloat groundVertices[] =
    {
        // Positions                  // Texture Coords
        0.5f,  0.5f, 0.0f,            1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,            1.0f, 0.0f, // Bottom Right
        -0.5f,  0.5f, 0.0f,            0.0f, 1.0f,  // Top Left
        0.5f, -0.5f, 0.0f,             1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,            0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,            0.0f, 1.0f  // Top Left
        
    };
    
    GLfloat netVertices[] =
    {
        // Positions                  // Texture Coords
        0.5f,  0.5f, 0.0f,            1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,            1.0f, 0.0f, // Bottom Right
        -0.5f,  0.5f, 0.0f,            0.0f, 1.0f,  // Top Left
        0.5f, -0.5f, 0.0f,             1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,            0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,            0.0f, 1.0f  // Top Left
    };
    
    // Setup ground VAO
    GLuint groundVAO, groundVBO;
    glGenVertexArrays( 1, &groundVAO );
    glGenBuffers( 1, &groundVBO );
    glBindVertexArray( groundVAO );
    glBindBuffer( GL_ARRAY_BUFFER, groundVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( groundVertices ), &groundVertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glBindVertexArray(0);
    
    
    
    // Load textures
    GLuint groundTexture = TextureLoading::LoadTexture( "resources/images/court_top_view1.jpg" );
    GLuint netTexture = TextureLoading::LoadTexture("resources/images/net.png");
    
    glm::mat4 groundmodel;
    groundmodel = glm::rotate(groundmodel, glm::radians(-71.0f), glm::vec3(1.0f, 0.0f, 0.0f))*glm::rotate(groundmodel, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::mat4 groundview;
    groundview = glm::translate(groundview, glm::vec3(0.0f, 0.0f, -0.868f));
    
    glm::mat4 groundprojection;
    groundprojection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT , 0.1f, 100.0f);
    
    //</ground>
    
    
    
    
    
    
    
    
    // Setup and compile our bat shaders
    Shader shader( "resources/shaders/modelLoading.vs", "resources/shaders/modelLoading.frag" );
    
    // Load models
    Model bat( "resources/models/sujitBat/racket.obj" );
    Model ball( "resources/models/pgoball/PokemonGoBall.obj" );
    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    
    //  glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    glm::mat4 projection = glm::perspective( glm::radians(45.0f), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    
    
    // vel of ball
    float vy = 0.0006f;
    float vx = 0.0005f;
    float vz = 0.0009f;
    
    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        // DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 0.2f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        
        
        
        // <ground>
        
        // Draw our ground
        groundShader.Use( );
        
        // Bind Textures using texture units
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, groundTexture );
        glUniform1i( glGetUniformLocation( shader.Program, "texture1" ), 0 );
        
        // Get the uniform locations
        GLint groundmodelLoc = glGetUniformLocation( groundShader.Program, "groundmodel" );
        GLint groundviewLoc = glGetUniformLocation( groundShader.Program, "groundview" );
        GLint groundprojLoc = glGetUniformLocation( groundShader.Program, "groundprojection" );
        
        // Pass the matrices to the shader
        glUniformMatrix4fv( groundviewLoc, 1, GL_FALSE, glm::value_ptr( groundview ) );
        glUniformMatrix4fv( groundprojLoc, 1, GL_FALSE, glm::value_ptr( groundprojection ) );
        
        glBindVertexArray( groundVAO );
        
        // Calculate the model matrix for each object and pass it to shader before drawing
        glUniformMatrix4fv( groundmodelLoc, 1, GL_FALSE, glm::value_ptr( groundmodel ) );
        
        glDrawArrays( GL_TRIANGLES, 0, 6 );
        glBindVertexArray( 0 );
        
        // </ground>
        
        
        
        
        
        // <bat1 model>
        
        shader.Use( );
        
        glm::mat4 view = camera.GetViewMatrix();
        
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        
        // Draw the loaded model
        glm::mat4 model;
        double xpos = 0, ypos = 0;
        glfwGetCursorPos(window,&xpos, &ypos);
        
        //move bat1
        pos1X = (xpos/WIDTH) * 2.0f - 1.0f;

        if(pos1X < -0.22f) pos1X = -0.20f;
        if(pos1X > 0.22f) pos1X = 0.20f;

       // pos1Y = -((ypos/HEIGHT) * 2.0f - 1.0f);
      //  if(pos1Y < -1.0f) pos1Y = -1.0f;
        
        model = glm::translate(glm::mat4(), glm::vec3( pos1X, pos1Y, pos1Z ) ); // Translate it down a bit so it's at the center of the scene
        
        model = glm::scale( model, glm::vec3( 0.002f, 0.002f, 0.002f ) );    // It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model));
        bat.Draw( shader );
        
        // </bat1 model>
        
        
        
        // <bat2 model>
        
        glm::mat4 model2;
        model2 = glm::translate( model2, glm::vec3( pos2X, pos2Y, pos2Z ) ); // Translate it down a bit so it's at the center of the scene
        
        model2 = glm::scale( model2, glm::vec3( 0.002f, 0.002f, 0.002f ) );    // It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model2 ) );
        bat.Draw( shader );
        
        
        // </bat2 model>
        
        
        
        
        // <ball model>
        
        
        
        
        glm::mat4 ballModel;
        ballModel = glm::translate( ballModel, glm::vec3( ballPosX, ballPosY, ballPosZ ) );
        ballModel = glm::scale( ballModel, glm::vec3( 0.00005f, 0.00005f, 0.00005f ) );    // It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( ballModel ) );
        ball.Draw( shader );
        
        
        
        
        //move ball
       /*
        
        if(movebal == true)
            moveBall(vx, vy, vz);
        
        if ((inBetween( ballPosX, pos1X - 0.02, pos1X + 0.02) && vy < 0) && ballPosY == -0.1)  vy = -vy;
        
      //  if(ballPosY > 0.04438f || ballPosY < pos1Y )    vy = -vy;
   
        pos2X = ballPosX;
        if(ballPosY > 0.04438f)
        {
            if(ballPosX < pos2X - 0.013 || ballPosX > pos2X + 0.013)  break;
                else
                vy = -vy;
        }
        
        if(ballPosY < -0.11f)
        {
            if(ballPosX < pos1X - 0.02 || ballPosX > pos1X + 0.02)  break;
                else
                vy = -vy;
        }
    */
        
       // if( ballPosX > ((8.0/5.0)*(ballPosY + 0.04)+0.21) )    vx = -vx;
        //if(ballPosX > 0.2f || ballPosX < -0.2f )    vx = -vx;
        
     
          
        
        
        
        // </ball model>
        
        
        
        
        
        // Swap the buffers
        glfwSwapBuffers( window );
        
    }
    
    
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
    
        
        glfwSwapBuffers( window );
    }
    
    
    std::cout << "x = " << ballPosX << std::endl << "y = " << ballPosY << std::endl << "Z = " << ballPosZ << std::endl ;
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( GLFW_KEY_UP == key && GLFW_PRESS == action )   ballPosY += 0.01;
    if ( GLFW_KEY_DOWN == key && GLFW_PRESS == action )   ballPosY -= 0.01;
    
    if ( GLFW_KEY_RIGHT == key && GLFW_PRESS == action )   ballPosX += 0.01;
    if ( GLFW_KEY_LEFT == key && GLFW_PRESS == action )   ballPosX -= 0.01;
    
    if ( GLFW_KEY_W == key && GLFW_PRESS == action )   pos1Z += 0.2;
    if ( GLFW_KEY_S == key && GLFW_PRESS == action )   pos1Z -= 0.2;
    
    
    
    
    
    
    
    
    
    /*
     if ( key >= 0 && key < 1024 )
     {
     if ( action == GLFW_PRESS )
     {
     keys[key] = true;
     }
     else if ( action == GLFW_RELEASE )
     {
     keys[key] = false;
     }
     }
     */
    
    
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = yPos - lastY;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}




void moveBall(float vx,float vy, float vz)
{
 
    ballPosY += vy;
    ballPosX += vx;
    //  ballPosZ -= vz;
    //  vy -= 0.0000009 /*deltaTime*/;
  
  
    
}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        movebal = true;
}

bool inBetween(float p, float a, float b)
{
    if( p < a || p > b) return false;
    else
        return true;
}






