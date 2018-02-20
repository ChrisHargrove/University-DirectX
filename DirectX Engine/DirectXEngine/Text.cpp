//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Headers
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Text.h"
#include "GraphicsManager.h"
#include "ScreenManager.h"
#include "Log.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Text::Text(Texture* texture, TextShader* shaderPtr)
{
    //Get the texture.
    _Texture = texture;
    //sort of buffer description for vertex.
    D3D11_BUFFER_DESC textVertexDesc;
    ZeroMemory(&textVertexDesc, sizeof(textVertexDesc));
    textVertexDesc.Usage = D3D11_USAGE_DYNAMIC;
    textVertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    textVertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    textVertexDesc.ByteWidth = SPRITE_SIZE * MAX_LENGTH;

    //create the buffer for the vertices on GPU
    if (FAILED(Graphics::Instance()->GetDevice()->CreateBuffer(&textVertexDesc, 0, &_TextBuffer))) {
        DX_LOG("Text failed to create Buffer", DX_LOG_EMPTY, LOG_ERROR);
    }
    //Load the shader.
    _Shader.LoadShader(L"fontShader.vs", L"fontShader.ps");

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Text::~Text()
{
    //get rid of texture pointer but dont remove from memory
    _Texture = nullptr;
    //release the vertex buffer from GPU as not needed.
    if (_TextBuffer) _TextBuffer->Release();
    //clear pointer.
    _TextBuffer = nullptr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool Text::DrawString(const std::string& message, float posX, float posY, XMFLOAT3 color)
{
    //get string length
    int length = message.size();
    //cap string length to max size.
    if (length > MAX_LENGTH) length = MAX_LENGTH;

    //calculate all sprite info.
    float charWidth = _Texture->GetHeight() / Screen::Instance()->GetWidth();
    float charHeight = _Texture->GetHeight() / Screen::Instance()->GetHeight();
    float texelWidth = _Texture->GetHeight() / _Texture->GetWidth();

    //create mapped resource to upload vertex data to GPU
    D3D11_MAPPED_SUBRESOURCE mapResource;
    if (FAILED(Graphics::Instance()->GetDeviceContext()->Map(_TextBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource))){
        DX_LOG("Text failed to map resource", DX_LOG_EMPTY, LOG_ERROR);
    }

    //create a vertex array
    TextVertexPos* spritePtr = (TextVertexPos*)mapResource.pData;

    const int indexSpace = static_cast<char>(' ');
    const int indexA = static_cast<char>('A');
    const int indexZ = static_cast<char>('Z');
    const int indexSquare = static_cast<char>(127);

    //for each character in string create all 6 vertices.
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
        
        //if you try to draw character not on the range will default to square
        if (letter < indexSpace || letter > indexSquare)
        {
            texLookup = indexSquare;
        }
        else
        {
            texLookup = (letter - indexSpace);
        }

        //calculate the texture UV's for each vertex.
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
    //finish sorting the vertex info.
    Graphics::Instance()->GetDeviceContext()->Unmap(_TextBuffer, 0);

    //bind shader and setup all buffers sending data to GPU
    _Shader.Bind(_Texture);
    _Shader.UpdateConstantBuffers(XMFLOAT4(color.x, color.y, color.z, 0.0f));

    Graphics::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &_TextBuffer, &STRIDE, &OFFSET);

    //Draw the string.
    Graphics::Instance()->GetDeviceContext()->Draw(VERTS_PER_LETTER * length, 0);

    return true;
}
