#ifndef OCTAVE_LAYERSTACK_HPP
#define OCTAVE_LAYERSTACK_HPP

#include <memory>
#include <vector>

#include "Layer.hpp"

namespace Octave {

class LayerStack {
public:
	using LayerPtr = std::unique_ptr<Layer>;

	LayerStack() noexcept;

	LayerStack& PushLayer( LayerPtr layer ) noexcept;
	[[nodiscard]] LayerPtr PopLayer() noexcept;

	std::vector<LayerPtr>::iterator begin() noexcept { return layers_.begin(); }
	std::vector<LayerPtr>::iterator end() noexcept { return layers_.end(); }

private:
	std::vector<LayerPtr> layers_;
	std::vector<LayerPtr>::iterator insert_pos_;
};

}

#endif
