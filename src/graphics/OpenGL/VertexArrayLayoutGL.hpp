#ifndef OCTAVE_GRAPHICS_OPENGL_VERTEXARRAYLAYOUTGL_HPP
#define OCTAVE_GRAPHICS_OPENGL_VERTEXARRAYLAYOUTGL_HPP

#include <unordered_set>
#include <glad/glad.h>

#include "graphics/VertexArrayLayout.hpp"

namespace Octave::Impl {

struct VertexAttributeGL {
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;

	bool operator==( const VertexAttributeGL& other ) const noexcept;
};

class VertexArrayLayoutGL : public VertexArrayLayout {
public:
	using Iterator = std::unordered_set<VertexAttributeGL>::iterator;

	VertexArrayLayoutGL() noexcept;
	~VertexArrayLayoutGL() noexcept override;

	void Bind() const noexcept;
	void Unbind() const noexcept;

	[[nodiscard]] GLuint GetId() const noexcept { return id_; }

	void SetAttributes(
		const std::vector<VertexAttribute>& attributes ) override;
	void AddAttribute( VertexAttribute attribute ) override;

	Iterator begin() noexcept { return attributes_.begin(); }
	Iterator end() noexcept { return attributes_.end(); }

private:
	GLuint id_ = 0;
	std::unordered_set<VertexAttributeGL> attributes_;
};

}  // namespace Octave::Impl

#endif
