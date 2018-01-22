#pragma once

#include "GameObject.h"
#include "Animation.h"

#include <map>


class AnimatedGameObject :	public GameObject
{
public:
	AnimatedGameObject(XMVECTOR Position, Texture* texture);
	~AnimatedGameObject();

	void Update(float delta);
	void AddAnimation(std::string name, Animation* anim);
	void Unload();

private:
	std::map<std::string, Animation*> _Animations;
	std::string _CurrentAnim;
};

