#ifndef transform_h
#define transform_h

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <../src/ces/component.h>

class Transform : public Component {
public:
	// Member Variables

	glm::vec3 position;
	glm::vec3 rotation;

	// Constructors & Destructors
	
	Transform(const glm::vec3& position = glm::vec3(), const glm::vec3& rotation = glm::vec3());
	Transform(Shared<Entity> entity, const Transform& transform);

	~Transform();
	
	// Member Functions
	
	void posite(const glm::vec3& position);
	void rotate(float rotation, const glm::vec3& axis);
	
	glm::vec3 up() const;
	glm::vec3 down() const;
	glm::vec3 left() const;
	glm::vec3 right() const;
	glm::vec3 forward() const;
	glm::vec3 backward() const;
};

#endif
