#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "CommonInclude.hpp"

namespace octave::graphics {

class Window {
    friend class Renderer;

public:
    Window();
    explicit Window( const std::string& title );
    Window( int width, int height, const std::string& title );
    ~Window() noexcept;

    void GetSize( int& width, int& height ) const noexcept;

    [[nodiscard]] bool IsOpen() const noexcept;
    [[nodiscard]] bool IsMinimized() const noexcept;
    [[nodiscard]] bool IsMaximized() const noexcept;

    // Actions
    void PollEvents() const noexcept;
    void Minimize() const noexcept;
    void Maximize() const noexcept;
    void Restore() const noexcept;

    using OnSizeChangedCallback = std::function<void(int, int)>;
    void SetSizeChangedCallback(OnSizeChangedCallback callback) noexcept {
        cb_window_size_ = std::move( callback );
    }

private:
    GLFWwindow* handle_ = nullptr;

    // Callbacks
    OnSizeChangedCallback cb_window_size_;
    static void WindowSizeCallback(GLFWwindow* window, int width, int height) noexcept;

    NON_COPYABLE_OR_MOVABLE_CLASS( Window )
};

} // namespace octave::graphics

#endif
