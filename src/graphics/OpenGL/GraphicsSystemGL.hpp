#ifndef OCTAVE_GRAPHICSSYSTEMGL_HPP
#define OCTAVE_GRAPHICSSYSTEMGL_HPP

#include "graphics/GraphicsSystem.hpp"

namespace Octave::Impl {

class GraphicsSystemGL : public GraphicsSystem {
public:
	// Constructors
	GraphicsSystemGL();
	GraphicsSystemGL( const GraphicsSystemGL& other ) = delete;
	GraphicsSystemGL( GraphicsSystemGL&& other ) noexcept = delete;
	~GraphicsSystemGL() noexcept override;

	// Operators
	GraphicsSystemGL& operator=( const GraphicsSystemGL& other ) = delete;
	GraphicsSystemGL& operator=( GraphicsSystemGL&& other ) noexcept = delete;

	// Methods
	[[nodiscard]] std::string TryDequeueError() noexcept override;

	[[nodiscard]] std::unique_ptr<Renderer> CreateRenderer() noexcept override;
};

}

#endif

