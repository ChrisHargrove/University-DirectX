#include "Font.h"
#include "GraphicsManager.h"

Font::Font()
{
	m_font = 0;
	m_texture = 0;
}

Font::Font(const Font &)
{
}

Font::~Font()
{
}

bool Font::Initialize(char* fontFileLocation, const char* textureFileLocation)
{
	bool result;


	// Load in the text file containing the font data.
	result = LoadFontData(fontFileLocation);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(textureFileLocation);
	if (!result)
	{
		return false;
	}

	return true;
}

void Font::Shutdown()
{
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();
}

Texture * Font::GetTexture()
{
    return m_texture;
}

void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// Draw each letter onto a quad.
	for (i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}
}

bool Font::LoadFontData(char* fileLocation)
{
	std::ifstream fin;
	int i;
	char temp;


	// Create the font spacing buffer.
	m_font = new FontType[95];
	if (!m_font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(fileLocation);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void Font::ReleaseFontData()
{
	// Release the font data array.
	if (m_font)
	{
		delete[] m_font;
		m_font = 0;
	}

}

bool Font::LoadTexture(const char* fileLocation)
{
	// Create the texture object.
	m_texture = new Texture();
	if (!m_texture)
	{
		return false;
	}
    m_texture->LoadTexture(fileLocation);

	return true;
}

void Font::ReleaseTexture()
{
	// Release the texture object.
	if (m_texture)
	{
		//m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

}
