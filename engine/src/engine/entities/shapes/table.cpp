#include "pch.h"
#include "table.h"
#include <engine.h>	

engine::table::table(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;

	// bottom
	// front normal:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(2)));
	// left normal:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(1)));
	// back normal:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(4)));
	// right normal:
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(3)));

	// mid
	// front normal:
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(8), vertices.at(6) - vertices.at(7)));
	// front normal:
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(5), vertices.at(6) - vertices.at(8)));
	// left normal:
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(9), vertices.at(5) - vertices.at(8)));
	// left normal:
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(12), vertices.at(5) - vertices.at(9)));
	// back normal:
	normals.push_back(glm::cross(vertices.at(12) - vertices.at(10), vertices.at(12) - vertices.at(9)));
	// back normal:
	normals.push_back(glm::cross(vertices.at(12) - vertices.at(11), vertices.at(12) - vertices.at(10)));
	// right normal:
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(6), vertices.at(11) - vertices.at(7)));
	// right normal:
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(7), vertices.at(11) - vertices.at(10)));

	// top
	// front normal:
	normals.push_back(glm::cross(vertices.at(14) - vertices.at(16), vertices.at(14) - vertices.at(15)));
	// front normal:
	normals.push_back(glm::cross(vertices.at(14) - vertices.at(13), vertices.at(14) - vertices.at(16)));
	// left normal:
	normals.push_back(glm::cross(vertices.at(13) - vertices.at(17), vertices.at(13) - vertices.at(16)));
	// left normal:
	normals.push_back(glm::cross(vertices.at(13) - vertices.at(20), vertices.at(13) - vertices.at(17)));
	// back normal:
	normals.push_back(glm::cross(vertices.at(20) - vertices.at(18), vertices.at(20) - vertices.at(17)));
	// back normal:
	normals.push_back(glm::cross(vertices.at(20) - vertices.at(19), vertices.at(20) - vertices.at(18)));
	// right normal:
	normals.push_back(glm::cross(vertices.at(19) - vertices.at(14), vertices.at(19) - vertices.at(15)));
	// right normal:
	normals.push_back(glm::cross(vertices.at(19) - vertices.at(15), vertices.at(19) - vertices.at(18)));
	// top normal:
	normals.push_back(glm::cross(vertices.at(19) - vertices.at(13), vertices.at(19) - vertices.at(14)));
	// top normal:
	normals.push_back(glm::cross(vertices.at(19) - vertices.at(20), vertices.at(19) - vertices.at(13)));
	// bottom normal:
	normals.push_back(glm::cross(vertices.at(15) - vertices.at(16), vertices.at(15) - vertices.at(17)));
	// bottom normal:
	normals.push_back(glm::cross(vertices.at(15) - vertices.at(17), vertices.at(15) - vertices.at(18)));

	std::vector<mesh::vertex> table_vertices
	{
		// BOTTOM
		//front
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.5f,  1.f } },
		{ vertices.at(1),		normals.at(0),		{ 0.5f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.f,   1.f } },
		
		//left 
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(1),		{ 0.5f,  1.f } },
		{ vertices.at(4),		normals.at(1),		{ 0.5f,  0.f } },
		{ vertices.at(1),		normals.at(1),		{ 1.f,   1.f } },
		//left 2
		
		//back
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(2),		{ 0.5f,  1.f } },
		{ vertices.at(3),		normals.at(2),		{ 0.5f,  0.f } },
		{ vertices.at(4),		normals.at(2),		{ 1.f,   1.f } },
		
		//right 
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(3),		{ 0.5f,  1.f } },
		{ vertices.at(2),		normals.at(3),		{ 0.5f,  0.f } },
		{ vertices.at(3),		normals.at(3),		{ 1.f,   1.f } },


		// MIDDLE
		//front
		//  position			normal			      tex coord       
		{ vertices.at(6),		normals.at(4),		{ 1.0f,  1.f } },
		{ vertices.at(8),		normals.at(4),		{ 0.0f,  0.f } },
		{ vertices.at(7),		normals.at(4),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(6),		normals.at(5),		{ 1.0f,  1.f } },
		{ vertices.at(5),		normals.at(5),		{ 0.0f,  1.f } },
		{ vertices.at(8),		normals.at(5),		{ 0.f,   0.f } },

		//left
		//  position			normal			      tex coord       
		{ vertices.at(5),		normals.at(6),		{ 1.0f,  1.f } },
		{ vertices.at(9),		normals.at(6),		{ 0.0f,  0.f } },
		{ vertices.at(8),		normals.at(6),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(5),		normals.at(7),		{ 1.0f,  1.f } },
		{ vertices.at(12),		normals.at(7),		{ 0.0f,  1.f } },
		{ vertices.at(9),		normals.at(7),		{ 0.f,   0.f } },

		//back
		//  position			normal			      tex coord       
		{ vertices.at(12),		normals.at(8),		{ 1.0f,  1.f } },
		{ vertices.at(10),		normals.at(8),		{ 0.0f,  0.f } },
		{ vertices.at(9),		normals.at(8),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(12),		normals.at(9),		{ 1.0f,  1.f } },
		{ vertices.at(11),		normals.at(9),		{ 0.0f,  1.f } },
		{ vertices.at(10),		normals.at(9),		{ 0.f,   0.f } },

		//right
		//  position			normal			      tex coord       
		{ vertices.at(11),		normals.at(10),		{ 1.0f,  1.f } },
		{ vertices.at(6),		normals.at(10),		{ 0.0f,  0.f } },
		{ vertices.at(7),		normals.at(10),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(11),		normals.at(11),		{ 1.0f,  1.f } },
		{ vertices.at(7),		normals.at(11),		{ 0.0f,  1.f } },
		{ vertices.at(10),		normals.at(11),		{ 0.f,   0.f } },


		// TOP
		//front
		//  position			normal			      tex coord       
		{ vertices.at(14),		normals.at(12),		{ 1.0f,  1.f } },
		{ vertices.at(16),		normals.at(12),		{ 0.0f,  0.f } },
		{ vertices.at(15),		normals.at(12),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(14),		normals.at(13),		{ 1.0f,  1.f } },
		{ vertices.at(13),		normals.at(13),		{ 0.0f,  1.f } },
		{ vertices.at(16),		normals.at(13),		{ 0.f,   0.f } },
		
		//left
		//  position			normal			      tex coord       
		{ vertices.at(13),		normals.at(14),		{ 1.0f,  1.f } },
		{ vertices.at(17),		normals.at(14),		{ 0.0f,  0.f } },
		{ vertices.at(16),		normals.at(14),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(13),		normals.at(15),		{ 1.0f,  1.f } },
		{ vertices.at(20),		normals.at(15),		{ 0.0f,  1.f } },
		{ vertices.at(17),		normals.at(15),		{ 0.f,   0.f } },
		
		//back
		//  position			normal			      tex coord       
		{ vertices.at(20),		normals.at(16),		{ 1.0f,  1.f } },
		{ vertices.at(18),		normals.at(16),		{ 0.0f,  0.f } },
		{ vertices.at(17),		normals.at(16),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(20),		normals.at(17),		{ 1.0f,  1.f } },
		{ vertices.at(19),		normals.at(17),		{ 0.0f,  1.f } },
		{ vertices.at(18),		normals.at(17),		{ 0.f,   0.f } },
		
		//right
		//  position			normal			      tex coord       
		{ vertices.at(19),		normals.at(18),		{ 1.0f,  1.f } },
		{ vertices.at(14),		normals.at(18),		{ 0.0f,  0.f } },
		{ vertices.at(15),		normals.at(18),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(19),		normals.at(19),		{ 1.0f,  1.f } },
		{ vertices.at(15),		normals.at(19),		{ 0.0f,  1.f } },
		{ vertices.at(18),		normals.at(19),		{ 0.f,   0.f } },
			
		//top of top
		//  position			normal			      tex coord       
		{ vertices.at(19),		normals.at(18),		{ 1.0f,  1.f } },
		{ vertices.at(14),		normals.at(18),		{ 0.0f,  0.f } },
		{ vertices.at(15),		normals.at(18),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(19),		normals.at(19),		{ 1.0f,  1.f } },
		{ vertices.at(15),		normals.at(19),		{ 0.0f,  1.f } },
		{ vertices.at(18),		normals.at(19),		{ 0.f,   0.f } },
				
		//top
		//  position			normal			      tex coord       
		{ vertices.at(19),		normals.at(20),		{ 1.0f,  1.f } },
		{ vertices.at(13),		normals.at(20),		{ 0.0f,  0.f } },
		{ vertices.at(14),		normals.at(20),		{ 1.f,   0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(19),		normals.at(21),		{ 1.0f,  1.f } },
		{ vertices.at(20),		normals.at(21),		{ 0.0f,  1.f } },
		{ vertices.at(13),		normals.at(21),		{ 0.f,   0.f } },

		//bottom of top
		//  position			normal			      tex coord       
		{ vertices.at(15),		normals.at(22),		{ 1.f,   0.f } },
		{ vertices.at(16),		normals.at(22),		{ 1.0f,  1.f } },
		{ vertices.at(17),		normals.at(22),		{  0.0f, 0.f } },
		//  position			normal			      tex coord       
		{ vertices.at(15),		normals.at(23),		{ 1.0f,  1.f } },
		{ vertices.at(17),		normals.at(23),		{ 0.0f,  1.f } },
		{ vertices.at(18),		normals.at(23),		{ 0.f,   0.f } },

	};

	const std::vector<uint32_t> table_indices
	{
		//bottom
		0, 1, 2,					// front
		3, 4, 5,					// left
		6, 7, 8,					// back
		9, 10, 11,					// right

		//mid
		12, 13, 14,
		15, 16, 17,
		        
		18, 19, 20,
		21, 22, 23,
		        
		24, 25, 26,
		27, 28, 29,
		        
		30, 31, 32,
		33, 34, 35,

		// top
		36, 37, 38,
		39, 40, 41,

		42, 43, 44,
		45, 46, 47,

		48, 49,50,
		51, 52, 53,

		54, 55, 56,
		57, 58, 59,

		60, 61, 62,
		63, 64, 65,

		66, 67, 68,
		69, 70, 71,

		72, 73, 74,
		75, 76, 77,

	};

	m_mesh = engine::mesh::create(table_vertices, table_indices);
}

engine::table::~table() {}

engine::ref<engine::table> engine::table::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::table>(vertices);
}
