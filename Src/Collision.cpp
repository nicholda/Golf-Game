#include "Collision.h"
#include "ECS/ColliderComponent.h"

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB, const Vector2& prevPos) {
	if (colA.tag == colB.tag) { // return early, more performant
		return false;
	}

	for (float i = 1; i <= 1000; i++) { // takes 1,000 lerped collision samples for high precision, lower if performance becomes an issue
		Vector2 pos;
		pos.x = static_cast<float>(colA.collider.x);
		pos.y = static_cast<float>(colA.collider.y);
		Vector2 lerpedPos;
		lerpedPos.lerp(prevPos, pos, i / 1000);
		if (
			lerpedPos.x + static_cast<float>(colA.collider.w) >= static_cast<float>(colB.collider.x) &&
			static_cast<float>(colB.collider.x + colB.collider.w) >= lerpedPos.x &&
			lerpedPos.y + static_cast<float>(colA.collider.h) >= static_cast<float>(colB.collider.y) &&
			static_cast<float>(colB.collider.y) + static_cast<float>(colB.collider.h) >= lerpedPos.y
			) {
			return true;
		}
	}

	return false;
}

void Collision::Rebound(const ColliderComponent& colA, const ColliderComponent& colB,
	TransformComponent* ballTransform, Vector2& prevBallPos) {
	Vector2 colACenter; // center of a
	colACenter.x = colA.collider.x + colA.collider.w / 2.0f;
	colACenter.y = colA.collider.y + colA.collider.h / 2.0f;
	Vector2 colBCenter; // center of b
	colBCenter.x = colB.collider.x + colB.collider.w / 2.0f;
	colBCenter.y = colB.collider.y + colB.collider.h / 2.0f;

	Vector2 diff = colACenter - colBCenter; // difference between center positions

	Vector2 minDist; // the minimum distance between the middle of colliders for them to be intersecting
	minDist.x = colA.collider.w / 2.0f + colB.collider.w / 2.0f;
	minDist.y = colA.collider.h / 2.0f + colB.collider.h / 2.0f;

	Vector2 depth; // how far colA is into colB
	depth.x = diff.x > 0 ? minDist.x - diff.x : -minDist.x - diff.x;
	depth.y = diff.y > 0 ? minDist.y - diff.y : -minDist.y - diff.y;

	if (abs(depth.x) < abs(depth.y)) {
		ballTransform->velocity.x *= -1;
		if (diff.x < 0 && abs(ballTransform->velocity.x) < 0.05) { // left side
			ballTransform->velocity.x = -0.06f; // ensures that the ball doesn't stop while in collider
		} else if (diff.x > 0 && abs(ballTransform->velocity.x) < 0.05) { // right side
			ballTransform->velocity.x = 0.06f;
		}
	} else {
		ballTransform->velocity.y *= -1;
		if (diff.y < 0 && abs(ballTransform->velocity.y) < 0.05) { // top
			ballTransform->velocity.y = -0.06f; // ensures that the ball doesn't stop while in collider
		} else if (diff.y > 0 && abs(ballTransform->velocity.y) < 0.05) { // bottom
			ballTransform->velocity.y = 0.06f;
		}
	}

	ballTransform->position = prevBallPos;

	return;
}