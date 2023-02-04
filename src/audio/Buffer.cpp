#include "Buffer.hpp"

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.cpp"

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

void Buffer::LoadFromVorbisFile( const std::string& path ) {
	int n_channels, sample_rate;
	short* data;

	const int n_samples = stb_vorbis_decode_filename(path.c_str(), &n_channels, &sample_rate, &data );

	if ( n_samples < 0 ) {
		throw Exception( "Unable to open OGG Vorbis file " + path );
	}

	ALenum format;
	if ( n_channels  > 1 ) {
		format = AL_FORMAT_STEREO16;
	} else {
		format = AL_FORMAT_MONO16;
	}

	alBufferData( id_, format, static_cast<void*>( data ),
				  static_cast<ALsizei>( n_samples * sizeof ( short ) * n_channels ),
				  static_cast<ALsizei>( sample_rate ) );

	free( data );

	al::ThrowIfFailed();
}

}  // namespace octave::audio
