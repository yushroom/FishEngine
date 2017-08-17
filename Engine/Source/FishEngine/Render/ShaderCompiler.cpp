#include <FishEngine/ShaderCompiler.hpp>

#include <iostream>
#include <cctype>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <FishEngine/Debug.hpp>
#include <FishEngine/Color.hpp>

#include <FishEngine/Render/Shader/ShaderCompileError.hpp>

using namespace std;
using namespace FishEngine;

constexpr size_t npos = std::numeric_limits<size_t>::max();

static std::string ReadFile(const FishEngine::Path& path)
{
	std::ifstream fin(path.string());
	if (!fin.is_open()) {
		LogError("Can not open shader file: " + path.string());
		throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::FileNotFound);
	}
	return std::string(
		std::istreambuf_iterator<char>(fin.rdbuf()),
		std::istreambuf_iterator<char>());
}


std::string nextTok(const std::string &shaderText, size_t& cursor)
{
	size_t start = cursor;
	size_t end = shaderText.size();
	
	string test = shaderText.substr(start, 2);
	if (test == "//" || test == "/*" || test == "*/")
	{
		return test;
	}
	
	test = shaderText.substr(start, 8);
	if (test == "#include")
	{
		cursor += 8;
		return test;
	}
	bool first_is_space = (std::isspace(shaderText[start]) != 0);
	while (cursor < end)
	{
		char c = shaderText[cursor];
		bool is_space = (std::isspace(c) != 0);
		if ((first_is_space && !is_space) ||
			(!first_is_space && is_space))
			break;
		cursor++;
	}
	if (start == cursor)
		cursor++;
	return shaderText.substr(start, cursor - start);
}

void readToNewline(const std::string& shaderText, size_t& cursor)
{
	for (; cursor < shaderText.size(); cursor++)
	{
		if (shaderText[cursor] == '\n')
			return;
	}
}

void ignoreSpace(const std::string& text, size_t& cursor)
{
	if (std::isspace(text[cursor]))
		nextTok(text, cursor);
}

bool expect(const std::string& text, size_t& cursor, char target)
{
	if (text[cursor] != target)
		return false;
	cursor++;
	return true;
}

bool expect(const std::string& text, size_t& cursor, const std::string& target)
{
	for (int i = 0; i < target.size(); ++i)
	{
		if (text[cursor + i] != target[i])
			return false;
	}
	cursor += target.size();
	return true;
}

size_t findRightPair(const std::string& text, const size_t cursor, char left, char right)
{
	int left_count = 1;
	size_t i = cursor;
	for (; i < text.size(); ++i)
	{
		if (text[i] == left)
		{
			left_count++;
		}
		else if (text[i] == right)
		{
			left_count--;
			if (left_count == 0)
			{
				return i;
			}
		}
	}
	return npos;
}


size_t findPair(const std::string& text, const size_t cursor)
{
	int left_count = 1;
	size_t i = cursor;
	for (; i < text.size(); ++i)
	{
		if (text[i] == '{')
		{
			left_count++;
		}
		else if (text[i] == '}')
		{
			left_count--;
			if (left_count == 0)
			{
				return i;
			}
		}
	}
	return npos;
}

class Tokenizer
{
public:

	std::string m_string;
	std::size_t m_cursor = 0;

	Tokenizer(std::string const & str) : m_string(str)
	{

	}

	// ignore leading spaces
	// until spaces
	std::string NextToken()
	{
		RangeCheck();
		ignoreSpace(m_string, m_cursor);
		return nextTok(m_string, m_cursor);
	}

	// cursor will point the position right behind target
	std::string NextTokenUntil(char target)
	{
		RangeCheck();
		ignoreSpace(m_string, m_cursor);
		auto end = m_string.find_first_of(target, m_cursor);
		if (end == std::string::npos)
		{
			throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::InvalidSyntax);
		}
		std::string ret = m_string.substr(m_cursor, end - m_cursor);
		m_cursor = end+1;
		return ret;
	}

	// ')'
	std::string NextTokenUntilRightParentheese()
	{
		auto pos = findRightPair(m_string, m_cursor, '(', ')');
		//assert(pos != npos); // unmatched parenthesis
		if (pos == npos)
		{
			throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::UnMatchedParenthese);
		}

		auto begin = m_cursor;
		m_cursor = pos + 1;
		return m_string.substr(begin, pos - begin);
	}

	std::string RemainingString()
	{
		RangeCheck();
		auto p = m_cursor;
		m_cursor = m_string.size();
		return m_string.substr(p);
	}

	void Expect(char target)
	{
		RangeCheck();
		ignoreSpace(m_string, m_cursor);
		bool result = expect(m_string, m_cursor, target);
		//assert(result);		// syntax error
		if (!result)
		{
			throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::InvalidSyntax);
		}
	}

	void Expect(std::string const & target)
	{
		RangeCheck();
		ignoreSpace(m_string, m_cursor);
		bool result = expect(m_string, m_cursor, target);
		//assert(result);		// syntax error
		if (!result)
		{
			throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::InvalidSyntax);
		}
	}

	// assert eof
	void ExpectEndOfFile()
	{
		ignoreSpace(m_string, m_cursor);
		//assert(m_cursor >= m_string.size());
		if (m_cursor < m_string.size())
		{
			throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::InvalidSyntax);
		}
	}

	void RangeCheck()
	{
		assert(m_cursor < m_string.size());
		if (m_cursor >= m_string.size()) // early end of file
		{
			throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::EarlyEndOfFile);
		}
	}
};

enum class ShaderLabPropertyType
{
	Float,
	Range,
	Color,
	Vector,
	Texture2D,
	TextureCube,
	Texture3D,
	Texture2DArray,
};


namespace FishEngine
{
	std::map<std::string, std::string> ShaderCompiler::s_cachedHeaders;

	Path ShaderCompiler::s_shaderIncludeDir;

	std::string ShaderCompiler::PreprocessShaderFile(const Path& path)
	{
		if (path.extension() == ".inc")
		{
			std::string full_path = boost::filesystem::absolute(path).string();
			auto it = s_cachedHeaders.find(full_path);
			if (it != s_cachedHeaders.end())
			{
				return it->second;
			}
			//Debug::LogWarning("Open header %s", path.string().c_str());
			const std::string& shaderText = ReadFile(path);
			auto parsed = PreprocessImpl(shaderText, m_path.parent_path());
			s_cachedHeaders[full_path] = parsed;
			return parsed;
		}

		auto shaderText = ReadFile(path);
		if (path.extension() == ".surf")
		{
			shaderText = "#include <SurfaceShaderCommon.inc>\n#ifdef SURFACE_SHADER\n" +
				shaderText + "\n#endif\n";
		}

		return PreprocessImpl(shaderText, m_path.parent_path());
	}
	
	ShaderBlendFactor ParseShaderBlendFactor(std::string const & str)
	{
		if (str ==  "One")
		{
			return ShaderBlendFactor::One;
		}
		else if (str ==  "Zero")
		{
			return ShaderBlendFactor::Zero;
		}
		else if (str ==  "SrcColor")
		{
			return ShaderBlendFactor::SrcColor;
		}
		else if (str ==  "SrcAlpha")
		{
			return ShaderBlendFactor::SrcAlpha;
		}
		else if (str ==  "DstColor")
		{
			return ShaderBlendFactor::DstColor;
		}
		else if (str ==  "DstAlpha")
		{
			return ShaderBlendFactor::DstAlpha;
		}
		else if (str ==  "OneMinusSrcColor")
		{
			return ShaderBlendFactor::OneMinusDstColor;
		}
		else if (str ==  "OneMinusSrcAlpha")
		{
			return ShaderBlendFactor::OneMinusSrcAlpha;
		}
		else if (str ==  "OneMinusDstColor")
		{
			return ShaderBlendFactor::OneMinusDstColor;
		}
		else if (str ==  "OneMinusDstAlpha")
		{
			return ShaderBlendFactor::OneMinusDstAlpha;
		}
		//abort();
		throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::UnknownType);
	}

	std::string ShaderCompiler::PreprocessImpl(const std::string& shaderText, const Path& localDir)
	{
		std::string parsed;
		parsed.reserve(shaderText.size());

		size_t cursor = 0;
		size_t end = shaderText.size();

		while (cursor < end)
		{
			size_t begin_of_this_tok = cursor;
			string tok = nextTok(shaderText, cursor);
			if (tok == "//")
			{
				readToNewline(shaderText, cursor);
				parsed += shaderText.substr(begin_of_this_tok, cursor - begin_of_this_tok);
			}
			else if (tok == "/*")
			{
				bool found_end_of_comment = false;
				size_t i = cursor;
				for (; i < shaderText.size() - 1; ++i)
				{
					if (shaderText[i] == '*' && shaderText[i + 1] == '/')
					{
						found_end_of_comment = true;
						break;
					}
				}
				assert(found_end_of_comment);
				cursor = i + 2;
				parsed += shaderText.substr(begin_of_this_tok, cursor - begin_of_this_tok);
			}
			else if (tok == "#include")
			{
				ignoreSpace(shaderText, cursor);
				auto tok = nextTok(shaderText, cursor);
				bool is_system_include = tok[0] == '<';
				auto header = tok.substr(1, tok.size() - 2);
				//cout << "include " << header << " " << is_system_include << endl;
				Path header_path = is_system_include ?
					(s_shaderIncludeDir / header) : (localDir / header);
				m_includeDepth++;
				string parsed_header_text = PreprocessShaderFile(header_path);
				//out_parsedShaderText += shaderText.substr(begin_of_this_tok, begin-begin_of_this_tok);
				parsed += parsed_header_text + "\n";
			}
			else if (tok == "uniform")
			{
				ignoreSpace(shaderText, cursor);
				auto type = nextTok(shaderText, cursor);
				ignoreSpace(shaderText, cursor);
				auto name = nextTok(shaderText, cursor);
				//cout << "uniform " << type << " " << name << endl;
				parsed += shaderText.substr(begin_of_this_tok, cursor - begin_of_this_tok);
			}
			else if (tok.size() > 0 && tok[0] == '@')
			{
				if (tok == "@Properties")
				{
					//ShaderLabProperties m_properties;
					ignoreSpace(shaderText, cursor);
					expect(shaderText, cursor, "{");
					auto begin = cursor;
					auto end = findPair(shaderText, cursor);
					if (end == npos)
					{
						throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::InvalidSyntax);
					}
					cursor = end + 1;
					std::string properitesString = shaderText.substr(begin + 1, end - begin - 1);
					//Debug::Log("properitesString %s", properitesString.c_str());
					boost::trim(properitesString);
					std::vector<std::string> lines;
					boost::split(lines, properitesString, boost::is_any_of("\n"));
					for (auto & line : lines)
					{
						boost::trim(line);
						if (line.empty())
							continue;
						Tokenizer tokenizer(line);
						std::string name = tokenizer.NextTokenUntil('(');
						boost::trim(name);
						tokenizer.Expect('"');
						std::string displayName = tokenizer.NextTokenUntil('"');
						tokenizer.Expect(',');
						std::string type = tokenizer.NextTokenUntilRightParentheese();
						boost::trim(type);
						tokenizer.Expect('=');
						std::string defaultValue = tokenizer.RemainingString();
						boost::trim(defaultValue);
						tokenizer.ExpectEndOfFile();
						m_savedProperties.AddProperty(name, displayName, type, defaultValue);
					}
				}
				else if (tok == "@vertex")
				{
					parsed += parseSubShader(shaderText, cursor, "VERTEX_SHADER", localDir);
				}
				else if (tok == "@geometry")
				{
					LogInfo("Geometry shader enabled");
					m_hasGeometryShader = true;
					parsed += parseSubShader(shaderText, cursor, "GEOMETRY_SHADER", localDir);
				}
				else if (tok == "@fragment")
				{
					parsed += parseSubShader(shaderText, cursor, "FRAGMENT_SHADER", localDir);
				}
				else    // keyword
				{
					auto name = tok.substr(1);
					boost::to_lower(name);
					if (name == "deferred")
					{
						//cout << "Keyword " << name << endl;
						m_settings[name] = "on";
					}
					else if (name == "name")
					{
						ignoreSpace(shaderText, cursor);
						assert(expect(shaderText, cursor, "\""));
						auto begin = cursor;
						readToNewline(shaderText, cursor);
						cursor--;
						assert(shaderText[cursor] == '"');
						auto end = cursor;
						m_name = shaderText.substr(begin, end-begin);
					}
					else if (name == "blend")
					{
						m_blendEnabled = true;
						//m_settings[nam]
						//auto begin = cursor;
						//readToNewline(shaderText, cursor);
						//auto line = shaderText.substr(begin, cursor - begin);
						//auto t = Tokenizer(line);
						ignoreSpace(shaderText, cursor);
						auto factor1 = nextTok(shaderText, cursor);
						ignoreSpace(shaderText, cursor);
						auto factor2 = nextTok(shaderText, cursor);
						m_blendFactorCount = 2;
						m_blendFactors[0] = ParseShaderBlendFactor(factor1);
						m_blendFactors[1] = ParseShaderBlendFactor(factor2);
						if (expect(shaderText, cursor, ","))
						{
							ignoreSpace(shaderText, cursor);
							auto factor3 = nextTok(shaderText, cursor);
							ignoreSpace(shaderText, cursor);
							auto factor4 = nextTok(shaderText, cursor);
							m_blendFactors[2] = ParseShaderBlendFactor(factor3);
							m_blendFactors[3] = ParseShaderBlendFactor(factor4);
							m_blendFactorCount = 4;
						}
					}
					else
					{
						ignoreSpace(shaderText, cursor);
						auto setting = nextTok(shaderText, cursor);
						boost::to_lower(setting);
						//cout << "Keyword " << name << " " << setting << endl;
						// TODO: override if smaller depth
						// do NOT override settings;
						auto it = m_settings.find(name);
						if (it == m_settings.end())
							m_settings[name] = setting;
					}

					readToNewline(shaderText, cursor);
				}
			}
			else
			{
				parsed += tok;
			}
		}
		return parsed;
	}

	std::string ShaderCompiler::parseSubShader(const std::string& shaderText, size_t& cursor, const std::string& str, const Path& localDir)
	{
		std::string out_parsedShaderText;
		ignoreSpace(shaderText, cursor);
		expect(shaderText, cursor, "{");
		//cout << "vertex begin"  << endl;
		out_parsedShaderText += "#ifdef " + str;
		size_t right = findPair(shaderText, cursor);
		//std::cout << shaderText.substr(begin, right - begin) << std::endl;
		string parsed = PreprocessImpl(shaderText.substr(cursor, right - cursor), localDir);
		out_parsedShaderText += parsed;
		out_parsedShaderText += "#endif /*" + str + "*/\n";
		cursor = right + 1;
		return out_parsedShaderText;
	}


	void ShaderLabProperties::AddProperty(std::string const & name, std::string const & displayName, std::string const & type, std::string const & defalutValueString)
	{
		if (type == "Float")
		{
			float value = boost::lexical_cast<float>(defalutValueString);
			m_Floats[name] = value;
		}
		else if (boost::starts_with(type, "Range"))
		{
			Tokenizer t(type);
			t.NextTokenUntil('(');
			auto minStr = t.NextTokenUntil(',');
			auto maxStr = t.NextTokenUntil(')');
			float minFloat = boost::lexical_cast<float>(minStr);
			float maxFloat = boost::lexical_cast<float>(maxStr);
			float value = boost::lexical_cast<float>(defalutValueString);
			m_Ranges[name] = ShaderLabRangeProperty(value, minFloat, maxFloat);
			t.ExpectEndOfFile();
		}
		else if (type == "Color")
		{
			FishEngine::Color color;
			Tokenizer t(defalutValueString);
			t.NextTokenUntil('(');
			auto str = t.NextTokenUntil(',');
			float value = boost::lexical_cast<float>(str);
			color.r = value;

			str = t.NextTokenUntil(',');
			value = boost::lexical_cast<float>(str);
			color.g = value;

			str = t.NextTokenUntil(',');
			value = boost::lexical_cast<float>(str);
			color.b = value;

			str = t.NextTokenUntil(')');
			value = boost::lexical_cast<float>(str);
			color.a = value;

			t.ExpectEndOfFile();
			m_Colors[name] = color;
		}
		else if (type == "Vector")
		{
			FishEngine::Vector4 v;
			Tokenizer t(defalutValueString);
			t.NextTokenUntil('(');
			auto str = t.NextTokenUntil(',');
			float value = boost::lexical_cast<float>(str);
			v.x = value;

			str = t.NextTokenUntil(',');
			value = boost::lexical_cast<float>(str);
			v.y = value;

			str = t.NextTokenUntil(',');
			value = boost::lexical_cast<float>(str);
			v.z = value;

			str = t.NextTokenUntil(')');
			value = boost::lexical_cast<float>(str);
			v.w = value;

			t.ExpectEndOfFile();
			m_Vectors[name] = v;
		}
		else if (type == "2D")
		{
			Tokenizer t(defalutValueString);
			t.NextTokenUntil('"');
			auto defaultTextureName = t.NextTokenUntil('"');
			t.Expect('{');
			t.Expect('}');
			t.ExpectEndOfFile();
			m_TexEnvs[name] = nullptr;
		}
		else
		{
			//abort(); // unknown property type
			throw ShaderCompileError(ShaderCompileStage::Preprocessor, 0, ShaderCompileErrorType::UnknownType);
		}
		m_names.emplace_back(std::make_pair(displayName, name));
	}

}
