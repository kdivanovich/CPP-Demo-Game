#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a chair object with a mesh of a specified size
	class chair
	{
	public:
		/// \brief Constructor
		chair(std::vector<glm::vec3> vertices);

		/// \brief Destructor
		~chair();

		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<chair> create(std::vector<glm::vec3> vertices);
	private:
		/// \brief Fields
		// vertices of the chair specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}

