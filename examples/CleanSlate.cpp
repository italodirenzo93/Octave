#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bimg/encode.h>

int main( int argc, char* argv[] ) {
	bimg::getFormat("");
	bimg::imageEncode(0, bimg::TextureFormat::A8, bimg::Quality::Count, bimg::ImageContainer());
	bimg::imageDecodeToBgra8(0, 0, 0, 0, 0, 0, bimg::TextureFormat::A8);
	return 0;
}
