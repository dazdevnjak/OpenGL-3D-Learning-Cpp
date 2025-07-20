#pragma once
class GameObject;

class Component {
public:
	GameObject* owner = nullptr;
	virtual ~Component() = default;
	virtual void update(float delta_time){}
	virtual void render(){}
	virtual void init() = 0;
};

