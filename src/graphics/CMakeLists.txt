set(sources IndexBuffer.cpp IndexBuffer.hpp Mesh.cpp Mesh.hpp Model.cpp Model.hpp Renderer.cpp Renderer.hpp Shader.cpp Shader.hpp stb_include.h Texture.cpp Texture.hpp VertexBuffer.cpp VertexBuffer.hpp)
list(TRANSFORM sources PREPEND "src/graphics/")

target_sources(Octave PRIVATE ${sources})
