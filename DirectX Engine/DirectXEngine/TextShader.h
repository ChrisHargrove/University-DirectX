#pragma once

#include <d3d11.h>
#include <xnamath.h>

class Texture;

class TextShader {

public:
    TextShader();
    ~TextShader();

    bool LoadShader(WCHAR* vertexFileLocation, WCHAR* pixelFileLocation);
    void Bind(Texture* texture, D3D_PRIMITIVE_TOPOLOGY renderMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
    TextShader(const TextShader&);

private:
    void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* fileLocation);
    void SetTexture(Texture* texture);

private:
    ID3D11VertexShader*		m_vertexShader;
    ID3D11PixelShader*		m_pixelShader;
    ID3D11InputLayout*		m_layout;
};