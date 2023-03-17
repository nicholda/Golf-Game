#pragma once

#include "SDL.h"
#include "Vector2.h"

class ColliderComponent;
class TransformComponent;

class Collision {
public:
	// this function performs an AABB collision check between two colliders previous and current positions
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB, const Vector2& prevPos);

	// this function inverses the velocity of a collider based on what face has been hit
	static void Rebound(const ColliderComponent& colA, const ColliderComponent& colB,
		TransformComponent* ballTransform, Vector2& prevBallPos);
};