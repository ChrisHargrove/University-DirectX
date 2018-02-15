#include "AnimatedGameObject.h"



AnimatedGameObject::AnimatedGameObject(XMVECTOR Position, Texture* texture) 
	//GameObject(Position, nullptr, texture)
{
	
	_CurrentAnim = "walk";
}


AnimatedGameObject::~AnimatedGameObject()
{
}

void AnimatedGameObject::Update(float delta)
{
	_Animations.at(_CurrentAnim)->Update(delta);
	SetModel(_Animations.at(_CurrentAnim)->GetCurrentFrame());
}

void AnimatedGameObject::AddAnimation(std::string name,Animation * anim)
{
	_Animations.emplace(std::pair<std::string, Animation*>(name, anim));
}

void AnimatedGameObject::Unload()
{
	for (auto a : _Animations) {
		a.second->Unload();
		delete a.second;
	}
}
