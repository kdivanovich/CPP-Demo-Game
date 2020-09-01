#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a table object with a mesh of a specified size
	class table
	{
	public:
		/// \brief Constructor
		table(std::vector<glm::vec3> vertices);

		/// \brief Destructor
		~table();

		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<table> create(std::vector<glm::vec3> vertices);
	private:
		/// \brief Fields
		// vertices of the table specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}

