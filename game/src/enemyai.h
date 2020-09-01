#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"

class enemyai
{
public:
	enemyai();
	~enemyai();

	void initialise(engine::ref<engine::game_object> object, glm::vec3 position);
	void on_update(const engine::timestep& time_step, glm::vec3 position, bool hard, bool started);


	engine::ref<engine::game_object> object() const { return m_object; }

	void turn(float angle);
	//void update_camera(engine::perspective_camera& camera);

	//void jump();
	void attack();
	void jump();
	void idle();

private:
	float m_speed{ 0.f };
	float m_timer;
	glm::vec3 forward_towards_player;
	bool is_close = false;
	bool timeup = true;

	engine::ref<engine::game_object> m_object;
};
