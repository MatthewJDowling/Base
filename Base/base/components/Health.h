#pragma once
#include "base.h"
#include "Rigidbody.h"
using namespace base;

class Health
{
	float hitPoints; 
	
	bool zombie = false;

public:
	bool pushRequest = false;
	Health() {}
	Health(float initialHealth) : hitPoints(initialHealth) {}

	void takeDamage(float damageDealt)
	{
		hitPoints -= damageDealt;
		
	}

	void setHealth(float entityHealth)
	{
		hitPoints = entityHealth;
	}

	float getHealth()
	{
		return hitPoints;
	}

	bool isDead()
	{
		return hitPoints <= 0.0f;
	}

	void PushBack(Rigidbody *R)
	{
		if (pushRequest == true)
		{

			R->addImpulse(vec2{ 100,0 });

			pushRequest = false;

		}
	}

	void update(Rigidbody *R)
	{
		if (zombie)
		{
			R->addForce(vec2{ -10,0 });
		}
			
		
	}
};