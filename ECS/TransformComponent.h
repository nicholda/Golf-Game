#pragma once

#include "Components.h"

class TransformComponent : public Component {
private:
	int xpos;
	int ypos;

public:
	TransformComponent() {
		xpos = 0;
		ypos = 0;
	}

	TransformComponent(int x, int y) {
		xpos = x;
		ypos = y;
	}

	void update() override {
		xpos++;
		ypos++;
	}

	int x() { return xpos; }
	void x(int x) { xpos = x; }
	int y() { return ypos; }
	void y(int y) { xpos = y; }
	void setPos(int x, int y) {
		xpos = x;
		ypos = y;
	}
};