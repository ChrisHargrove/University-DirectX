#pragma once
#include "Model.h"

#include <vector>

class Animation
{
public:
	Animation(int NumFrames, char* FileName, ID3D11Device* Device);
	~Animation();

	Model* GetCurrentFrame();
	void Update(float deltaTime);

	void Load(char* FileName, ID3D11Device* Device);
	void Unload();

private:
	std::vector<Model*> _Models;

	float _DeltaTime;
	int _NumFrames;
	int _CurrentFrame;
	
};

