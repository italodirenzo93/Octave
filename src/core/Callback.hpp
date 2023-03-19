#ifndef OCTAVE_CORE_CALLBACK_HPP
#define OCTAVE_CORE_CALLBACK_HPP

#include <functional>
#include <vector>

namespace Octave {

template <class TReturn, class... TArgs>
class Callback {
public:
	using Listener = std::function<TReturn( TArgs... )>;
	using Iterator = typename std::vector<Listener>::iterator;
	using ConstIterator = typename std::vector<Listener>::const_iterator;

	[[nodiscard]] Iterator begin() const noexcept { return listeners_.begin(); }
	[[nodiscard]] Iterator end() const noexcept { return listeners_.end(); }

	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return listeners_.cbegin();
	}
	[[nodiscard]] ConstIterator cend() const noexcept {
		return listeners_.cend();
	}

	Iterator Add( Listener&& listener ) noexcept {
		return listeners_.emplace( listeners_.end(), listener );
	}

	Listener Remove( ConstIterator which ) noexcept {
		if ( which == listeners_.end() ) {
			return listeners_.end();
		}

		auto listener = *which;
		listeners_.erase( which );

		return listener;
	}

	void InvokeAll( TArgs... args ) {
		for ( auto& fn : listeners_ ) {
			fn( args... );
		}
	}

private:
	std::vector<Listener> listeners_;
};

}  // namespace Octave

#endif
