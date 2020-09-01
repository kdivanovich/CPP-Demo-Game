#include "pch.h"
#include "enemy.h"
#include <engine.h>	

engine::enemy::enemy(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;

	// front normal 1:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(1)));
	// front normal 2:
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(3), vertices.at(1) - vertices.at(2)));
	
	// left normal 1:
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(4), vertices.at(7) - vertices.at(0)));
	// left normal 2:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(3)));
	
	// right normal 1:
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) - vertices.at(6)));
	// right normal 2:
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(2), vertices.at(6) - vertices.at(5)));
	
	// back normal 1:
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(5), vertices.at(6) - vertices.at(7)));
	// back normal 2:
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(5), vertices.at(7) - vertices.at(4)));

	// top 1
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(7), vertices.at(6) - vertices.at(0)));

	// top 2
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(0), vertices.at(6) - vertices.at(1)));


	std::vector<mesh::vertex> enemy_vertices
	{
		//front 1
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.f,   1.f } },
		{ vertices.at(3),		normals.at(0),		{ 0.f,   0.f } },
		{ vertices.at(1),		normals.at(0),		{ .5f,   1.f } },

		//front 2   
		{ vertices.at(1),		normals.at(1),		{ .5f,   1.f } },
		{ vertices.at(3),		normals.at(1),		{ 0.f,   0.f } },
		{ vertices.at(2),		normals.at(1),		{ .5f,   0.f } },


		//left 1
		{ vertices.at(7),		normals.at(2),		{ 0.5f,  1.f } },
		{ vertices.at(4),		normals.at(2),		{ 0.5f,  0.f } },
		{ vertices.at(0),		normals.at(2),		{ 1.f,   1.f } },
										   
		//left 2						   
		{ vertices.at(0),		normals.at(3),		{ 1.f,   1.f } },
		{ vertices.at(4),		normals.at(3),		{ 0.5f,  0.f } },
		{ vertices.at(3),		normals.at(3),		{ 1.f,   0.f } },


		//right 1
		{ vertices.at(1),		normals.at(4),		{ 0.5f,  1.f } },
		{ vertices.at(2),		normals.at(4),		{ 0.5f,  0.f } },
		{ vertices.at(6),		normals.at(4),		{ 1.f,   1.f } },
										   
		//right 2						  
		{ vertices.at(6),		normals.at(5),		{ 1.f,   1.f } },
		{ vertices.at(2),		normals.at(5),		{ 0.5f,  0.f } },
		{ vertices.at(5),		normals.at(5),		{ 1.f,   0.f } },


		//back 1
		{ vertices.at(6),		normals.at(6),		{ 0.5f,  1.f } },
		{ vertices.at(5),		normals.at(6),		{ 0.5f,  0.f } },
		{ vertices.at(7),		normals.at(6),		{ 1.f,   1.f } },
										   
		//back 2						   
		{ vertices.at(7),		normals.at(7),		{ 1.f,   1.f } },
		{ vertices.at(5),		normals.at(7),		{ 0.5f,  0.f } },
		{ vertices.at(4),		normals.at(7),		{ 1.f,   0.f } },


		//roof front
		{ vertices.at(6),		normals.at(8),		{ 0.25f, 0.5f } },
		{ vertices.at(7),		normals.at(8),		{ 0.0f,   0.f } },
		{ vertices.at(0),		normals.at(8),		{ 0.5f,   0.f } },

		//roof left
		{ vertices.at(6),		normals.at(9),		{ 0.25f,  0.5f } },
		{ vertices.at(0),		normals.at(9),		{  0.0f,   0.f } },
		{ vertices.at(1),		normals.at(9),		{  0.5f,   0.f } },

	};

	const std::vector<uint32_t> enemy_indices
	{
		0,1,2,		3,4,5,		// front
		6,7,8,		9,10,11,	// left 
		12,13,14,	15,16,17,	// right
		18,19,20,	21,22,23,	// back
		24,25,26,				// roof 1
		27,28,29,				// roof 2
	};

	m_mesh = engine::mesh::create(enemy_vertices, enemy_indices);
}

engine::enemy::~enemy() {}

engine::ref<engine::enemy> engine::enemy::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::enemy>(vertices);
}
