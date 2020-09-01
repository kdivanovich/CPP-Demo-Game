#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "enemyai.h"
#include "pickup.h"

#include "glm/ext.hpp"
#include <string.h>

example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())


{

	// Hide the mouse and lock it inside the window
	//engine::input::anchor_mouse(true);
	engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::event, "bounce"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/DST-impuretechnology.mp3", engine::sound_type::track, "music");  // Royalty free music from http://www.nosoapradio.us/

	//custom sounds - all from Freesound
	m_audio_manager->load_sound("assets/audio/crouch.wav", engine::sound_type::event, "crouch");
	m_audio_manager->load_sound("assets/audio/pickup.wav", engine::sound_type::event, "pickup");
	m_audio_manager->load_sound("assets/audio/shoot.wav", engine::sound_type::event, "shoot");

	m_audio_manager->play("music");


	// Initialise the shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity",
		0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity",
		1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity",
		0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	std::vector<engine::ref<engine::texture_2d>> walls_textures = { engine::texture_2d::create("assets/textures/wall3.png", false) };
	std::vector<engine::ref<engine::texture_2d>> fur_textures = { engine::texture_2d::create("assets/textures/fur.bmp", false) };
	std::vector<engine::ref<engine::texture_2d>> metal_textures = { engine::texture_2d::create("assets/textures/metal.bmp", false) };
	std::vector<engine::ref<engine::texture_2d>> wood_textures = { engine::texture_2d::create("assets/textures/wood.png", false) };
	std::vector<engine::ref<engine::texture_2d>> enemy_textures = { engine::texture_2d::create("assets/textures/enemy.png", false) };

	m_lightsource_material = engine::material::create(1.f, m_pointLight.Color, m_pointLight.Color, m_pointLight.Color, .0f);

	// Skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
	m_skybox = engine::skybox::create(100.f,
		{
			engine::texture_2d::create("assets/textures/skybox/scorched_ft.jpg", true),
			engine::texture_2d::create("assets/textures/skybox/scorched_lf.jpg", true),
			engine::texture_2d::create("assets/textures/skybox/scorched_bk.jpg", true),
			engine::texture_2d::create("assets/textures/skybox/scorched_rt.jpg", true),
			engine::texture_2d::create("assets/textures/skybox/scorched_up.jpg", true),
			engine::texture_2d::create("assets/textures/skybox/scorched_dn.jpg", true)
		});

	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(3.0f, 0.5f, -5.0f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	m_mannequin = engine::game_object::create(mannequin_props);

	m_enemyai.initialise(m_mannequin, m_3d_camera.position());

	//mannequin2
	engine::ref<engine::skinned_mesh> m_skinned_mesh2 = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh2->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh2->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh2->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh2->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh2->switch_root_movement(false);

	engine::game_object_properties mannequin_props2;
	mannequin_props2.animated_mesh = m_skinned_mesh2;
	mannequin_props2.scale = glm::vec3(1.f / glm::max(m_skinned_mesh2->size().x, glm::max(m_skinned_mesh2->size().y, m_skinned_mesh2->size().z)));
	mannequin_props2.position = glm::vec3(1.0f, -2.5f, 0.0f);
	mannequin_props2.type = 0;
	mannequin_props2.bounding_shape = m_skinned_mesh2->size() / 2.f * mannequin_props2.scale.x;
	m_mannequin2 = engine::game_object::create(mannequin_props2);

	m_enemyai2.initialise(m_mannequin2, m_3d_camera.position());

	// Pickup
	engine::ref<engine::cuboid> pickup_shape = engine::cuboid::create(glm::vec3(0.5f), false);
	engine::ref<engine::texture_2d> pickup_texture = engine::texture_2d::create("assets/textures/medkit.jpg", true);
	engine::game_object_properties pickup_props;
	pickup_props.position = { 2.f, .8f, 1.f };
	pickup_props.meshes = { pickup_shape->mesh() };
	pickup_props.textures = { pickup_texture };
	m_pickup = pickup::create(pickup_props);
	m_pickup->init();

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/Terrain2.png", true) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	// Load the cow model. Create a cow object. Set its properties
	engine::ref <engine::model> cow_model = engine::model::create("assets/models/static/cow4.3ds");
	engine::game_object_properties cow_props;
	cow_props.meshes = cow_model->meshes();
	cow_props.textures = cow_model->textures();
	float cow_scale = 1.f / glm::max(cow_model->size().x, glm::max(cow_model->size().y, cow_model->size().z));
	cow_props.position = { -4.f,0.5f, -5.f };
	cow_props.scale = glm::vec3(cow_scale);
	cow_props.bounding_shape = cow_model->size() / 2.f * cow_scale;
	m_cow = engine::game_object::create(cow_props);

	// Load the tree model. Create a tree object. Set its properties
	engine::ref <engine::model> tree_model = engine::model::create("assets/models/static/elm.3ds");
	engine::game_object_properties tree_props;
	tree_props.meshes = tree_model->meshes();
	tree_props.textures = tree_model->textures();
	float tree_scale = 3.f / glm::max(tree_model->size().x, glm::max(tree_model->size().y, tree_model->size().z));
	tree_props.position = { 4.f, 0.5f, -5.f };
	tree_props.bounding_shape = tree_model->size() / 2.f * tree_scale;
	tree_props.scale = glm::vec3(tree_scale);
	m_tree = engine::game_object::create(tree_props);

	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.f, -3.f, -5.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.000001f;
	m_ball = engine::game_object::create(sphere_props);

	m_ballistic.initialise(engine::game_object::create(sphere_props));

	// ------------------------  CRYSTAL  -----------------------------------------------------------------

	// Load the tree 1 model. Create a crystal object. Set its properties
	engine::ref <engine::model> crystal_model = engine::model::create("assets/models/static/crystal.3ds");
	engine::game_object_properties crystal_model_props;
	crystal_model_props.meshes = crystal_model->meshes();
	crystal_model_props.textures = crystal_model->textures();
	float crystal_model_scale = 1.f / glm::max(crystal_model->size().x, glm::max(crystal_model->size().y, crystal_model->size().z));
	crystal_model_props.position = { 1.f, .5f, 3.f };
	crystal_model_props.scale = glm::vec3(crystal_model_scale);
	crystal_model_props.bounding_shape = crystal_model->size() / 2.f * crystal_model_scale;
	m_crystal = engine::game_object::create(crystal_model_props);

	// ------------------------  TREE 2  -----------------------------------------------------------------

	// Load the tree 2 model. Create a tree 2 object. Set its properties
	engine::ref <engine::model> tree2_model = engine::model::create("assets/models/static/tree2.3ds");
	engine::game_object_properties tree2_props;
	tree2_props.meshes = tree2_model->meshes();
	tree2_props.textures = tree2_model->textures();
	float tree2_scale = 1.f / glm::max(tree2_model->size().x, glm::max(tree2_model->size().y, tree2_model->size().z));
	tree2_props.position = { 2.f, .5f, 4.f };
	tree2_props.scale = glm::vec3(tree2_scale);
	tree2_props.bounding_shape = tree2_model->size() / 2.f * tree2_scale;
	m_tree2 = engine::game_object::create(tree2_props);

	// ------------------------  CARROT  -----------------------------------------------------------------

	// Load the carrot model. Create a carrot object. Set its properties
	engine::ref <engine::model> carrot_model = engine::model::create("assets/models/static/carrot.3ds");
	engine::game_object_properties carrot_props;
	carrot_props.meshes = carrot_model->meshes();
	carrot_props.textures = carrot_model->textures();
	float carrot_scale = 1.f / glm::max(carrot_model->size().x, glm::max(carrot_model->size().y, carrot_model->size().z));
	carrot_props.position = { 3.f, .5f, 5.f };
	carrot_props.scale = glm::vec3(carrot_scale);
	carrot_props.bounding_shape = carrot_model->size() / 2.f * carrot_scale;
	m_carrot = engine::game_object::create(carrot_props);

	// ------------------------  HOUSE  ------------------------------------------------------------------

	std::vector<glm::vec3> house_vertices;
	house_vertices.push_back(glm::vec3(-2.f, 2.f, 1.f));  //0
	house_vertices.push_back(glm::vec3(2.f, 2.f, 1.f));  //1
	house_vertices.push_back(glm::vec3(2.f, 0.f, 1.f));  //2
	house_vertices.push_back(glm::vec3(-2.f, 0.f, 1.f));  //3
	house_vertices.push_back(glm::vec3(-2.f, 0.f, -1.f));  //4
	house_vertices.push_back(glm::vec3(2.f, 0.f, -1.f));  //5
	house_vertices.push_back(glm::vec3(2.f, 2.f, -1.f));  //6
	house_vertices.push_back(glm::vec3(-2.f, 2.f, -1.f));  //7
	house_vertices.push_back(glm::vec3(0.f, 3.f, 0.f));  //8

	engine::ref<engine::house> house_shape = engine::house::create(house_vertices);
	engine::game_object_properties house_props;
	house_props.position = { 0.f, .5f, -12.f };
	house_props.meshes = { house_shape->mesh() };
	house_props.textures = walls_textures;
	m_house = engine::game_object::create(house_props);

	// ------------------------  CAT MESH  ---------------------------------------------------------------

	// Load the cat model. Create a cat object. Set its properties
	engine::ref <engine::model> cat_model = engine::model::create("assets/models/static/cat2.3ds");
	engine::game_object_properties cat_props;
	cat_props.meshes = cat_model->meshes();
	//cat_props.textures = cat_model->textures();	// default texture
	cat_props.textures = fur_textures;
	float cat_scale = 1.f / glm::max(cat_model->size().x, glm::max(cat_model->size().y, cat_model->size().z));
	cat_props.position = { -4.f,0.5f, -4.f };
	cat_props.scale = glm::vec3(cat_scale);
	cat_props.bounding_shape = cat_model->size() / 2.f * cat_scale;
	m_cat = engine::game_object::create(cat_props);

	// ------------------------  ENEMY  ------------------------------------------------------------------

	std::vector<glm::vec3> enemy_vertices;
	enemy_vertices.push_back(glm::vec3(-.15f, .6f, .15f));  //0
	enemy_vertices.push_back(glm::vec3(.15f, .6f, .15f));  //1
	enemy_vertices.push_back(glm::vec3(.15f, 0.f, .15f));  //2
	enemy_vertices.push_back(glm::vec3(-.15f, 0.f, .15f));  //3
	enemy_vertices.push_back(glm::vec3(-.15f, 0.f, -.15f));  //4
	enemy_vertices.push_back(glm::vec3(.15f, 0.f, -.15f));  //5
	enemy_vertices.push_back(glm::vec3(.15f, .6f, -.15f));  //6
	enemy_vertices.push_back(glm::vec3(-.15f, .6f, -.15f));  //7

	engine::ref<engine::enemy> enemy_shape = engine::enemy::create(enemy_vertices);
	engine::game_object_properties enemy_props;
	enemy_props.position = { 3.f,0.5f, 0.f };
	enemy_props.meshes = { enemy_shape->mesh() };
	enemy_props.textures = enemy_textures;
	m_enemy = engine::game_object::create(enemy_props);

	// ------------------------  CHAIR  -------------------------------------------------------------------

	std::vector<glm::vec3> chair_vertices;
	chair_vertices.push_back(glm::vec3(0.f, .1f, .1f));  //0
	chair_vertices.push_back(glm::vec3(.1f, 0.f, .1f));  //1
	chair_vertices.push_back(glm::vec3(-.1f, 0.f, .1f));  //2
	chair_vertices.push_back(glm::vec3(-.1f, 0.f, -0.1f));  //3
	chair_vertices.push_back(glm::vec3(0.f, .1f, -.1f));  //4
	chair_vertices.push_back(glm::vec3(.1f, 0.f, -0.1f));  //5

	chair_vertices.push_back(glm::vec3(.1f, .2f, .1f));   //6
	chair_vertices.push_back(glm::vec3(-.1f, .2f, .1f));   //7
	chair_vertices.push_back(glm::vec3(-.1f, .2f, -0.1f));  //8
	chair_vertices.push_back(glm::vec3(.1f, .2f, -0.1f));  //9

	engine::ref<engine::chair> chair_shape = engine::chair::create(chair_vertices);
	engine::game_object_properties chair_props;
	chair_props.position = { 0.f,0.5f, -1.f };
	chair_props.meshes = { chair_shape->mesh() };
	chair_props.textures = wood_textures;
	m_chair = engine::game_object::create(chair_props);

	// ------------------------  TABLE  ------------------------------------------------------------------

	std::vector<glm::vec3> table_vertices;
	table_vertices.push_back(glm::vec3(0.f, .2f, 0.f));   //0
	table_vertices.push_back(glm::vec3(-.2f, 0.f, .2f));  //1
	table_vertices.push_back(glm::vec3(.2f, 0.f, .2f));   //2 
	table_vertices.push_back(glm::vec3(.2f, 0.f, -.2f));  //3
	table_vertices.push_back(glm::vec3(-.2f, 0.f, -.2f)); //4

	table_vertices.push_back(glm::vec3(-.03f, .4f, .03f));	//5
	table_vertices.push_back(glm::vec3(.03f, .4f, .03f));	//6 
	table_vertices.push_back(glm::vec3(.03f, 0.f, .03f));	//7
	table_vertices.push_back(glm::vec3(-.03f, 0.f, .03f));	//8
	table_vertices.push_back(glm::vec3(-.03f, 0.f, -.03f));  //9
	table_vertices.push_back(glm::vec3(.03f, 0.f, -.03f));	//10
	table_vertices.push_back(glm::vec3(.03f, .4f, -.03f));	//11
	table_vertices.push_back(glm::vec3(-.03f, .4f, -.03f));  //12

	table_vertices.push_back(glm::vec3(-.5f, .45f, .23f));	//13
	table_vertices.push_back(glm::vec3(.5f, .45f, .23f));	//14
	table_vertices.push_back(glm::vec3(.5f, .4f, .23f));	//15
	table_vertices.push_back(glm::vec3(-.5f, .4f, .23f));	//16
	table_vertices.push_back(glm::vec3(-.5f, .4f, -.23f));  //17
	table_vertices.push_back(glm::vec3(.5f, .4f, -.23f));	//18
	table_vertices.push_back(glm::vec3(.5f, .45f, -.23f));	//19
	table_vertices.push_back(glm::vec3(-.5f, .45f, -.23f)); //20

	engine::ref<engine::table> table_shape = engine::table::create(table_vertices);
	engine::game_object_properties table_props;
	table_props.position = { 0.f,0.5f, -2.f };
	table_props.meshes = { table_shape->mesh() };
	table_props.textures = metal_textures;
	m_table = engine::game_object::create(table_props);
	// ---------------------------------------------------------------------------------------------------

	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_ball);
	//m_game_objects.push_back(m_cow);
	//m_game_objects.push_back(m_tree);
	//m_game_objects.push_back(m_pickup);
	m_game_objects.push_back(m_carrot);
	m_game_objects.push_back(m_tree2);
	m_game_objects.push_back(m_crystal);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(1);
	m_skinned_mesh2->switch_animation(1);

	m_projectile.initialise(m_ball);

	m_cross_fade = cross_fade::create(glm::vec3(1.f, 0.f, 0.f), 2.0f, 1.6f, 0.9f);
}

example_layer::~example_layer() {}


void example_layer::on_update(const engine::timestep& time_step)
{
	m_3d_camera.on_update(time_step);

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	//m_mannequin->animated_mesh()->on_update(time_step);
	m_enemyai.on_update(time_step, m_3d_camera.position(), is_hard, game_started);

	// shooting
	if (((m_mannequin->position().x < m_projectile.last_position().x + 0.5 && m_mannequin->position().x > m_projectile.last_position().x - 0.5) &&
		(m_mannequin->position().y < m_projectile.last_position().y + 1.5 && m_mannequin->position().y > m_projectile.last_position().y - 1.5) &&
		(m_mannequin->position().z < m_projectile.last_position().z + 0.5 && m_mannequin->position().z > m_projectile.last_position().z - 0.5)) && !game_over)
	{
		m_mannequin->set_position(glm::vec3(.0f, .5f, .0f));
		score++;
	}

	//m_mannequin2->animated_mesh()->on_update(time_step);

	m_enemyai2.on_update(time_step, m_3d_camera.position());

	if (((m_mannequin2->position().x < m_projectile.last_position().x + 0.5 && m_mannequin2->position().x > m_projectile.last_position().x - 0.5) &&
		(m_mannequin2->position().y < m_projectile.last_position().y + 1.5 && m_mannequin2->position().y > m_projectile.last_position().y - 1.5) &&
		(m_mannequin2->position().z < m_projectile.last_position().z + 0.5 && m_mannequin2->position().z > m_projectile.last_position().z - 0.5)) && !game_over)
	{
		m_mannequin2->set_position(glm::vec3(2.f, 0.5, -2.f));
		score++;
	}

	// HP drain upon contact
	if ((m_mannequin->position().z < m_3d_camera.position().z + 0.5  &&  m_mannequin->position().z >m_3d_camera.position().z - 0.5) &&
		(m_mannequin->position().x < m_3d_camera.position().x + 0.5 && m_mannequin->position().x >m_3d_camera.position().x - 0.5) && game_started)
	{
		hp -= 1;
		if (hp <= 0)
			game_over = true;
	}

	if ((m_mannequin2->position().z < m_3d_camera.position().z + 2.5 && m_mannequin2->position().z >m_3d_camera.position().z - 2.5) &&
		(m_mannequin2->position().x < m_3d_camera.position().x + 2.5 && m_mannequin2->position().x >m_3d_camera.position().x - 2.5) && game_started)
	{
		hp -= 1;
		if (hp <= 0)
			game_over = true;
	}

	

	m_pickup->update(m_3d_camera.position(), time_step);

	m_ballistic.on_update(time_step);

	m_projectile.on_update(time_step);

	//m_cross_fade->on_update(time_step);
	check_bounce();
}


void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();

	//const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
	//engine::renderer::begin_scene(m_3d_camera, textured_shader);

	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(textured_lighting_shader, m_terrain);

	engine::renderer::submit(textured_lighting_shader, m_house);

	engine::renderer::submit(textured_lighting_shader, m_cat);

	engine::renderer::submit(textured_lighting_shader, m_enemy);

	engine::renderer::submit(textured_lighting_shader, m_chair);

	engine::renderer::submit(textured_lighting_shader, m_table);

	m_projectile.on_render(textured_lighting_shader);


	glm::mat4 tree_transform(1.0f);
	tree_transform = glm::translate(tree_transform, glm::vec3(4.f, 0.5, -5.0f));
	tree_transform = glm::rotate(tree_transform, m_tree->rotation_amount(), m_tree->rotation_axis());
	tree_transform = glm::scale(tree_transform, m_tree->scale());
	engine::renderer::submit(textured_lighting_shader, tree_transform, m_tree);

	glm::mat4 cow_transform(1.0f);
	cow_transform = glm::translate(cow_transform, m_cow->position());
	cow_transform = glm::rotate(cow_transform, m_cow->rotation_amount(), m_cow->rotation_axis());
	cow_transform = glm::scale(cow_transform, m_cow->scale());
	engine::renderer::submit(textured_lighting_shader, cow_transform, m_cow);

	// pickup
	if (m_pickup->active()) {
		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_pickup->position());
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform = glm::scale(pickup_transform, m_pickup->scale() * 0.6f);
		engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform);

	}

	// working pick up effect - makes the player faster -- duck to reset it (left this way for debugging for now)
	if (!(m_pickup->active()) && !pickup_taken)
	{
		m_3d_camera.set_movement_speed(m_3d_camera.movement_speed() * 1.5f);
		hp += (100 - hp);
		pickup_taken = true;
		m_audio_manager->play("pickup");
	}


	//glm::vec3 p = m_mannequin->position();
	//glm::vec3 c = m_3d_camera.position();
	//glm::vec3 v = c - p;
	//float theta = atan2(v.x, v.z);
	//m_mannequin->set_rotation_amount(theta);

	//engine::renderer::submit(textured_lighting_shader, cow_transform, m_mannequin2);
	//m_mannequin2 = engine::game_object::create(mannequin_props2);


	// ------------------------  HOUSES  ----------------------------------------------------------------

	//// render of a regularly drawn house
	//glm::mat4 house_transform(1.0f);
	//house_transform = glm::translate(house_transform, m_house->position());
	//house_transform = glm::rotate(house_transform, m_house->rotation_amount(), m_house->rotation_axis());
	//house_transform = glm::scale(house_transform, m_house->scale());
	//engine::renderer::submit(textured_lighting_shader, house_transform, m_house);

	// ~45* rotation
	glm::mat4 house_transform2(1.0f);
	house_transform2 = glm::translate(house_transform2, glm::vec3(-10.f, 0.5f, -3.f));
	house_transform2 = glm::rotate(house_transform2, .7f, m_house->rotation_axis());
	house_transform2 = glm::scale(house_transform2, m_house->scale());
	engine::renderer::submit(textured_lighting_shader, house_transform2, m_house);

	// 90* rotation
	glm::mat4 house_transform3(1.0f);
	house_transform3 = glm::translate(house_transform3, glm::vec3(-12.f, 0.5f, 4.f));
	house_transform3 = glm::rotate(house_transform3, 1.571f, m_house->rotation_axis());
	house_transform3 = glm::scale(house_transform3, m_house->scale());
	engine::renderer::submit(textured_lighting_shader, house_transform3, m_house);

	// scaling demonstration
	glm::mat4 house_transform4(1.0f);
	house_transform4 = glm::translate(house_transform4, glm::vec3(-12.f, 0.5f, 9.f));
	house_transform4 = glm::rotate(house_transform4, 1.571f, m_house->rotation_axis());
	house_transform4 = glm::scale(house_transform4, m_house->scale() * 0.4f);
	engine::renderer::submit(textured_lighting_shader, house_transform4, m_house);

	// rotation alongside the Y axis,  NOTE: the bottom is not rendered as
	// this object will never be in that position in the final game
	glm::mat4 house_transform5(1.0f);
	house_transform5 = glm::translate(house_transform5, glm::vec3(-8.f, 1.5f, 12.f));
	house_transform5 = glm::rotate(house_transform5, -1.571f, glm::vec3(1.0f, 0.0f, 0.0f));
	house_transform5 = glm::scale(house_transform5, m_house->scale());
	engine::renderer::submit(textured_lighting_shader, house_transform5, m_house);

	// ---------------------------  CATS (imported meshes) ----------------------------------------

	glm::mat4 cat_transform(1.0f);
	cat_transform = glm::translate(cat_transform, glm::vec3(5.f, .5f, 3.f));
	cat_transform = glm::rotate(cat_transform, m_cat->rotation_amount(), m_cat->rotation_axis());
	cat_transform = glm::scale(cat_transform, m_cat->scale());
	engine::renderer::submit(textured_lighting_shader, cat_transform, m_cat);

	glm::mat4 cat_transformBig(1.0f);
	cat_transform = glm::translate(cat_transform, glm::vec3(-10.f, -4.0f, 100.f));
	cat_transform = glm::rotate(cat_transform, 2.9f, m_cat->rotation_axis());
	cat_transform = glm::scale(cat_transform, glm::vec3(31.0f, 31.0f, 31.0f));
	engine::renderer::submit(textured_lighting_shader, cat_transform, m_cat);

	// rotating alongside 2 axes
	glm::mat4 cat_transform2(1.0f);
	cat_transform2 = glm::translate(cat_transform2, glm::vec3(5.f, .6f, 0.f));
	cat_transform2 = glm::rotate(cat_transform2, 3.14f, glm::vec3(0.0f, 1.0f, 1.0f));
	cat_transform2 = glm::scale(cat_transform2, m_cat->scale());
	engine::renderer::submit(textured_lighting_shader, cat_transform2, m_cat);

	// stacking and rotating another cat onto the first
	glm::mat4 cat_transform3(1.0f);
	cat_transform3 = glm::translate(cat_transform3, glm::vec3(5.f, .85f, 3.f));
	cat_transform3 = glm::rotate(cat_transform3, 3.14f, m_cat->rotation_axis());
	cat_transform3 = glm::scale(cat_transform3, m_cat->scale());
	engine::renderer::submit(textured_lighting_shader, cat_transform3, m_cat);

	// rotating alongside all 3 axes (separately to allow for custom rotations)
	glm::mat4 cat_transform4(1.0f);
	cat_transform4 = glm::translate(cat_transform4, glm::vec3(5.f, .7f, 5.f));
	cat_transform4 = glm::rotate(cat_transform4, 1.57f, glm::vec3(0.0f, 0.0f, 1.0f));
	cat_transform4 = glm::rotate(cat_transform4, 1.2f, glm::vec3(0.0f, 1.0f, 0.0f));
	cat_transform4 = glm::rotate(cat_transform4, 1.49f, glm::vec3(1.0f, 0.0f, 0.0f));
	cat_transform4 = glm::scale(cat_transform4, m_cat->scale());
	engine::renderer::submit(textured_lighting_shader, cat_transform4, m_cat);


	// ------------------------  CRYSTAL (imported meshes) ----------------------------------------

	glm::mat4 crystal_transform(1.0f);
	crystal_transform = glm::translate(crystal_transform, glm::vec3(5.f, 0.5f, 10.f));
	crystal_transform = glm::rotate(crystal_transform, -1.571f, m_crystal->rotation_axis());
	crystal_transform = glm::scale(crystal_transform, m_crystal->scale());
	engine::renderer::submit(textured_lighting_shader, crystal_transform, m_crystal);

	glm::mat4 crystal_transform1(1.0f);
	crystal_transform1 = glm::translate(crystal_transform1, glm::vec3(5.f, 0.3f, 11.f));
	crystal_transform1 = glm::rotate(crystal_transform1, -1.671f, m_crystal->rotation_axis());
	crystal_transform1 = glm::scale(crystal_transform1, m_crystal->scale());
	engine::renderer::submit(textured_lighting_shader, crystal_transform1, m_crystal);


	// ------------------------  TREE 2 (imported meshes) ----------------------------------------

	glm::mat4 tree2_transform(1.0f);
	tree2_transform = glm::translate(tree2_transform, glm::vec3(4.f, 0.45f, 7.f));
	tree2_transform = glm::rotate(tree2_transform, -1.371f, m_tree2->rotation_axis());
	tree2_transform = glm::scale(tree2_transform, m_tree2->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, tree2_transform, m_tree2);



	// ------------------------  CARROT (imported meshes) ----------------------------------------

	glm::mat4 carrot_transform(1.0f);
	carrot_transform = glm::translate(carrot_transform, glm::vec3(4.f, 0.3f, 6.f));
	carrot_transform = glm::rotate(carrot_transform, -1.571f, m_carrot->rotation_axis());
	carrot_transform = glm::scale(carrot_transform, m_carrot->scale());
	engine::renderer::submit(textured_lighting_shader, carrot_transform, m_carrot);

	// ----------------------------------------------------------------------------------------------------

	engine::renderer::end_scene();

	// Set up material shader. (does not render textures, renders materials instead)
	const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
	engine::renderer::begin_scene(m_3d_camera, material_shader);

	m_material->submit(material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	//ballistic
	m_material->submit(material_shader);
	m_ballistic.on_render(material_shader);

	engine::renderer::submit(material_shader, m_ball);
	engine::renderer::submit(material_shader, m_enemy);	// uncomment this and use textures
	engine::renderer::submit(material_shader, m_chair);
	engine::renderer::submit(material_shader, m_table);

	// standard render - look above for the manual SoRT rendering as per requirement
	//engine::renderer::submit(material_shader, m_house);

	engine::renderer::end_scene();

	const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
	engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	glm::mat4 aniTransform = glm::mat4(1.0f);


	if (game_started)
	{

		//engine::renderer::submit(animated_mesh_shader, m_mannequin);
		engine::renderer::submit(animated_mesh_shader, m_enemyai.object());

		//engine::renderer::submit(animated_mesh_shader, m_mannequin2);
		engine::renderer::submit(animated_mesh_shader, m_enemyai2.object());

	}


	m_cross_fade->on_render(material_shader);

	engine::renderer::end_scene();


	// for debugging ---------------------------------------------------------
	//const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	//std::string coords = ">>> " + std::to_string(m_3d_camera.position.x()) + ", " + m_3d_camera.position.y + ", " + m_3d_camera.position.z ;

	//// Current title 
	//m_text_manager->render_text(text_shader, coords,
	//	(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 600,
	//	(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 + 100,
	//	1.7f,
	//	glm::vec4(.7f, 0.1f, 0.3f, 1.f));
	// -----------------------------------------------------------------------

	

	// Start screen 
	if (!game_started)
	{

			const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

			// Current title 
			m_text_manager->render_text(text_shader, "Deserted cats and weird houses!",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 600,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 + 100,
				1.7f,
				glm::vec4(.7f, 0.1f, 0.3f, 1.f));

			// "Start" button prompt
			m_text_manager->render_text(text_shader, "Press ENTER to start",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 90,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 60,
				.5f,
				glm::vec4(.4f, 0.2f, 0.9f, 1.f));

			// Controls
			m_text_manager->render_text(text_shader, "Controls:",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 20,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 140,
				.5f,
				glm::vec4(.6f, 0.6f, 0.1f, 1.f));

			m_text_manager->render_text(text_shader, "W/A/S/D - move",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 45,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 170,
				.5f,
				glm::vec4(.6f, 0.6f, 0.1f, 1.f));

			m_text_manager->render_text(text_shader, "Space - shoot",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 35,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 200,
				.5f,
				glm::vec4(.6f, 0.6f, 0.1f, 1.f));

			m_text_manager->render_text(text_shader, "(1 - straight projectile -- in progress)",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 135,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 230,
				.5f,
				glm::vec4(.6f, 0.6f, 0.1f, 1.f));


			m_text_manager->render_text(text_shader, "C - crouch",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 25,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 260,
				.5f,
				glm::vec4(.6f, 0.6f, 0.1f, 1.f));


			m_text_manager->render_text(text_shader, "Esc - Exit",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 22,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 290,
				.5f,
				glm::vec4(.6f, 0.6f, 0.1f, 1.f));

			m_text_manager->render_text(text_shader, "Tip: Pick up the rotating bonus for HP restore and bonus speed",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 290,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 320,
				.5f,
				glm::vec4(.3f, 0.9f, 0.6f, 1.f));

			m_text_manager->render_text(text_shader, "(Known bug: waiting at the title screen before starting freezes the shooting projectile)",
				(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 390,
				(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 350,
				.5f,
				glm::vec4(.8f, 0.2f, 0.2f, 0.9f));
	}

	else if (game_over )
	{
		const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
		m_text_manager->render_text(text_shader, "Game Over",
			(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 250,
			(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 + 100,
			1.7f,
			glm::vec4(.7f, 0.1f, 0.3f, 1.f));

		m_text_manager->render_text(text_shader, "Your Score: " + std::to_string(score),
			(float)engine::application::window().width() - ((float)engine::application::window().width()) / 2 - 210,
			(float)engine::application::window().height() - ((float)engine::application::window().height()) / 2 - 100,
			1.2f,
			glm::vec4(.7f, 0.1f, 0.3f, 1.f));

		m_3d_camera.set_movement_speed(0.f);
	}

	else
	{
		// HUD
		const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
		m_text_manager->render_text(text_shader, ("Speed: " + std::to_string(m_3d_camera.movement_speed())),
			10.f,
			(float)engine::application::window().height() - 25.f, 0.5f,
			glm::vec4(.6f, 0.6f, 0.1f, 1.f));

		m_text_manager->render_text(text_shader, ("HP: " + std::to_string(hp)), 10.f, (float)engine::application::window().height() - 50.f, 0.5f, glm::vec4(.6f, 0.6f, 0.1f, 1.f));

		m_text_manager->render_text(text_shader, "Score: " + std::to_string(score), 10.f, (float)engine::application::window().height() - 95.f, 0.5f, glm::vec4(.6f, 0.6f, 0.1f, 1.f));

	}
}


void example_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
		if (e.key_code() == engine::key_codes::KEY_ENTER)
		{
			game_started = true;
		}
		if (e.key_code() == engine::key_codes::KEY_H)
		{
			game_started = true;
			is_hard = true;
		}
		if (e.key_code() == engine::key_codes::KEY_1)
		{
			m_ballistic.fire(m_3d_camera, 22.0f);
			m_audio_manager->play("shoot");
		}
		if (e.key_code() == engine::key_codes::KEY_SPACE)
		{

			if (m_3d_camera.z < 0.1)
			{
				m_3d_camera.set_view_matrix(glm::vec3(m_3d_camera.x, m_3d_camera.y, m_3d_camera.z),		// TODO: bug - waiting for some time before starting
											glm::vec3((m_3d_camera.x, m_3d_camera.y, m_3d_camera.z)));	// and shooting in the beginning stops ball in mid air				
			}
			m_projectile.kick(m_3d_camera, 270.0f);
			m_audio_manager->play("shoot");

			//m_projectile.kick(m_3d_camera, 270.0f);
			//m_audio_manager->play("shoot");
		}
		if (e.key_code() == engine::key_codes::KEY_C)
		{
			m_audio_manager->play("crouch");
		}
	}
}


void example_layer::check_bounce()
{
	//if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
		//m_audio_manager->play("bounce");
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}
