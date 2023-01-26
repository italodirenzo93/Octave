#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "CommonInclude.hpp"

namespace graphics {
class Window {
    friend class Renderer;

public:
    Window();
    explicit Window( const std::string& title );
    Window( int width, int height, const std::string& title );
    ~Window();

    void PollEvents() const noexcept;

    bool IsOpen() const noexcept;
	void GetSize( int& width, int& height ) const noexcept;

private:
    GLFWwindow* handle_ = nullptr;

    NON_COPYABLE_OR_MOVABLE_CLASS( Window )
};
} // namespace graphics

#endif
