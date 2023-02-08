#ifndef OCTAVE_MODELVIEWERSAMPLE_HPP
#define OCTAVE_MODELVIEWERSAMPLE_HPP

#include "Sample.hpp"
#include "graphics/Model.hpp"
#include "input/Keyboard.hpp"

namespace octave::samples {

class ModelViewerSample : public Sample {
public:
	explicit ModelViewerSample(
		const std::shared_ptr<graphics::Window>& window ) noexcept
		: Sample( window ) {}

	void OnLoad() override;
	void OnUpdate( const StepTimer& timer ) override;
	void OnRender() override;
	void OnUnload() override;

private:
	std::shared_ptr<graphics::Shader> shader_;
	std::unique_ptr<input::Keyboard> keyboard_;

	graphics::Model model_;
	glm::mat4 model_matrix_;

	graphics::VertexBuffer floor_vbo_;
	graphics::Texture floor_texture_diffuse_, floor_texture_specular_;
	glm::vec3 floor_position_;
};

}  // namespace octave::samples

#endif  // OCTAVE_MODELVIEWERSAMPLE_HPP
