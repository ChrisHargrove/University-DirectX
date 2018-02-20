//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Text.h, Text.cpp
//
//  Created By:     Chris Hargove
//  Last Updated:   15/02/2018
//  
//  This class handles all drawing of text to the screen. Only 1 Instance is required for
//  all wring purposes.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Headers
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Texture.h"
#include "TextShader.h"
#include <string>
#include <d3d11.h>
#include <xnamath.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Text Vertex Struct
//  --pos-- X,Y,Z position of vertex.
//  --text-- U,V texture co-ordinates.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct TextVertexPos {
    XMFLOAT3 pos;
    XMFLOAT2 tex;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  This class handles all drawing of text to the screen. Only 1 Instance is required for
//  all wring purposes.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Text
{
public:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Default Constructor
    //  --texture-- A pointer to a texture for the text to render with.
    //  --shaderPtr-- not used atm, but for later changes down the road.
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Text(Texture* texture, TextShader* shaderPtr);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Default Destructor
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ~Text();

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Draws a string to the screen at the x,y position given and the color specified.
    //  --message-- The message you want written to the screen e, "Hello World!"
    //  --posX-- The x position to start drawing from in NDC
    //  --posY-- The y position to start drawing from in NDC
    //  --color-- The color you wish the text to be. Defaults to White.
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    bool DrawString(const std::string& message, float posX, float posY, XMFLOAT3 color = XMFLOAT3(1.0f,1.0f,1.0f));

	Texture* GetTexture() { return _Texture; }
private:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  Member Variables.
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Texture* _Texture;              //Texture Pointer

    const int MAX_LENGTH = 24;      //Maximum Length of a string
    const int VERTS_PER_LETTER = 6; //Number of vertices per letter which never changes.
    const unsigned int STRIDE = sizeof(TextVertexPos);  //The stride between each character which also never changes.
    const unsigned int OFFSET = 0;  //The offset whihc is always 0 but it keeps code tidy.
    const int SPRITE_SIZE = sizeof(TextVertexPos) * 6;  //Size of each letter sprite in bytes. Doesnt change.

    ID3D11Buffer* _TextBuffer;      //The buffer holding all the text vertices.
    TextShader _Shader;             //The shader to use to draw all text.
};

