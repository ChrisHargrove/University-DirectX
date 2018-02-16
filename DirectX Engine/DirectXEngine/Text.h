#pragma once

#include "Texture.h"
#include "TextShader.h"
#include <d3d11.h>
#include <xnamath.h>
#include <string>

struct TextVertexPos {
    XMFLOAT3 pos;
    XMFLOAT2 tex;
};

class Text
{
public:
    Text(Texture* texture, TextShader* shaderPtr);
    ~Text();

    bool DrawString(const std::string& message, float posX, float posY);

private:
    Texture* _Texture;

    const int MAX_LENGTH = 24;
    const int VERTS_PER_LETTER = 6;
    const unsigned int STRIDE = sizeof(TextVertexPos);
    const unsigned int OFFSET = 0;
    const int SPRITE_SIZE = sizeof(TextVertexPos) * 6;

    ID3D11Buffer* _TextBuffer;
    TextShader _Shader;
};

