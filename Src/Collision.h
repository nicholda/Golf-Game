#pragma once

#include "SDL.h"
#include "Vector2.h"

class ColliderComponent;
class TransformComponent;

class Collision {
public:
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB, const Vector2& prevPos);

	static void Rebound(const ColliderComponent& colA, const ColliderComponent& colB,
		TransformComponent* ballTransform, Vector2& prevBallPos);
};