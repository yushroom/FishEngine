cpp_enum_code = '''
enum class LightType {
    Directional,
    Point,
    Spot,
    Area,
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
print ''

# count
template = "template<>\nconstexpr int EnumCount<{0}>() {{ return {1}; }}".format(enum_name, len(enum_elements))
print("// enum count")
print(template)
print ''

# String array
template = "constexpr const char* {}Strings[] = {{\n".format(enum_name)
for e in enum_elements:
	template += '    "{}", \n'.format(e)
template += '};'
print("// string array")
print(template)
print ''


# index to enum
template = 'template<>\n'.format(enum_name)
template += 'inline {0} ToEnum<{0}>(const int index)\n{{\n    switch (index) {{ \n'.format(enum_name)
for idx, e in enumerate(enum_elements):
	template += '        case {2}: return {0}::{1}; break; \n'.format(enum_name, e, idx)

print("// index to enum")
template += '        default: abort(); break;\n    }\n}'
print template
print ''


# enum to index
template = 'inline int ToIndex({} e)\n{{\n    switch (e) {{ \n'.format(enum_name)
for idx, e in enumerate(enum_elements):
	template += '        case {0}::{1}: return {2}; break; \n'.format(enum_name, e, idx)
template += '        default: abort(); break;\n    }\n}'
print("// enum to index")
print template
print ''


# enum to string
template = 'inline const char* ToString({} e)\n{{\n    switch (e) {{ \n'.format(enum_name)
for e in enum_elements:
	template += '        case {0}::{1}: return "{1}"; break; \n'.format(enum_name, e)
template += '        default: abort(); break;\n    }\n}'
print("// enum to string");
print template
print ''


# string to enum
template = 'template<>\n'.format(enum_name)
template += 'inline {0} ToEnum<{0}>(const std::string& s)\n{{\n'.format(enum_name)
for e in enum_elements:
	template += '    if (s == "{0}") return {1}::{0};\n'.format(e, enum_name)
print("// string to enum")
template += '    abort();\n}'
print template