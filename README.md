# Octave Game Toolkit

This is my first real big pet project. Having dabbled in this type of work frequently and using commercial engines like Unity and Unreal for years, I've decided to learn how the sauce is made.

## General Project Goals

- Create a collection of code and tools for creating 3D games
  - Libraries for coding
  - Map editor
  - Material editor
- Focus on use of **"Standard C++"** (C++17) for easy knowledge transfer
  - Use modern features where it makes sense (_noexcept_, _lambdas_, _raw strings_, etc.)
  - _const_ and _constexpr_ everywhere!
  - Use exceptions only for "exceptional code" (i.e. this complex operation encountered a non-trivial error)
- Focus on cross-platform (**Windows**, **macOS**, and **Linux** initially)
  - **Graphics** - OpenGL 4.1+
  - **Windowing**/**Input** - GLFW 3.3
  - **Audio** - OpenAL Soft
