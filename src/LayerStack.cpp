#include "pch/pch.hpp"
#include "LayerStack.hpp"

namespace Octave {

LayerStack::LayerStack() noexcept {
	insert_pos_ = layers_.begin();
}

LayerStack& LayerStack::PushLayer( LayerPtr layer ) noexcept {
	insert_pos_ = layers_.emplace( insert_pos_, std::move( layer ) );
	return *this;
}

LayerStack::LayerPtr LayerStack::PopLayer() noexcept {
	if ( layers_.empty() ) return nullptr;

	auto ptr = std::move(*(insert_pos_));

	layers_.erase( insert_pos_ );

	return ptr;
}

}  // namespace Octave
