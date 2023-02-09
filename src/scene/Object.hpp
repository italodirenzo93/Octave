#ifndef OCTAVE_OBJECT_HPP
#define OCTAVE_OBJECT_HPP

#include <glm/glm.hpp>
#include <list>
#include <memory>

namespace octave::scene {

class Object {
public:
	Object() noexcept;
	Object( const Object& other ) noexcept = default;
	Object( Object&& other ) noexcept = default;
	~Object() noexcept = default;

	[[nodiscard]] glm::mat4 GetModelMatrix() noexcept;

	[[nodiscard]] glm::vec3 GetPosition() const noexcept { return position_; }
	Object& SetPosition( const glm::vec3& position ) noexcept;

	Object& Translate( const glm::vec3& translation ) noexcept;

	template <typename... Ty>
	Object& AddChild( const Ty&... args ) noexcept {
		auto child = std::make_unique<Object>( args... );
		child->parent_ = this;
		children_.emplace_back( child );
		return *this;
	}

protected:
	glm::vec3 position_;
	glm::vec3 euler_;
	glm::vec3 scale_;

	glm::mat4 model_matrix_;
	glm::mat4 world_matrix_;

	bool update_model_matrix_ = false;
	bool update_world_matrix_ = false;

	Object* parent_ = nullptr;
	std::list<std::unique_ptr<Object>> children_;

public:
	Object& operator=( const Object& other ) noexcept = default;
	Object& operator=( Object&& other ) noexcept = default;
};

}  // namespace octave::scene

#endif  // OCTAVE_OBJECT_HPP
