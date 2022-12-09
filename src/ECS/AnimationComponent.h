#pragma once

struct AnimationComponent {
	int index;
	int frames;
	int speed;

	AnimationComponent() {}
	AnimationComponent(int i, int f, int s) {
		index = i;
		frames = f;
		speed = s;
	}
};