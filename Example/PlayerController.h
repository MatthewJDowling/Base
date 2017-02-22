#pragma once

#include "Base.h"

using namespace base; 

class PlayerController
{

public:
	float speed = 0, turnSpeed = 1;

	float shotTimer = 0.0f;
	bool shotRequest = false;

	void poll(base::Transform *T, base::Rigidbody *rb, float dt)
	{		
			if (sfw::getKey('W'))
				rb->addForce(vec2{ 0,6500 });

			if (sfw::getKey('S'))
				rb->addForce(vec2{ 0,-6500 });
		


		shotTimer -= dt;
		if (sfw::getKey(' ') && shotTimer < 0)
		{
			shotRequest = true;
			shotTimer = 0.33f;
		}
		else shotRequest = false;
	}

};