#include "Animation.h"



Animation::Animation(int NumFrames, char* FileName, ID3D11Device* Device) :
	_DeltaTime(0),
	_CurrentFrame(0),
	_NumFrames(NumFrames)
{
	Load(FileName, Device);
}


Animation::~Animation()
{
}

Model * Animation::GetCurrentFrame()
{
	if (_CurrentFrame == _NumFrames) {
		_CurrentFrame == 0;
	}
	return _Models[_CurrentFrame];
}

void Animation::Update(float deltaTime)
{
	_DeltaTime += deltaTime;
	if (_DeltaTime >= 0.1) {
		_DeltaTime = 0;
		_CurrentFrame++;
	}
	if (_NumFrames <= _CurrentFrame) {
		_CurrentFrame = 0;
	}
}

void Animation::Load(char* FileName, ID3D11Device* Device)
{
	std::string NewFileName;

	for (int i = 0; i < _NumFrames; i++) {
		NewFileName = FileName;
		NewFileName += "_" + std::to_string(i) + ".obj";

		char* temp = (char*)NewFileName.c_str();

		_Models.push_back(new Model());
		_Models[i]->Load(temp, Device);
	}
}

void Animation::Unload()
{
	for (auto mp : _Models) {
		delete mp;
		mp = nullptr;
	}
}
