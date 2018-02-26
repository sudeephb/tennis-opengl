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
#include "Entity.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 1440, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;



// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
glm::mat4 createTransformationMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);



// Camera
bool keys[1024];



Entity* playerBat;
Entity* opponentBat;
Entity* ball;


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
    
    
//    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    
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

    
    // Setup and compile our bat shaders
    Shader groundShader( "resources/shaders/ground.vs", "resources/shaders/ground.frag" );
    Shader shader( "resources/shaders/modelLoading.vs", "resources/shaders/modelLoading.frag" );

    
    playerBat = new Entity(glm::vec3(0.0f, -0.15f, -0.5f), glm::vec3(0.0f), glm::vec3(0.01f),"resources/models/sujitBat/racket.obj");
    opponentBat = new Entity(glm::vec3(0.0f, 0.3f, -2.0f), glm::vec3(0.0f), glm::vec3(0.01f),"resources/models/sujitBat/racket.obj");
    ball = new Entity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.002),"resources/models/pgoball/PokemonGoBall.obj");
    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glClearColor( 0.2f, 1.0f, 1.0f, 1.0f );
    
    //  glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    glm::mat4 projection = glm::perspective( glm::radians(45.0f), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    glm::mat4 groundmodel, groundview;
    
    groundShader.Use();
    groundmodel = glm::rotate(groundmodel, glm::radians(-71.0f), glm::vec3(1.0f, 0.0f, 0.0f))*glm::rotate(groundmodel, glm::radians(90.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    groundview = glm::translate(groundview, glm::vec3(0.0f, 0.0f, -1.0f));
    
    GLint groundprojLoc = glGetUniformLocation( groundShader.Program, "groundprojection" );
    GLint groundmodelLoc = glGetUniformLocation( groundShader.Program, "groundmodel" );
    GLint groundviewLoc = glGetUniformLocation( groundShader.Program, "groundview" );
    
    glUniformMatrix4fv( groundprojLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv( groundviewLoc, 1, GL_FALSE, glm::value_ptr(groundview));
    
    groundShader.unUse();
    
    shader.Use();
    GLint modelProjectionLoc = glGetUniformLocation( shader.Program, "projection" );
    GLint modelTransformationLoc = glGetUniformLocation( shader.Program, "transformation" );
    GLint modelViewLoc = glGetUniformLocation( shader.Program, "view" );
    glUniformMatrix4fv(modelProjectionLoc,1, GL_FALSE, glm::value_ptr(projection));
    shader.unUse();
    
    
    while( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents( );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
    
        groundShader.Use( );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, groundTexture );
        glUniform1i( glGetUniformLocation( shader.Program, "texture1" ), 0 );
        glBindVertexArray( groundVAO );
        glUniformMatrix4fv( groundmodelLoc, 1, GL_FALSE, glm::value_ptr( groundmodel ) );
        
        glDrawArrays( GL_TRIANGLES, 0, 6 );
        glBindVertexArray( 0 );
        groundShader.unUse();
        //ball->draw(shader);
        
        shader.Use();
        
        glm::mat4 transformationMatrix = createTransformationMatrix(playerBat->getPosition(), playerBat->getRotation(), playerBat->getScale());
        glUniformMatrix4fv(modelTransformationLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
      
        playerBat->draw(shader);

        transformationMatrix = createTransformationMatrix(opponentBat->getPosition(), opponentBat->getRotation(), opponentBat->getScale());
        glUniformMatrix4fv(modelTransformationLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

        opponentBat->draw(shader);
        shader.unUse();
                
        
    
        
       
        glfwSwapBuffers( window );
        
    }
    
    
    /*while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
    
        
        glfwSwapBuffers( window );
    }
     */
    
    
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input


// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
   
    
    
    
    
    
    
    
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



bool inBetween(float p, float a, float b)
{
    if( p < a || p > b) return false;
    else
        return true;
}

glm::mat4 createTransformationMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    
    glm::mat4 model = glm::mat4();
    
    model = glm::translate(glm::mat4(), position);
  //  model = glm::rotate(model,glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
   // model = glm::rotate(model,glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model,glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    model = glm::scale(model, scale);
    
    return model;
}




