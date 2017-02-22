#pragma once

#include "sfwdraw.h"
#include "BaseState.h"
#include "Factory.h"


/*
	The gamestate represents a discrete container of all that is 
	necessary to drive the game experience.

	A factory object is used to manage the creation and deletion of
	objects in the simulation.

	The game state updates the entities within the factory using
	a series of 'systems.'
*/

class GameState : public BaseState
{
	Factory factory;
	unsigned spr_space, spr_player, spr_bullet, spr_zombie, spr_font;
	ObjectPool<Entity>::iterator currentCamera;

public:
	virtual void init()
	{
		spr_bullet = sfw::loadTextureMap("../res/bullet.png");
		spr_player = sfw::loadTextureMap("../res/character.png");
		spr_zombie = sfw::loadTextureMap("../res/zombie.png");
		spr_font = sfw::loadTextureMap("../res/font.png",32,4);
	}

	virtual void play()
	{
		
		// delete any old entities sitting around
		for (auto it = factory.begin(); it != factory.end(); it->onFree(), it.free());

		// setup a default camera
		currentCamera = factory.spawnCamera(800, 600, 1);
		currentCamera->transform->setGlobalPosition(vec2{ 400, 300 });

		// call some spawning functions!
		//factory.spawnStaticImage(spr_space, 0, 0, 800, 600);

		factory.spawnPlayer(spr_player);
		factory.BuildTheWall();
		
		
		
	}

	virtual void stop()
	{

	}

	// should return what state we're going to.
	// REMEMBER TO HAVE ENTRY AND STAY states for each application state!
	virtual size_t next() const { return 0; }


	// update loop, where 'systems' exist
	virtual void step()
	{
		float dt = sfw::getDeltaTime();

		// maybe spawn some asteroids here.
		// maybe spawn some zombies here.
		 static float tempDeleteMeHolyFuck = 2.0f;
		tempDeleteMeHolyFuck -= dt;

		if (tempDeleteMeHolyFuck < 0)
		{
			factory.spawnZombie(spr_zombie);
			tempDeleteMeHolyFuck = 2.0f;
		}

		for(auto it = factory.begin(); it != factory.end();) // no++!
		{
			bool del = false; // does this entity end up dying?
			auto &e = *it;    // convenience reference

			// rigidbody update
			if (e.transform && e.rigidbody)
				e.rigidbody->integrate(&e.transform, dt);

			// controller update
			if (e.transform && e.rigidbody && e.controller)
			{
				e.controller->poll(&e.transform, &e.rigidbody, dt);
				if (e.controller->shotRequest) // controller requested a bullet fire
				{
					factory.spawnBullet(spr_bullet, e.transform->getGlobalPosition()  + perp(e.transform->getGlobalUp() ),
											vec2{ 32,32 }, e.transform->getGlobalAngle(), 150, 1);
				}
			}
			// lifetime decay update

			if (e.health && e.rigidbody)
			{
				if (e.health->isDead())
				{
					del = true;
				}
				e.health->PushBack(&e.rigidbody);
			}

			if (e.lifetime)
			{
				e.lifetime->age(dt);
				if (!e.lifetime->isAlive())
					del = true;
			
			}
			// health update
			if (e.health)
			{
				if(e.rigidbody)
					e.health->update(&e.rigidbody);
				//std::cout << "ENTITY HEALTH" << e.health->getHealth() << std::endl;
				if (e.health->isDead())
					del = true;
			}

			// ++ here, because free increments in case of deletions
			if (!del) it++;
			else
			{
				it->onFree();
				it.free();
			}
		}


		// Physics system!
		// You'll want to extend this with custom collision responses

		
		for(auto it = factory.begin(); it != factory.end(); it++) // for each entity
			for(auto bit = it; bit != factory.end(); bit++)		  // for every other entity
				if (it != bit && it->transform && it->collider && bit->transform && bit->collider)
				// if they aren't the same and they both have collidable bits...
				{
					// test their bounding boxes
					if (base::BoundsTest(&it->transform, &it->collider, &bit->transform, &bit->collider))
					{
						// if true, get the penetration and normal from the convex hulls
						auto cd = base::ColliderTest(&it->transform, &it->collider, &bit->transform, &bit->collider);
						
						// if there was a collision,
						if (cd.result())
						{
							// COLLISION SOLVER
							if (!it->collider->isTrigger() && !bit->collider->isTrigger())
							{
								// condition for dynamic resolution
								if (it->rigidbody && bit->rigidbody)
									base::DynamicResolution(cd, &it->transform, &it->rigidbody, &bit->transform, &bit->rigidbody);

								// condition for static resolution
								else if (it->rigidbody && !bit->rigidbody)
									base::StaticResolution(cd, &it->transform, &it->rigidbody);
							}
							// GAME CODE
							if (it->health && bit->rigidbody && bit->lifetime)
							{
								if (it->teams && bit->teams &&
									it->teams->getTeam() == bit->teams->getTeam())
								{
									
									
								}
								else
								{	
									std::cout << "Hit" << std::endl;
									bit->lifetime->lifespan = 0.f;
									it->health->takeDamage(1);			
									it->health->pushRequest = true;
									it->health->PushBack(&it->rigidbody);
								}
							}
							if (it->health && bit->health)
							{
								if (it->teams && bit->teams &&
									it->teams->getTeam() == bit->teams->getTeam())
								{

								}
								else
								{
									std::cout << "Zombie hit wall" << std::endl;
									it->health->takeDamage(1);
									bit->health->pushRequest = true;
									bit->health->PushBack(&bit->rigidbody);
									//bit->rigidbody->addImpulse(vec2{ 75,0 });
									//bit->transform->setGlobalPosition(bit->transform->getGlobalPosition() + vec2{ 300,0 });
								}
							}
						}
					}
				}

	}


	virtual void draw()	
	{
		// kind of round about, but this is the camera matrix from the factory's current camera
		auto cam = currentCamera->camera->getCameraMatrix(&currentCamera->transform);

		// draw sprites
		for each(auto &e in factory)
			if (e.transform && e.sprite)
				e.sprite->draw(&e.transform, cam);

		// draw text
		for each(auto &e in factory)
			if (e.transform && e.text)
				e.text->draw(&e.transform, cam);


#ifdef _DEBUG
		for each(auto &e in factory)
			if (e.transform)
				e.transform->draw(cam);

		for each(auto &e in factory)
			if (e.transform && e.collider)
				e.collider->draw(&e.transform, cam);

		for each(auto &e in factory)
			if (e.transform && e.rigidbody)
				e.rigidbody->draw(&e.transform, cam);
#endif
	}
};