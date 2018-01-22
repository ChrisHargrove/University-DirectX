#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/*******************************************************************************************************************
	Constants.h, Constants.cpp
	Created by Kim Kane
	Last updated: 04/01/2018

	Temporary header file to store all of the engine constant values.
	Eventually, these will be loaded in from files.

*******************************************************************************************************************/

namespace FileConstants {

	const std::string LineBreak = "-";
	const std::string Divider = ":";
	const std::string ObjectBegin = "TYPE >>";
	const std::string ObjectEnd = "----------END----------";
	const std::string EndOfFile = "<";
	const std::string Vertices = "v ";
	const std::string TextureCoords = "vt ";
	const std::string Normals = "vn ";
	const std::string Faces = "f ";

	enum OffsetType { 
		OBJECT_TITLE_OFFSET			= 8,
		OBJECT_DIVIDER_OFFSET		= 2,
		VERTICES_TITLE_OFFSET		= 2,
		TEXTURE_COORDS_TITLE_OFFSET = 3,
		NORMALS_TITLE_OFFSET		= 3,
		FACES_TITLE_OFFSET			= 2
	};
}

namespace InputConstants {

	enum TotalButtons {
		TOTAL_MOUSE_BUTTONS			= 4,
		TOTAL_CONTROLLER_BUTTONS	= 14
	};

	const std::string SupportedGuid = "4c05c405000000000000504944564944";
	const int BufferSize = 33;
	const int DeadZone = 8000;
	const float Speed = 0.1f;
	const float RotateSpeed = 0.01f;
}

namespace ScreenConstants {

	enum WindowSpec {
		SCREEN_WIDTH = 800,
		SCREEN_HEIGHT = 600,
		OPENGL_VERSION = 4,
		OPENGL_SUBVERSION = 0
	};
}

#endif