#pragma once
#include <engine.h>
#include "cross_fade.h"
#include "enemyai.h"
#include "enemyai2.h"
#include "ballistic.h"
#include "projectile.h"
#include "pickup.h"

class pickup;

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:
	void check_bounce();
	void hexagon_shape();

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_cow{};
	engine::ref<engine::game_object>	m_cat{};
	engine::ref<engine::game_object>	m_tree{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_ball1{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_mannequin2{};
	engine::ref<engine::game_object>	m_mannequin3{};
	engine::ref<engine::game_object>	m_house{};
	engine::ref<engine::game_object>	m_enemy{};
	engine::ref<engine::game_object>	m_chair{};
	engine::ref<engine::game_object>	m_table{};
	engine::ref<engine::game_object>	m_crystal{};
	engine::ref<engine::game_object>	m_tree2{};
	engine::ref<engine::game_object>	m_carrot{};

	enemyai								m_enemyai{};
	enemyai2							m_enemyai2{};

	engine::ref<engine::material>		m_lightsource_material{};
	engine::PointLight					m_pointLight;
	engine::ref<engine::material>		m_ballistic_material{};
	ballistic							m_ballistic;
	ballistic							m_ballistic1;

	projectile m_projectile;
	
	engine::ref<engine::material>		m_material{};

	engine::DirectionalLight            m_directionalLight;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};
	engine::ref<cross_fade>				m_cross_fade{};

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;
	engine::ref<pickup>				  m_pickup{};

	bool							  game_started = false;
	bool							  pickup_taken = false;
	bool							  game_over = false;

	bool							  is_hard = false;
	int								  score = 0;		
	int								  hp = 100;

	glm::vec3 temppp;
};
