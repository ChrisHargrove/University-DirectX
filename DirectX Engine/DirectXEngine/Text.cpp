#include "Text.h"
#include "GraphicsManager.h"
#include "ScreenManager.h"
#include "Log.h"



Text::Text(Texture* texture, TextShader* shaderPtr)
{
    _Texture = texture;
    D3D11_BUFFER_DESC textVertexDesc;
    ZeroMemory(&textVertexDesc, sizeof(textVertexDesc));
    textVertexDesc.Usage = D3D11_USAGE_DYNAMIC;
    textVertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    textVertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    textVertexDesc.ByteWidth = SPRITE_SIZE * MAX_LENGTH;

    if (FAILED(Graphics::Instance()->GetDevice()->CreateBuffer(&textVertexDesc, 0, &_TextBuffer))) {
        DX_LOG("Text failed to create Buffer", DX_LOG_EMPTY, LOG_ERROR);
    }
    _Shader.LoadShader(L"Assets\\Shaders\\fontShader.vs", L"Assets\\Shaders\\fontShader.ps");

}

Text::~Text()
{
    _Texture = nullptr;
    if (_TextBuffer) _TextBuffer->Release();
    _TextBuffer = nullptr;
}

bool Text::DrawString(const std::string& message, float posX, float posY)
{
    int length = strlen(message.c_str());
    if (length > MAX_LENGTH) length = MAX_LENGTH;

    float charWidth = 32.0f / Screen::Instance()->GetWidth();
    float charHeight = 32.0f / Screen::Instance()->GetHeight();
    float texelWidth = 32.0f / _Texture->GetWidth();

    D3D11_MAPPED_SUBRESOURCE mapResource;
    if (FAILED(Graphics::Instance()->GetDeviceContext()->Map(_TextBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource))){
        DX_LOG("Text failed to map resource", DX_LOG_EMPTY, LOG_ERROR);
    }

    TextVertexPos* spritePtr = (TextVertexPos*)mapResource.pData;

    const int indexSpace = static_cast<char>(' ');
    const int indexA = static_cast<char>('A');
    const int indexZ = static_cast<char>('Z');
    const int indexSquare = static_cast<char>(127);

    for (int i = 0; i < length; ++i)
    {
        float thisStartX = posX + (charWidth * static_cast<float>(i));
        float thisEndX = thisStartX + charWidth;
        float thisEndY = posY + charHeight;

        spritePtr[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
        spritePtr[1].pos = XMFLOAT3(thisEndX, posY, 1.0f);
        spritePtr[2].pos = XMFLOAT3(thisStartX, posY, 1.0f);
        spritePtr[3].pos = XMFLOAT3(thisStartX, posY, 1.0f);
        spritePtr[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
        spritePtr[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

        int texLookup = 0;
        int letter = static_cast<char>(message[i]);

        //if( letter < indexA || letter > indexZ )
        if (letter < indexSpace || letter > indexSquare)
        {
            // Grab one index past Z, which is a blank space in the texture.
            // texLookup = ( indexZ - indexA ) + 1;
            texLookup = indexSquare;
        }
        else
        {
            // A = 0, B = 1, Z = 25, etc.
            //texLookup = ( letter - indexA );
            texLookup = (letter - indexSpace);
        }

        float tuStart = 0.0f + (texelWidth * static_cast<float>(texLookup));
        float tuEnd = tuStart + texelWidth;

        spritePtr[0].tex = XMFLOAT2(tuEnd, 0.0f);
        spritePtr[1].tex = XMFLOAT2(tuEnd, 1.0f);
        spritePtr[2].tex = XMFLOAT2(tuStart, 1.0f);
        spritePtr[3].tex = XMFLOAT2(tuStart, 1.0f);
        spritePtr[4].tex = XMFLOAT2(tuStart, 0.0f);
        spritePtr[5].tex = XMFLOAT2(tuEnd, 0.0f);

        spritePtr += 6;
    }

    Graphics::Instance()->GetDeviceContext()->Unmap(_TextBuffer, 0);

    _Shader.Bind(_Texture);

    Graphics::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &_TextBuffer, &STRIDE, &OFFSET);

    Graphics::Instance()->GetDeviceContext()->Draw(VERTS_PER_LETTER * length, 0);

    return true;
}
