#ifndef OCTAVE_RESOURCEMANAGER_HPP
#define OCTAVE_RESOURCEMANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace octave::resources {

template <class TResource>
class ResourceManager {
public:
	using ResourcePtr = std::shared_ptr<TResource>;

	virtual ResourceManager& AddResource( const std::string& key,
										  ResourcePtr ptr );
	virtual ResourceManager& AddOrReplaceResource( const std::string& key,
												   ResourcePtr ptr ) noexcept;
	virtual ResourceManager& ClearUnused() noexcept;

	[[nodiscard]] virtual ResourcePtr Get( const std::string& key ) noexcept;

protected:
	std::unordered_map<std::string, ResourcePtr> resources_;
};

template <class TResource>
ResourceManager<TResource>& ResourceManager<TResource>::AddResource(
	const std::string& key, ResourcePtr ptr ) {
	resources_.insert( { key, std::move( ptr ) } );
	return *this;
}

template <class TResource>
ResourceManager<TResource>& ResourceManager<TResource>::AddOrReplaceResource(
	const std::string& key, ResourcePtr ptr ) noexcept {
	resources_[key] = std::move( ptr );
	return *this;
}

template <class TResource>
ResourceManager<TResource>& ResourceManager<TResource>::ClearUnused() noexcept {
	std::vector<std::string> unused;

	for ( auto& [key, ptr] : resources_ ) {
		// If the use-count is 1, then the ResourceManager is the only owner
		// left
		if ( ptr.use_count() == 1 ) {
			ptr.reset();
			unused.emplace_back( key );
		}
	}

	for ( const auto& name : unused ) {
		resources_.erase( name );
	}

	return *this;
}

template <class TResource>
typename ResourceManager<TResource>::ResourcePtr
ResourceManager<TResource>::Get( const std::string& key ) noexcept {
	const auto it = resources_.find( key );
	if ( it != resources_.end() ) {
		return it->second;
	}

	return nullptr;
}

}  // namespace octave::resources

#endif  // OCTAVE_RESOURCEMANAGER_HPP
