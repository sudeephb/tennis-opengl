#pragma once

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

class TextureLoading
{
public:
    static GLuint LoadTexture( const char* filename )
    {
        GLuint textureId;
        int width, height, nrComponents;
        unsigned char* image = SOIL_load_image(filename, &width, &height, &nrComponents, SOIL_LOAD_AUTO);
        
        if (image == NULL) {
            std::cout << "Error: SOIL " << filename << std::endl;
            getchar();
            exit(0);
        }
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureId;
    }
    
    static GLuint LoadCubemap( vector<const GLchar * > faces)
    {
        GLuint textureID;
        glGenTextures( 1, &textureID );
        
        int imageWidth, imageHeight;
        unsigned char *image;
        
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );
        
        for ( GLuint i = 0; i < faces.size( ); i++ )
        {
            image = SOIL_load_image( faces[i], &imageWidth, &imageHeight, 0, SOIL_LOAD_AUTO );
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
            SOIL_free_image_data( image );
        }
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
        glBindTexture( GL_TEXTURE_CUBE_MAP, 0);
        
        return textureID;
    }
};
