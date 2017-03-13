#ifndef Shader_hpp
#define Shader_hpp

#include <set>

#include "Object.hpp"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include "Common.hpp"
#include "ShaderProperty.hpp"
#include "Resources.hpp"

namespace FishEngine
{
	class ShaderImpl;

	class FE_EXPORT Shader : public Object
	{
	public:
		InjectClassName(Shader)

			Shader();
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		//Shader(Shader&&);

		~Shader();

		// TODO: ->private
		static void Init(std::string const & rootDir);

		static ShaderPtr CreateFromFile(const Path& path);

		void Use() const;

		bool HasUniform(const std::string& name);

		//GLuint getAttribLocation(const char* name) const;

		//void BindUniformFloat(const char* name, const float value) const;
		//void BindUniformVec3(const char* name, const glm::vec3& value) const;
		//void BindUniformMat3(const char* name, const glm::mat3& value) const;
		void BindUniformVec4(const char* name, const Vector4& value);
		void BindUniformMat4(const char* name, const Matrix4x4& value);

		//void BindUniformTexture(const char* name, const GLuint texture, const GLuint id, GLenum textureType = GL_TEXTURE_2D) const;

		void BindMatrixArray(const std::string& name, const std::vector<Matrix4x4>& matrixArray);
		void BindUniforms(const ShaderUniforms& uniforms);

		void BindTexture(const std::string& name, TexturePtr texture);
		void BindTextures(const std::map<std::string, TexturePtr>& textures);

		void PreRender() const;
		void PostRender() const;

		void CheckStatus() const;

		const std::vector<UniformInfo>& uniforms() const
		{
			return m_uniforms;
		}

		std::vector<UniformInfo>& uniforms()
		{
			return m_uniforms;
		}

		static ShaderPtr builtinShader(const std::string& name);

		static const std::map<std::string, ShaderPtr>& allShaders()
		{
			// TODO
			return m_builtinShaders;
		}

		bool IsTransparent() const
		{
			return m_blend;
		}

		bool IsDeferred() const
		{
			return m_deferred;
		}

		bool IsKeywordEnabled(ShaderKeyword keyword)
		{
			return (m_keywords & static_cast<ShaderKeywords>(keyword)) != 0;
		}

		void SetLocalKeywords(ShaderKeyword keyword, bool value);

		void EnableLocalKeywords(ShaderKeywords keyword);

		void DisableLocalKeywords(ShaderKeywords keyword);

	private:
		friend class Material;
		friend class RenderSystem;

		Meta(NonSerializable)
			std::unique_ptr<ShaderImpl> m_impl;

		//void GetAllUniforms();
		bool FromFile(const Path& path);

		// cache
		Meta(NonSerializable)
			unsigned int m_GLNativeProgram = 0;
		std::vector<UniformInfo> m_uniforms;

		Cullface    m_cullface = Cullface::Back;
		bool        m_ZWrite = true;
		bool        m_blend = false;
		bool        m_deferred = false;

		ShaderKeywords m_keywords = static_cast<ShaderKeywords>(ShaderKeyword::None);

		static std::map<std::string, ShaderPtr> m_builtinShaders;
	};
}

#endif
