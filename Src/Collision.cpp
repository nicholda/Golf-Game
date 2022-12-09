#include "Collision.h"
#include "ECS/ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		) {
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	if (colA.tag != colB.tag and AABB(colA.collider, colB.collider)) {
		//std::cout << colA.tag << " hit: " << colB.tag << "\n";
		return true;
	}
	else {
		return false;
	}
}

void Collision::reboundBall(const ColliderComponent& colA, const ColliderComponent& colB, TransformComponent* ballTransform, Vector2D prevBallPos) {
	Vector2D colACenter;
	colACenter.x = colA.collider.x + colA.collider.w / 2;
	colACenter.y = colA.collider.y + colA.collider.h / 2;
	Vector2D colBCenter;
	colBCenter.x = colB.collider.x + colB.collider.w / 2;
	colBCenter.y = colB.collider.y + colB.collider.h / 2;

	Vector2D diff = colACenter - colBCenter;
	
	Vector2D minDist;
	minDist.x = colA.collider.w / 2 + colB.collider.w / 2;
	minDist.y = colA.collider.h / 2 + colB.collider.h / 2;
	
	Vector2D depth;
	depth.x = diff.x > 0 ? minDist.x - diff.x : -minDist.x - diff.x;
	depth.y = diff.y > 0 ? minDist.y - diff.y : -minDist.y - diff.y;
	
	if (abs(depth.x) < abs(depth.y)) {
		ballTransform->velocity.x *= -1;
		if (diff.x < 0 and ballTransform->velocity.x > -0.05) {
			ballTransform->velocity.x == 0.06; // ensures that the ball doesn't stop while in collider
		} else if (ballTransform->velocity.x < 0.05) {
			ballTransform->velocity.x == 0.06;
		}
	} else {
		ballTransform->velocity.y *= -1;
		if (diff.y < 0 and ballTransform->velocity.y > -0.05) {
			ballTransform->velocity.y == 0.06; // ensures that the ball doesn't stop while in collider
		} else if (ballTransform->velocity.y < 0.05) {
			ballTransform->velocity.y == 0.06;
		}
	}
}