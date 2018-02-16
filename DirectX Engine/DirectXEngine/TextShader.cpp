#include "TextShader.h"
#include "ScreenManager.h"
#include "GraphicsManager.h"

#include "Log.h"
#include "Texture.h"

#include <d3dcompiler.h>
#include <fstream>

TextShader::TextShader() :  m_vertexShader(nullptr),
                            m_pixelShader(nullptr),
                            m_layout(nullptr)
{
}

TextShader::~TextShader()
{
    if (m_layout) { m_layout->Release(); m_layout = nullptr; }
    if (m_pixelShader) { m_pixelShader->Release(); m_pixelShader = nullptr; }
    if (m_vertexShader) { m_vertexShader->Release(); m_vertexShader = nullptr; }
}

bool TextShader::LoadShader(WCHAR * vertexFileLocation, WCHAR * pixelFileLocation)
{
    HRESULT result = S_OK;

    ID3D10Blob* errorMessage = nullptr;
    ID3D10Blob* vertexShaderBuffer = nullptr;

    //-------------------------------------------- Compile the vertex shader code
    result = D3DCompileFromFile(vertexFileLocation, nullptr, nullptr, "VertexMain", "vs_4_0",
        D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);

    if (FAILED(result))
    {
        //-------------------------------------------- If the shader failed to compile it should have writen something to the error message
        if (errorMessage) { OutputShaderErrorMessage(errorMessage, vertexFileLocation); }

        //-------------------------------------------- If there was  nothing in the error message then it simply could not find the shader file itself
        else { MessageBox(Screen::Instance()->GetWindow(), (LPCSTR)vertexFileLocation, "Missing Shader File", MB_OK); }

        return false;
    }

    ID3D10Blob* pixelShaderBuffer = nullptr;

    //-------------------------------------------- Compile the pixel shader code
    result = D3DCompileFromFile(pixelFileLocation, nullptr, nullptr, "PixelMain", "ps_4_0",
        D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

    if (FAILED(result))
    {
        if (errorMessage) { OutputShaderErrorMessage(errorMessage, pixelFileLocation); }
        else { MessageBox(Screen::Instance()->GetWindow(), (LPCSTR)pixelFileLocation, "Missing Shader File", MB_OK); }

        return false;
    }

    //-------------------------------------------- Create the vertex shader from the buffer
    result = Graphics::Instance()->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
    if (FAILED(result)) {
        DX_LOG("[BASIC SHADER] Can't create vertex shader", DX_LOG_EMPTY, LOG_ERROR); return false;
    }

    //-------------------------------------------- Create the pixel shader from the buffer
    result = Graphics::Instance()->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
    if (FAILED(result)) {
        DX_LOG("[BASIC SHADER] Can't create pixel shader", DX_LOG_EMPTY, LOG_ERROR); return false;
    }

    //-------------------------------------------- Create the layout description
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //-------------------------------------------- Create vertex input layout
    result = Graphics::Instance()->GetDevice()->CreateInputLayout(layout, _countof(layout), vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result)) {
        DX_LOG("[BASIC SHADER] Can't create the input layout", DX_LOG_EMPTY, LOG_ERROR); return false;
    }

    //-------------------------------------------- Release the vertex shader buffer and pixel shader buffer since they are no longer needed
    vertexShaderBuffer->Release();
    vertexShaderBuffer = nullptr;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = nullptr;

    return true;
}

void TextShader::Bind(Texture * texture, D3D_PRIMITIVE_TOPOLOGY renderMode)
{
    //-------------------------------------------- Set the vertex input layout
    Graphics::Instance()->GetDeviceContext()->IASetInputLayout(m_layout);

    //-------------------------------------------- Set how this will be drawn - triangles/lines/points, etc.
    Graphics::Instance()->GetDeviceContext()->IASetPrimitiveTopology(renderMode);

    //-------------------------------------------- Set the vertex and pixel shaders that will be used to render this object
    Graphics::Instance()->GetDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
    Graphics::Instance()->GetDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);

    SetTexture(texture);
}

void TextShader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, WCHAR * fileLocation)
{
    std::ofstream file;

    //-------------------------------------------- Get a pointer to the error message text buffer
    char* compileErrors = (char*)(errorMessage->GetBufferPointer());

    //-------------------------------------------- Get the length of the message
    unsigned long bufferSize = errorMessage->GetBufferSize();

    //-------------------------------------------- Open a file to write the error message to
    file.open("ShaderErrors.txt");

    //-------------------------------------------- Write out the error message
    for (unsigned long i = 0; i < bufferSize; i++) { file << compileErrors[i]; }

    //-------------------------------------------- Close file and release error message pointer
    file.close();
    errorMessage->Release();
    errorMessage = nullptr;

    //-------------------------------------------- Pop a message up on the screen to notify the user to check the text file for compile errors
    MessageBox(Screen::Instance()->GetWindow(), "Error compiling shader.  Check ShaderErrors.txt for message.", (LPCSTR)fileLocation, MB_OK);
}

void TextShader::SetTexture(Texture * texture)
{
    if (texture != nullptr) {
        Graphics::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, texture->GetTexture());
        Graphics::Instance()->GetDeviceContext()->PSSetSamplers(0, 1, texture->GetSampler());
    }
}
