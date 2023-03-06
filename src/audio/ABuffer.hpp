#ifndef OCTAVE_AUDIO_BUFFER_HPP
#define OCTAVE_AUDIO_BUFFER_HPP

namespace Octave::audio {

class Buffer {
	friend class Source;

public:
	Buffer();
	~Buffer() noexcept;

	[[nodiscard]] int GetFrequency() const noexcept;
	[[nodiscard]] int GetBitDepth() const noexcept;
	[[nodiscard]] int GetChannels() const noexcept;
	[[nodiscard]] int GetSize() const noexcept;

	void LoadFromVorbisFile( const std::string& path );

private:
	uint32_t id_ = 0;
};

}  // namespace octave::audio

#endif  // OCTAVE_AUDIO_BUFFER_HPP
