#pragma once

#include "Observer.h"
#include "Model/GameObject.h"
template<class T>
class Animator: public Observer<T>
{
public:
	float currentAnimationDuration = 0.0f;

	//virtual void update(GameObject gameObject) = 0;
	virtual void animate(float dt) = 0;
};

