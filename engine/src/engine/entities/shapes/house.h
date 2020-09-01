#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a house object with a mesh of a specified size
	class house
	{
	public:
		/// \brief Constructor
		house(std::vector<glm::vec3> vertices);

		/// \brief Destructor
		~house();

		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<house> create(std::vector<glm::vec3> vertices);
	private:
		/// \brief Fields
		// vertices of the house specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}

