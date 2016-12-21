from mako.template import Template
t_str = '''
// enum count
template<>
constexpr int EnumCount<${T}>() { return ${length}; }

// string array
static const char* ${T}Strings[] =
{
    ${CStrings}
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<${T}>()
{
    return ${T}Strings;
}

// index to enum
template<>
inline ${T} ToEnum<${T}>(const int index)
{
    switch (index) {
    ${IndexToEnumCases}
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<${T}>(${T} e)
{
    switch (e) {
    ${EnumToIndexCases}
    default: abort(); break;
    }
}

// string to enum
template<>
inline ${T} ToEnum<${T}>(const std::string& s)
{
    ${StringToEnumCases}
    abort();
}
'''
t = Template(t_str)


cpp_enum_code = '''
enum class TextureImporterType
{
    Default,        // This is the most common setting used for all the textures in general.
    NormalMap,      // Select this to turn the color channels into a format suitable for real - time normal mapping.
    GUI,            // Use this if your texture is going to be used on any HUD / GUI Controls.
    Sprite,         // Select this if you will be using your texture for Sprite graphics.
    Cursor,         // Use this if your texture is going to be used as a cursor.
    Cookie,         // This sets up your texture with the basic parameters used for the Cookies of your lights.
    Lightmap,       // This sets up your texture with the parameters used by the lightmap.
    SingleChannel,  //Use this for texture containing a single channel.
};
'''

lines = cpp_enum_code.strip().split('\n');
line1 = lines[0].strip()
if line1.endswith('{'):
	line1 = line1[:-1]
enum_name = line1.strip().split()[-1]
print(enum_name)

enum_elements = []

for line in lines[1:-1]:
	#print line
	if line.startswith('{'):
		continue
	if "=" in line:
		var = line.split('=')[0]
	else:
		var = line.split(',')[0]
	enum_elements.append(var.strip())

print(enum_elements)
print('')

#enum_name = "ShadowCastingMode"
#enum_elements = ['Off', 'On', 'TwoSided', 'ShdowsOnly']

index_to_enum_case  = "case {0}: return {1}::{2}; break;"
enum_to_index_case  = "case {1}::{2}: return {0}; break;"
string_to_enum_case = 'if (s == "{1}") return {0}::{1};'

index_to_enum_cases = ''
enum_to_index_cases = ''
string_to_enum_cases = ''

for i in range(len(enum_elements)):
	index_to_enum_cases  += index_to_enum_case.format(i, enum_name, enum_elements[i]) + '\n\t'
	enum_to_index_cases  += enum_to_index_case.format(i, enum_name, enum_elements[i]) + '\n\t'
	string_to_enum_cases += string_to_enum_case.format(enum_name, enum_elements[i]) + '\n\t'

CStrings = ',\n\t'.join(['"{}"'.format(e) for e in enum_elements])
print t.render(T = enum_name, length = len(enum_elements), CStrings= CStrings, \
	IndexToEnumCases = index_to_enum_cases, EnumToIndexCases = enum_to_index_cases, \
	StringToEnumCases = string_to_enum_cases)
