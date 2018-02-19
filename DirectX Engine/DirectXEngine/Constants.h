#pragma once

/*******************************************************************************************************************
	Constants.h, Constants.cpp
	Created by Kim Kane
	Last updated: 27/01/2018

	Temporary header file to store all of the engine constant values.
	Eventually, these will be loaded in from files.

*******************************************************************************************************************/
#include <string>
#include <d3d11.h>
#include <xnamath.h>

namespace FileConstants {

	const std::string LineBreak		= "-";
	const std::string Divider		= ":";
	const std::string ObjectBegin	= "TYPE >>";
	const std::string ObjectEnd		= "----------END----------";
	const std::string EndOfFile		= "<";
	const std::string Vertices		= "v ";
	const std::string TextureCoords = "vt ";
	const std::string Normals		= "vn ";
	const std::string Faces			= "f ";

	enum OffsetType { 
		OBJECT_TITLE_OFFSET			= 8,
		OBJECT_DIVIDER_OFFSET		= 2,
		VERTICES_TITLE_OFFSET		= 2,
		TEXTURE_COORDS_TITLE_OFFSET = 3,
		NORMALS_TITLE_OFFSET		= 3,
		FACES_TITLE_OFFSET			= 2,
		RGB_OFFSET					= 3,
		HEIGHTMAP_VERTICES			= 6
	};
}

namespace InputConstants {

	enum TotalButtons {
		TOTAL_KEYS	= 256
	};

	enum MouseButtons {
		LEFT		= 0,
		RIGHT		= 1,
		MIDDLE		= 2
	};

	const float Speed		= 0.1f;
	const float RotateSpeed = 0.1f;
}


namespace ScreenConstants {

	enum WindowSpec {
		SCREEN_WIDTH	= 800,
		SCREEN_HEIGHT	= 600
	};

	const float FieldOfView		= (float)3.141592654f / 4.0f;
	const float NearView		= 0.1f;
	const float FarView			= 100.0f;
	const float ScreenDepth		= FarView - NearView;
}


namespace GraphicConstants {

	enum GraphicSettings {
		BUFFER_SIZE		= 128
	};
}


namespace MathsConstants {
	
	const float Radians = 0.0174532925f;
}


namespace BufferConstants {

	struct PackedTerrainVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 textureCoord;
		XMFLOAT3 normal;
	};

	struct PackedVertex {
		XMFLOAT3 position;
		XMFLOAT2 textureCoord;
		XMFLOAT3 normal;
		bool operator<(const PackedVertex that) const {
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
		};
	};
}