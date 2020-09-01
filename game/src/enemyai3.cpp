#include "enemyai3.h"

enemyai3::enemyai3() : m_timer(0.0f), m_speed(.4f) {}
enemyai3::~enemyai3() {}

void enemyai3::initialise(engine::ref<engine::game_object> object, glm::vec3 position)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, 1.f));
	m_object->set_position(glm::vec3(0.f, 0.5, -2.f));
	m_object->animated_mesh()->set_default_animation(1);
}

void enemyai3::on_update(const engine::timestep& time_step, glm::vec3 position)
{
	glm::vec3 p = m_object->position();
	position.y = 0.0f;
	glm::vec3 c = position;
	glm::vec3 v = c - p;
	float theta = atan2(v.x, v.z);
	m_object->set_rotation_amount(theta);

	glm::vec3 forward_towards_player = { position.x - m_object->position().x, 0.f, position.z - m_object->position().z };
	m_object->set_position(m_object->position() += forward_towards_player * m_speed * (float)time_step);	// comment out if AI 2 is ON


	// AI 2 - approaches to no more than 2m
	if (abs(forward_towards_player.z) < 2.f)
	{
		m_object->set_position(m_object->position() -= forward_towards_player * m_speed * (float)time_step);
	}
	else if (abs(forward_towards_player.z) >= 2.f)
	{
		m_object->set_position(m_object->position() += forward_towards_player * m_speed * (float)time_step);
	}

	// AI 3 - jumps if too close
	//if (abs(forward_towards_player.z) < 1.f)
	//{
	//	jump();
	//}




	m_object->animated_mesh()->on_update(time_step);

	if (m_timer > 0.0f)
	{
		m_timer -= (float)time_step;
		if (m_timer < 0.0f)
		{
			m_object->animated_mesh()->switch_root_movement(false);
			m_object->animated_mesh()->switch_animation(m_object->animated_mesh()->default_animation());
			m_speed = .5f;
		}
	}
}

void enemyai3::jump()
{

	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(3);
	m_speed = 0.0f;

	m_timer = m_object->animated_mesh()->animations().at(3)->mDuration;
}

void enemyai3::idle()
{

	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(2);
	m_speed = 0.0f;

	m_timer = m_object->animated_mesh()->animations().at(3)->mDuration;
}
