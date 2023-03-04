#ifndef OCTAVE_GRAPHICSSYSTEMGL_HPP
#define OCTAVE_GRAPHICSSYSTEMGL_HPP

#include "graphics/GraphicsSystem.hpp"

struct GLFWwindow;

namespace Octave::Impl {

class GraphicsSystemGL : public GraphicsSystem {
public:
	// Constructors
	explicit GraphicsSystemGL( GLFWwindow* window );
	GraphicsSystemGL( const GraphicsSystemGL& other ) = delete;
	GraphicsSystemGL( GraphicsSystemGL&& other ) noexcept = delete;
	~GraphicsSystemGL() noexcept override;

	// Operators
	GraphicsSystemGL& operator=( const GraphicsSystemGL& other ) = delete;
	GraphicsSystemGL& operator=( GraphicsSystemGL&& other ) noexcept = delete;

	// Methods
    void SwapBuffers() override;

	[[nodiscard]] std::string TryDequeueError() noexcept override;

	[[nodiscard]] std::unique_ptr<Renderer> CreateRenderer() noexcept override;
	[[nodiscard]] std::unique_ptr<Buffer> CreateBuffer( BufferBinding binding, size_t byteWidth ) noexcept override;
	[[nodiscard]] std::unique_ptr<Texture> CreateTexture() noexcept override;

private:
    GLFWwindow* window_;
};

}

#endif

