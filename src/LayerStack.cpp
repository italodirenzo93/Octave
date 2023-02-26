#include "pch/pch.hpp"
#include "LayerStack.hpp"

namespace Octave {

LayerStack::LayerStack() noexcept {
	insert_pos_ = layers_.begin();
}

LayerStack& LayerStack::PushLayer( LayerPtr layer ) noexcept {
	insert_pos_ = layers_.emplace( insert_pos_, layer );
	return *this;
}

LayerStack::LayerPtr LayerStack::PopLayer() noexcept {
	return nullptr;
}

}  // namespace Octave
