#ifndef Material_hpp
#define Material_hpp

#pragma once

#include "Object.hpp"
#include "ShaderProperty.hpp"
#include "shader/ShaderLabProperties.hpp"

namespace FishEngine
{
	enum class MaterialPropertyType
	{
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Texture2D,
		Texture2DArray,
		Texture2DArrayShadow,
		Texture3D,
		TextureCube,
	};

	struct MaterialProperty
	{
		std::string             name;
		MaterialPropertyType    type;
	};


	class FE_EXPORT Material : public Object
	{
	public:
		InjectClassName(Material);

		Material() = default;

		Material(const ShaderPtr& shader)
		{
			setShader(shader);
		}

		/**
		 * The main material's color.
		 *
		 * @return FishEngine::Color
		 */
		 Color color() const;

		/**
		 * The shader used by the material.
		 *
		 * @return FishEngine::ShaderPtr
		 */
		 ShaderPtr shader() const
		{
			return m_shader;
		}

		// The shader used by the material.
		void setShader(const ShaderPtr& shader);

		/**
		 * Unset a shader keyword.
		 *
		 * @param ShaderKeyword keyword
		 * @return void
		 */
		void DisableKeyword(ShaderKeyword keyword)
		{
			DisableKeywords(static_cast<ShaderKeywords>(keyword));
		}


		/**
		 * Set a shader keyword that is enabled by this material.
		 *
		 * @param ShaderKeyword keyword
		 * @return void
		 */
		 void EnableKeyword(ShaderKeyword keyword)
		{
			EnableKeywords(static_cast<ShaderKeywords>(keyword));
		}

		void DisableKeywords(ShaderKeywords keyword);

		void EnableKeywords(ShaderKeywords keyword);

		bool IsKeywordEnabled(ShaderKeyword keyword);

		bool HasProperty(const std::string& propertyName);

		// Set a named float value.
		void SetFloat(const std::string& name, const float value);

		void SetVector2(const std::string& name, const Vector2& value);

		// Set a named Vector3 value.
		void SetVector3(const std::string& name, const Vector3& value);

		void SetVector4(const std::string& name, const Vector4& value);

		// Set a named matrix for the shader.
		void SetMatrix(const std::string& name, const Matrix4x4& value)
		{
			m_uniforms.mat4s[name] = value;
		}

		// Set a named texture
		void SetTexture(const std::string& name, TexturePtr texture);

		// The material's texture.
		// The same as using GetTexture or SetTexture with "_MainTex" name.
		void setMainTexture(TexturePtr texture);

		// The main material's color.
		// The same as using GetColor or SetColor with "_Color" name.
		void setColor(const Color& color);

		void BindTextures(const std::map<std::string, TexturePtr>& textures);

		void BindProperties();

		/************************************************************************/
		/* Static Members                                                       */
		/************************************************************************/

		static void Init();

		static MaterialPtr CreateMaterial()
		{
			return std::make_shared<Material>();
		}

		static MaterialPtr builtinMaterial(const std::string& name);

		static MaterialPtr InstantiateBuiltinMaterial(const std::string& name);

		static MaterialPtr defaultMaterial();

	private:
		friend class FishEditor::Inspector;

		ShaderPtr                           m_shader = nullptr;
		std::map<std::string, TexturePtr>   m_textures;
		ShaderUniforms                      m_uniforms;
		std::vector<MaterialProperty>       m_properties;

		ShaderLabProperties m_savedProperties;

		static std::map<std::string, MaterialPtr>   s_builtinMaterialInstance;
		//static MaterialPtr                          s_defaultMaterial;
	};
}


#endif /* Material_hpp */
