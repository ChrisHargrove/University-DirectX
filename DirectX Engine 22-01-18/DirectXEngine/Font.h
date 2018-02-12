#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include <fstream>


#include "texture.h"

class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	bool Initialize(ID3D11Device* device, char* fontFileLocation, const char* textureFileLocation);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char* fileLocation);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, const char* fileLocation);
	void ReleaseTexture();

private:
	FontType* m_font;
	Texture* m_texture;
};