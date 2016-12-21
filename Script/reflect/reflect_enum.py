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

all_headers_file_dir = os.path.dirname(all_headers_file_path)
if not os.path.exists(all_headers_file_dir):
    os.makedirs(all_headers_file_dir)

GetAllHeadersTo("../../Engine/Source/Runtime", all_headers_file_path)
enums = ExtractEnums(all_headers_file_path)

if debug:
    print('dump enums')
    with open(enum_json_file_path, 'w') as f:
        f.write(json.dumps(enums, indent = 4))

template_str = '''#pragma once

#include "../ReflectEnum.hpp"
#include "../${header}"

namespace ${scope}
{
${functions}
} // {scope}
'''
template = Template(template_str)

output_hpp_dir = '../../Engine/Source/Runtime/generate/'
if not os.path.exists(output_hpp_dir):
    os.mkdir(output_hpp_dir)

for e in enums.keys():
    item = enums[e]
    enum_functions = GenEnumFunctions(e, enums[e]['data'])
    s = template.render(header=item['header_file'], scope=item['scope_prefix'], functions=enum_functions)
    output_file_path = os.path.join(output_hpp_dir, "Enum_{}.hpp".format(e))
    if os.path.exists(output_file_path):
        with open(output_file_path) as f:
            content = f.read()
            if content == s:
                #print("no update for", output_file_path)
                continue
    with open(output_file_path, 'w') as f:
    	print("update", output_file_path)
        f.write(s)

