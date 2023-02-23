#ifndef SHADER_H
#define SHADER_H

#include "CommonInclude.h"

namespace graphics {

	class Shader {
	public:
		explicit Shader(const char* vertexPath, const char* fragmentPath);
		Shader(Shader&& other) noexcept;
		~Shader();

		uint32_t getID() const { return ID; }

		void activate() const;
		void deactivate() const;
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, const glm::mat4& value) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;

	private:
		uint32_t ID = 0;
		explicit Shader(uint32_t id) : ID(id) {}

		Shader() = default;

	public:
		Shader& operator= (Shader&& other) noexcept;

	private:
		NON_COPYABLE_CLASS(Shader)
	};

}

#endif
