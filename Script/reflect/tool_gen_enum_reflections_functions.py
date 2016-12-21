from mako.template import Template
t_str = '''
/**************************************************
* ${T}
**************************************************/

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

def GenEnumFunctions(enum_name, enum_elements):
    #print(enum_name, enum_elements)
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
    return t.render(T = enum_name, length = len(enum_elements), CStrings= CStrings, \
        IndexToEnumCases = index_to_enum_cases, EnumToIndexCases = enum_to_index_cases, \
        StringToEnumCases = string_to_enum_cases)

if __name__ == "__main__":
    enum_name = "ShadowCastingMode"
    enum_elements = ['Off', 'On', 'TwoSided', 'ShdowsOnly']
    print(GenEnumFunctions(enum_name, enum_elements))
