#ifndef transform_h
#define transform_h

#include <shared.h>

#include <ces/component.h>

class Transform : public Component {
public:
	// Member Variables

	glm::vec3 position;
	glm::vec3 rotation;

	glm::mat4 matrix = glm::mat4(1.0f);

	// Constructors & Destructors
	
	Transform(const glm::vec3& position = glm::vec3(), const glm::vec3& rotation = glm::vec3());
	
	// Generic Gameloop Functions
	
	void LateUpdate() override;
	
	// Member Functions
	
	void Posite(const glm::vec3& position);
	void Rotate(float rotation, const glm::vec3& axis);
	
	glm::vec3 Up() const;
	glm::vec3 Down() const;
	glm::vec3 Left() const;
	glm::vec3 Right() const;
	glm::vec3 Forward() const;
	glm::vec3 Backward() const;
};

#endif