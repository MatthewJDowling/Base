#pragma once

#include "Entity.h"
#include <time.h>
class Factory
{	
	ObjectPool<Entity>	  entities;
	
	// The factory will store an object pool for each component type
	ObjectPool<Transform> transforms;
	ObjectPool<Rigidbody> rigidbodies;
	ObjectPool<Collider>  colliders;
	ObjectPool<Sprite>    sprites;
	ObjectPool<Lifetime>  lifetimes;
	ObjectPool<Camera>    cameras;
	ObjectPool<Text>	  texts;
	ObjectPool<PlayerController> controllers;
	ObjectPool<Health>	  healths;
	ObjectPool<Faction>	  teams;

public:

	// iterators to access the entity pool
	ObjectPool<Entity>::iterator begin() { return entities.begin(); }
	ObjectPool<Entity>::iterator end() { return entities.end(); }

	// for now, they're all the same size
	Factory(size_t size = 512)
		: entities(size), transforms(size), rigidbodies(size),
		colliders(size), sprites(size), lifetimes(size),
		cameras(size), controllers(size), texts(size), healths(size), teams(size)
	{
	}

	// What follows are specialized spawning functions
	// just observe the steps taken and replicate for your own usages

	ObjectPool<Entity>::iterator spawnCamera(float w2, float h2, float zoom)
	{
		auto e = entities.push();
		e->transform = transforms.push();
		e->camera = cameras.push();
		e->camera->offset = vec2{w2,h2};
		e->camera->scale = vec2{ zoom,zoom };
		return e;
	}

	ObjectPool<Entity>::iterator spawnStaticImage(unsigned sprite, float x, float y, float w, float h)
	{
		auto e = entities.push();
		e->transform = transforms.push();
		e->sprite = sprites.push();
		e->sprite->sprite_id = sprite;
		e->sprite->dimensions = vec2{w,h};
		e->transform->setLocalPosition(vec2{ x,y });	
		return e;
	}

	ObjectPool<Entity>::iterator spawnBullet(unsigned sprite, vec2 pos, vec2 dim, float ang, float impulse, unsigned faction)
	{
		auto e = entities.push();

		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->lifetime = lifetimes.push();
		e->collider = colliders.push();
		e->teams = teams.push(Faction(0));

		e->transform->setLocalPosition(pos);
		e->transform->setLocalScale(dim);
		e->transform->setLocalAngle(ang);

		e->collider->setTrigger(true);

		e->sprite->sprite_id = sprite;
		e->sprite->dimensions = vec2{1.2f, 1.2f};

		e->rigidbody->addImpulse(vec2{3,0} *impulse);

		e->lifetime->lifespan = 1.6f;
		
		return e;
	}

	ObjectPool<Entity>::iterator spawnPlayer(unsigned sprite)
	{
		auto e = entities.push();

		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->collider = colliders.push();
		e->controller = controllers.push();
		e->text = texts.push();
		e->teams = teams.push(Faction(0));

		
		e->text->offset = vec2{ -24,-24 };
		e->text->off_scale = vec2{.5f,.5f};
		//e->text->setString("Player1");

		e->transform->setLocalScale(vec2{48,48});


		e->transform->setGlobalPosition(vec2{-375,10});
		e->rigidbody->mass = 1.0f;
		e->rigidbody->drag = 25.f;


		e->sprite->sprite_id = sprite;

		return e;
	}


	ObjectPool<Entity>::iterator spawnZombie(unsigned sprite)
	{
		auto e = entities.push();


		e->transform = transforms.push();
		e->rigidbody = rigidbodies.push();
		e->sprite = sprites.push();
		e->collider = colliders.push();
		e->health = healths.push(Health(3));
		e->teams = teams.push(Faction(1));

		e->transform->setLocalScale(vec2{ 48,48 });
		e->transform->setGlobalPosition(vec2{ 400, randRange(0,500) - 250});

		std::cout << e->transform->getGlobalPosition().y << std::endl;

		e->rigidbody->addForce(vec2{ -3000,0 });

		std::cout << " " << e->rigidbody->force.x << std::endl;

		e->sprite->sprite_id = sprite;

		return e;
	}
	ObjectPool<Entity>::iterator BuildTheWall()
	{
		auto e = entities.push();

		e->transform = transforms.push();
		//e->rigidbody = rigidbodies.push();
		//e->sprite = sprites.push();
		


		e->collider = colliders.push();
		e->health = healths.push(Health(10));
		e->teams = teams.push(Faction(0));

		vec2 verts[] = { vec2{0,0}, vec2{10,0}, vec2{10,10}, vec2{0,10} };
		e->collider = colliders.push(Collider{ verts,4 });

		e->transform->setLocalScale(vec2{ 1,1000 });
		e->transform->setGlobalPosition(vec2{-325,-325});


		//e->sprite->sprite_id = sprite;

		return e;
	}
};


