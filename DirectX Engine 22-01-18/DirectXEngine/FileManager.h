#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

/*******************************************************************************************************************
	FileManager.h, FileManager.cpp
	Created by Kim Kane
	Last updated: 04/01/2018

	*This class is still under construction.
	Singleton class that creates an fstream object and reads in data from files.

*******************************************************************************************************************/
#include <fstream>
#include <string>
#include <map>

#include "Singleton.h"

class FileManager {

public:
	friend class Singleton<FileManager>;

public:
	bool OpenForReading(const char* fileLocation);
	void Close(const char* fileLocation);

	std::istream& ExtractFileData();
	size_t FileDataContains(const std::string& data);
	std::string CutFileData(int startPosition = 0, int endPosition = std::string::npos);

	bool GetObjectData(const std::string& objectName);
	void PrintObjectData();

	size_t Find(const std::string& data);

private:
	const std::istream& Seek(int startPosition, int endPosition);
	int GetFileSize();

	void AddDataToContainer();

private:
	FileManager();
	FileManager(const FileManager&);
	FileManager& operator=(const FileManager&) {};

private:
	std::fstream	m_theFile;
	std::string		m_fileData;

private:
	std::map<std::string, std::string> m_objectData;
};

typedef Singleton<FileManager> File;

#endif

/*
//void ExtractBinaryData(int data);
template<typename T>
void ExtractBinaryData(T& data) {

while (EndFilePtr() < GetFileSize()) {
_theFile.read((char*)&data, sizeof(T));
}
}

//void WriteBinaryData(int data);
template<typename T>
void WriteBinaryData(T data) {
_theFile.write((char*)&data, sizeof(T));

}*/