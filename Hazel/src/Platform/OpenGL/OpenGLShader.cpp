#include "hzpch.h"
#include "OpenGLShader.h"
#include <fstream>
#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

namespace Hazel {

	static GLenum ShaderTypeFromStringType(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		HZ_CORE_ASSERT(false, "Unkown shader type!");
		return 0;
	}
	static std::string StringTypeFromShaderType(const GLenum& type) {

		if (type == GL_VERTEX_SHADER ) return "vertex";
		if (type == GL_FRAGMENT_SHADER) return "fragment";

		HZ_CORE_ASSERT(false, "Unkown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath) {
		HZ_PROFILE_FUNCTION();
		std::string source = ReadFile(filePath);

		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// extract filepath
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);

	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name) {
		HZ_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader() {
		HZ_PROFILE_FUNCTION();
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath) {
		HZ_PROFILE_FUNCTION();
		std::string result = "";
		std::ifstream in(filePath, std::ios::in | std::ios::binary);

		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			HZ_CORE_ERROR("Could not open file at file path: {0}", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		HZ_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTopenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax Error!");
			size_t begin = pos + typeTopenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(ShaderTypeFromStringType(type), "Invalid shader type specifier");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromStringType(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
		HZ_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();
		HZ_CORE_ASSERT(shaderSources.size() <= 2, "Only support two shaders for now!");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceC = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceC, 0);
			// Compile the vertex shader
			glCompileShader(shader);
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, StringTypeFromShaderType(type) + " shader compilation error!");
				return;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id: glShaderIDs) 
				glDeleteShader(id);

			// Use the infoLog as you see fit.
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader linking error!");
			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	void OpenGLShader::Bind() const {
		HZ_PROFILE_FUNCTION();
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const {
		HZ_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value) {
		HZ_PROFILE_FUNCTION();
		UploadUniformInt(name, value);
	}
	void OpenGLShader::SetFloat(const std::string& name, const float& value) {
		HZ_PROFILE_FUNCTION();
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
		HZ_PROFILE_FUNCTION();
		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
		HZ_PROFILE_FUNCTION();
		UploadUniformFloat3(name, value);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
		HZ_PROFILE_FUNCTION();
		UploadUniformFloat4(name, value);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
		HZ_PROFILE_FUNCTION();
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
