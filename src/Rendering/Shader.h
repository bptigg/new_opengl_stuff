#pragma once
#pragma once

#include <string>
#include <unordered_map>

#include <memory>

#include "glm/glm.hpp"

class shader
{
private:
	std::string m_filepath;
	std::string m_name;
	unsigned int m_renderer_id;

	std::unordered_map<std::string, int> m_uniform_cache;

	struct shader_source
	{
		std::string vertex_source;
		std::string fragment_source;
	};

	bool s_shutdown;

public:
	shader(const std::string& filepath, std::string name);
	~shader();

	void shutdown();

	void bind() const;
	void unbind() const;

	inline const std::string get_name() const { return m_name; };

#pragma region SET UNIFORMS
	void set_uniform_1i(const std::string& name, int v0);
	void set_uniform_1iv(const std::string& name, int count, const int* v0);
	void set_uniform_1f(const std::string& name, float v0);
	void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3);
	void set_uniform_mat_2f(const std::string& name, const glm::mat2& matrix);
	void set_uniform_mat_4f(const std::string& name, const glm::mat4& matrix);
#pragma endregion
private:
	unsigned int get_uniform_location(const std::string& name);

	shader_source parse_shader(const std::string& filepath);
	unsigned int compile_shader(const std::string& source, unsigned int type);
	unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
	void remove_shader(const unsigned int& program, const unsigned int& shader);

};

class Shader_Library
{
public:

	~Shader_Library();
	void shutdown();

	void Add(const std::string& name, const std::shared_ptr<shader>& shader);
	void Add(const std::shared_ptr<shader>& shader);

	std::shared_ptr<shader> Load(const std::string& name, const std::string& filepath);

	std::shared_ptr<shader> get(const std::string& name);

	bool shader_exists(const std::string& name) const;
private:
	std::unordered_map<std::string, std::shared_ptr<shader>> m_shaders;



};

