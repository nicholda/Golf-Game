#pragma once

#include "SDL.h"
#include "Vector2D.h"

class ColliderComponent;
class TransformComponent;

class Collision {
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);

	static void reboundBall(const ColliderComponent& colA, const ColliderComponent& colB, TransformComponent* ballTransform, Vector2D prevBallPos);
};