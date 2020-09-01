#include "pch.h"
#include "chair.h"
#include <engine.h>	

engine::chair::chair(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;

	//bottom
	// front normal:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));

	// left normal 1:
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(3), vertices.at(4) - vertices.at(0)));
	// left normal 2:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));

	// back normal 1:
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(5), vertices.at(4) - vertices.at(3)));

	// right normal:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
	// right normal 2:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));

	//top
	// front normal:
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(7), vertices.at(6) - vertices.at(0)));

	// left normal 1:
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(4), vertices.at(7) - vertices.at(0)));
	// left normal 2:
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(8), vertices.at(7) - vertices.at(4)));

	// back normal 1:
	normals.push_back(glm::cross(vertices.at(8) - vertices.at(9), vertices.at(8) - vertices.at(4)));

	// right normal:
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(0), vertices.at(6) - vertices.at(9)));
	// right normal 2:
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(0), vertices.at(9) - vertices.at(4)));

	// top normal:
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(7), vertices.at(9) - vertices.at(6)));
	// top normal 2:
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(8), vertices.at(9) - vertices.at(7)));

	std::vector<mesh::vertex> chair_vertices
	{
		//bottom
		//front
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(0),		{ 1.0f,  1.f } },
		{ vertices.at(2),		normals.at(0),		{ 0.0f,  0.f } },
		{ vertices.at(1),		normals.at(0),		{ 1.f,   0.f } },
		
		//left 1
		//  position			normal			      tex coord       
		{ vertices.at(4),		normals.at(1),		{ 1.0f,  1.f } },
		{ vertices.at(3),		normals.at(1),		{ 0.0f,  0.f } },
		{ vertices.at(0),		normals.at(1),		{ 1.f,   0.f } },
		//left 2
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(2),		{ 1.0f,  1.f } },
		{ vertices.at(3),		normals.at(2),		{ 0.0f,  1.f } },
		{ vertices.at(2),		normals.at(2),		{ 0.0f,  0.f } },
		
		//back
		//  position			normal			      tex coord       
		{ vertices.at(4),		normals.at(3),		{ 1.0f,  1.f } },
		{ vertices.at(5),		normals.at(3),		{ 0.0f,  1.f } },
		{ vertices.at(3),		normals.at(3),		{ 0.0f,  0.f } },
		
		//right 1
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(4),		{ 0.0f,  1.f } },
		{ vertices.at(1),		normals.at(4),		{ 0.0f,  0.f } },
		{ vertices.at(4),		normals.at(4),		{ 1.f,   1.f } },
		//right 2
		//  position			normal			      tex coord       
		{ vertices.at(1),		normals.at(5),		{ 0.0f,  0.f } },
		{ vertices.at(5),		normals.at(5),		{ 1.0f,  0.f } },
		{ vertices.at(4),		normals.at(5),		{ 1.0f,  1.f } },

		//top
		//front
		//  position			normal			      tex coord       
		{ vertices.at(6),		normals.at(6),		{ 1.0f,  1.f } },
		{ vertices.at(7),		normals.at(6),		{ 0.0f,  0.f } },
		{ vertices.at(0),		normals.at(6),		{ 1.f,   0.f } },

		//left 1
		//  position			normal			      tex coord       
		{ vertices.at(7),		normals.at(7),		{ 1.0f,  1.f } },
		{ vertices.at(4),		normals.at(7),		{ 0.0f,  0.f } },
		{ vertices.at(0),		normals.at(7),		{ 1.f,   0.f } },
		//left 2									  
		//  position			normal			      tex coord       
		{ vertices.at(7),		normals.at(8),		{ 1.0f,  1.f } },
		{ vertices.at(8),		normals.at(8),		{ 0.0f,  1.f } },
		{ vertices.at(4),		normals.at(8),		{ 0.0f,  0.f } },

		//back
		//  position			normal			      tex coord       
		{ vertices.at(8),		normals.at(9),		{ 1.0f,  1.f } },
		{ vertices.at(9),		normals.at(9),		{ 0.0f,  1.f } },
		{ vertices.at(4),		normals.at(9),		{ 0.0f,  0.f } },
		
		//right 1
		//  position			normal			      tex coord       
		{ vertices.at(6),		normals.at(10),		{ 1.0f,  1.f } },
		{ vertices.at(0),		normals.at(10),		{ 0.0f,  0.f } },
		{ vertices.at(9),		normals.at(10),		{ 1.f,   0.f } },
		//right 2
		//  position			normal			      tex coord       
		{ vertices.at(9),		normals.at(11),		{ 1.0f,  1.f } },
		{ vertices.at(0),		normals.at(11),		{ 0.0f,  1.f } },
		{ vertices.at(4),		normals.at(11),		{ 0.0f,  0.f } },

		//top 1
		//  position			normal			      tex coord     
		{ vertices.at(9),		normals.at(12),		{ 1.0f,  1.f } },
		{ vertices.at(7),		normals.at(12),		{ 0.0f,  0.f } },
		{ vertices.at(6),		normals.at(12),		{ 1.f,   0.f } },
		//top 2
		//  position			normal			      tex coord     
		{ vertices.at(9),		normals.at(13),		{ 1.0f,  1.f } },
		{ vertices.at(8),		normals.at(13),		{ 0.0f,  1.f } },
		{ vertices.at(7),		normals.at(13),		{ 0.0f,  0.f } },
	};

	const std::vector<uint32_t> chair_indices
	{
		0,1,2,					// front
		3,4,5,	6,7,8,			// left
		9,10,11,				// back
		12,13,14,	15,16,17,	// right

		18, 19,20,
		21, 22, 23,  24, 25, 26,
		27, 28, 29,
		30, 31, 32,  33, 34, 35,
		36, 37, 38,  39, 40, 41,
	};

	m_mesh = engine::mesh::create(chair_vertices, chair_indices);
}

engine::chair::~chair() {}

engine::ref<engine::chair> engine::chair::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::chair>(vertices);
}
