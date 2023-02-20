#ifndef OCTAVE_MODELVIEWERSAMPLE_HPP
#define OCTAVE_MODELVIEWERSAMPLE_HPP

#include "Sample.hpp"
#include "graphics/Model.hpp"
#include "input/Gamepad.hpp"
#include "resources/ShaderManager.hpp"

namespace Octave::Samples {

class ModelViewerSample : public Sample {
public:
	void Initialize() override;
	void OnUpdate() override;
	void OnRender() override;

private:
	void DebugCameraControls( DebugCamera& camera, float camera_speed,
							  float delta ) noexcept;
	void DebugCameraControls( const Gamepad& gamepad, DebugCamera& camera,
							  float camera_speed, float delta ) noexcept;

	std::string file_name_;

	std::shared_ptr<Shader> shader_;
	std::unique_ptr<Gamepad> pad_;

	ShaderManager shaders_;

	Model model_;
	glm::mat4 model_matrix_;

	std::unique_ptr<VertexBuffer> floor_vbo_;
	std::unique_ptr<Texture> floor_texture_diffuse_, floor_texture_specular_;
	glm::vec3 floor_position_;
};

}  // namespace Octave::Samples

#endif  // OCTAVE_MODELVIEWERSAMPLE_HPP
