from __future__ import print_function
import os
import json
from mako.template import Template
from tool_get_all_headers import GetAllHeadersTo
from tool_get_enum import ExtractEnums
from tool_gen_enum_reflections_functions import GenEnumFunctions

debug = False

all_headers_file_path = "temp/AllHeaders.hpp"
enum_json_file_path = 'temp/enum.json'

GetAllHeadersTo("../../Engine/Source/Runtime", all_headers_file_path)
enums = ExtractEnums(all_headers_file_path)

if debug:
    print('dump enums')
    with open(enum_json_file_path, 'w') as f:
        f.write(json.dumps(enums, indent = 4))

template_str = '''#pragma once

#include "../Common.hpp"
#include "../${header}"

namespace ${scope}
{
${functions}
} // {scope}
'''
template = Template(template_str)

for e in enums.keys():
    item = enums[e]
    enum_functions = GenEnumFunctions(e, enums[e]['data'])
    s = template.render(header=item['header_file'], scope=item['scope_prefix'], functions=enum_functions)
    output_file_path = "../../Engine/Source/Runtime/generate/Enum_{}.hpp".format(e)
    if os.path.exists(output_file_path):
        with open(output_file_path) as f:
            content = f.read()
            if content == s:
                print("not update for", output_file_path)
                continue
    with open(output_file_path, 'w') as f:
        f.write(s)

