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

	bool Initialize(char* fontFileLocation, const char* textureFileLocation);
	void Shutdown();

	Texture* GetTexture();

	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char* fileLocation);
	void ReleaseFontData();
	bool LoadTexture(const char* fileLocation);
	void ReleaseTexture();

private:
	FontType* m_font;
	Texture* m_texture;
};