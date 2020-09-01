#include "enemyai2.h"

enemyai2::enemyai2() : m_timer(0.0f), m_speed(.4f) {}
enemyai2::~enemyai2() {}

void enemyai2::initialise(engine::ref<engine::game_object> object, glm::vec3 position)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, 1.f));
	m_object->set_position(glm::vec3(2.f, 0.5, -4.f));
	m_object->animated_mesh()->set_default_animation(1);
}

void enemyai2::on_update(const engine::timestep& time_step, glm::vec3 position)
{
	glm::vec3 p = m_object->position();
	position.y = 0.0f;
	glm::vec3 c = position;
	glm::vec3 v = c - p;
	float theta = atan2(v.x, v.z);
	m_object->set_rotation_amount(theta);


	glm::vec3 forward_towards_player = {  position.x - m_object->position().x, 0.f, position.z - m_object->position().z };

	/*
	Note: original line below was abs(forward_towards_player.x) < 1.f
	seems abs modifies the value in place?
	with abs when x = it would go away from the player, to look further into it
	and make the below conditions more robust
	*/

	if (((forward_towards_player.x) < 2.f && (forward_towards_player.x) > -2.f) &&
		((forward_towards_player.z) < 2.f && (forward_towards_player.z) > -2.f))
	{
		m_object->set_position(m_object->position() -= forward_towards_player * m_speed * (float)time_step);
	}
	else 
	{
		m_object->set_position(m_object->position() += forward_towards_player * m_speed * (float)time_step);
	}




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

void enemyai2::jump()
{

	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(3);
	m_speed = 0.0f;

	m_timer = m_object->animated_mesh()->animations().at(3)->mDuration;
}

void enemyai2::idle()
{

	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(2);
	m_speed = 0.0f;

	m_timer = m_object->animated_mesh()->animations().at(3)->mDuration;
}
