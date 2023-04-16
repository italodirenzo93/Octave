#ifndef OCTAVE_GRAPHICS_COLOUR_HPP
#define OCTAVE_GRAPHICS_COLOUR_HPP

namespace Octave {

struct Colour {
	float m_Red, m_Green, m_Blue, m_Alpha;

	Colour() = delete;
	Colour( float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f )
		: m_Red( r ), m_Green( g ), m_Blue( b ), m_Alpha( a ) {}

	// Operators
	bool operator==( const Colour& other ) {
		return m_Red == other.m_Red && m_Green == other.m_Green &&
			   m_Blue == other.m_Blue && m_Alpha == other.m_Alpha;
	}

	bool operator!=( const Colour& other ) {
		return !( *this == other );
	}
};

namespace Colours {
	static const Colour Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const Colour White = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const Colour Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	static const Colour Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	static const Colour Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
}

}  // namespace Octave

#endif
