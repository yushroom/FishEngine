cpp_enum_code = '''
enum class ZTest {
    Less        = GL_LESS,
    Greater     = GL_GREATER,
    LEqual      = GL_LEQUAL,
    GEqual      = GL_GEQUAL,
    Equal       = GL_EQUAL,
    NotEqual    = GL_NOTEQUAL,
    Always      = GL_ALWAYS,
};
'''

lines = cpp_enum_code.strip().split('\n');
line1 = lines[0].strip()
if line1.endswith('{'):
	line1 = line1[:-1]
enum_name = line1.strip().split()[-1]
print enum_name

enum_elements = []

for line in lines[1:-1]:
	#print line
	if "=" in line:
		var = line.split('=')[0]
	else:
		var = line.split(',')[0]
	enum_elements.append(var.strip())

print enum_elements


# enum to string
template = 'inline const char* ToString({} e)\n{{\n    switch (e) {{ \n'.format(enum_name)

for e in enum_elements:
	template += '        case {0}::{1}: return "{1}"; break; \n'.format(enum_name, e)

template += '        default: abort(); break;\n    }\n}'

print template

print ''

# string to enum
template = 'template<>\n'.format(enum_name)
template += 'inline {0} ToEnum<{0}>(const std::string& s)\n{{\n'.format(enum_name)

for e in enum_elements:
	template += '    if (s == "{0}") return {1}::{0};\n'.format(e, enum_name)

template += '    abort();\n}'

print template