cpp_code = r'''TextureWrapMode m_wrapMode = TextureWrapMode::Repeat;'''

toks = cpp_code.strip().split()
print(toks)

var_type = toks[0]
var_name = toks[1]
if var_name.startswith("m_"):
	var_pretty_name = var_name[2:]
else:
	var_pretty_name = var_name

getter_template = '''
{0} {2}() const
{{
	return {1};
}}
'''

setter_template = '''
void set{3}(const {0}& {2})
{{
	{1} = {2};
}}
'''

var_name2 = var_pretty_name[0].upper() + var_pretty_name[1:]

print(getter_template.format(var_type, var_name, var_pretty_name))
print(setter_template.format(var_type, var_name, var_pretty_name, var_name2))