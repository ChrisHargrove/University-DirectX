#include "FileManager.h"
#include "Constants.h"
#include "Log.h"


/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
FileManager::FileManager()	:	m_theFile(""),
								m_fileData("")
{
	DX_LOG("[FILE] File constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Opens a text file for reading data only
*******************************************************************************************************************/
bool FileManager::OpenForReading(const char* fileLocation)
{
	m_theFile.open(fileLocation, std::ifstream::in);

	if (!m_theFile.is_open())	{ DX_LOG("[FILE] File doesn't exist: ", fileLocation, LOG_ERROR); return false; }
	if (!m_theFile.good())		{ DX_LOG("[FILE] File corrupted or a file is already open: ", fileLocation, LOG_ERROR); return false; }

	DX_LOG("[FILE] File opened successfully: ", fileLocation, LOG_SUCCESS);

	return true;
}


/*******************************************************************************************************************
	Function that closes a file
*******************************************************************************************************************/
void FileManager::Close(const char* fileLocation) {

	m_theFile.close();

	DX_LOG("[FILE] File closed successfully: ", fileLocation, LOG_SUCCESS);
}


/*******************************************************************************************************************
	Function that gets all the data from a file and stores it in to a string
*******************************************************************************************************************/
std::istream& FileManager::ExtractFileData() { return std::getline(m_theFile, m_fileData); }


/*******************************************************************************************************************
	Function that checks if certain data within a file exists
*******************************************************************************************************************/
size_t FileManager::FileDataContains(const std::string &data) { return Find(data) == 0; }


/*******************************************************************************************************************
	Function that cuts the string containing the file data, from a start to an end position
*******************************************************************************************************************/
std::string FileManager::CutFileData(int startPosition, int endPosition) { return m_fileData.substr(startPosition, endPosition); }


/*******************************************************************************************************************
	Function that moves the file pointer from a starting position to an end position
*******************************************************************************************************************/
const std::istream& FileManager::Seek(int startPosition, int endPosition) { return m_theFile.seekg(startPosition, endPosition); }


/*******************************************************************************************************************
	Function that finds data within a file and returns its position as a number
*******************************************************************************************************************/
size_t FileManager::Find(const std::string &data) { return m_fileData.find(data); }


/*******************************************************************************************************************
	Function that returns the size of a file in bytes
*******************************************************************************************************************/
int FileManager::GetFileSize()
{
	Seek(0, std::ios_base::end);
	int fileSize = (int)m_theFile.tellg();
	Seek(0, std::ios_base::beg);

	return fileSize;
}


/*******************************************************************************************************************
	[TEMPORARY] Get object data from a file, depending on the objectName passed to the function
*******************************************************************************************************************/
bool FileManager::GetObjectData(const std::string &objectName) {

	bool objectFound = false;

	while (ExtractFileData()) {

		if (FileDataContains(FileConstants::ObjectBegin)) {
			
			std::string fileObjectName = CutFileData(FileConstants::OBJECT_TITLE_OFFSET);

			if (objectName == fileObjectName) { objectFound = true; }
		} 
		else if (FileDataContains(FileConstants::ObjectEnd) && objectFound) { m_theFile.close(); }
		else if (objectFound) { 
			
			if (FileDataContains(FileConstants::LineBreak)) { continue; }
			
			AddDataToContainer();
		}
		else if (FileDataContains(FileConstants::EndOfFile) && !objectFound) {
			DX_LOG("[FILE] Object not found in file: ", objectName.c_str(), LOG_ERROR);
			m_theFile.close();
			return false;
		}
	}

	return true;
}


/*******************************************************************************************************************
	Adds object data to a map container
*******************************************************************************************************************/
void FileManager::AddDataToContainer() {

	int equalPosition = Find(FileConstants::Divider);
	std::string dataKey = CutFileData(0, equalPosition);
	std::string dataValue = CutFileData(equalPosition + FileConstants::OBJECT_DIVIDER_OFFSET);

	m_objectData[dataKey] = dataValue;
}


/*******************************************************************************************************************
	Prints all the game object data stored in the m_objectData map
*******************************************************************************************************************/
void FileManager::PrintObjectData() {
	
	if (!m_objectData.empty()) {

		DX_LOG("The following object data was stored: ", DX_LOG_EMPTY, LOG_MESSAGE);

		for (auto const& object : m_objectData) {
			DX_LOG("Key: ", object.first.c_str(), LOG_MESSAGE);
			DX_LOG("Value: ", object.second.c_str(), LOG_MESSAGE);
		}
	}

	else { DX_LOG("[FILE] Object data container is empty", DX_LOG_EMPTY, LOG_WARN); }
}