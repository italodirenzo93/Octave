#include "Buffer.hpp"

namespace octave::audio {

Buffer::Buffer() {
	alGenBuffers( 1, &id_ );
	al::ThrowIfFailed();
}

Buffer::~Buffer() noexcept {
	alDeleteBuffers( 1, &id_ );
}

int Buffer::GetFrequency() const noexcept {
	int frequency;
	alGetBufferi( id_, AL_FREQUENCY, &frequency );
	return frequency;
}

int Buffer::GetBitDepth() const noexcept {
	int bit_depth;
	alGetBufferi( id_, AL_BITS, &bit_depth );
	return bit_depth;
}

int Buffer::GetChannels() const noexcept {
	int channels;
	alGetBufferi( id_, AL_CHANNELS, &channels );
	return channels;
}

int Buffer::GetSize() const noexcept {
	int size;
	alGetBufferi( id_, AL_SIZE, &size );
	return size;
}

}  // namespace octave::audio
