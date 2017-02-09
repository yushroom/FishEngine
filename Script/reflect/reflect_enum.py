from __future__ import print_function
import os
import json
from mako.template import Template
from tool_get_all_headers import GetAllHeadersTo
from tool_get_enum import ExtractEnums
from tool_gen_enum_reflections_functions import GenEnumFunctions
from tool_helper import UpdateFile

debug = True

all_headers_file_path = "temp/AllHeaders.hpp"
enum_json_file_path = 'temp/enum.json'

all_headers_file_dir = os.path.dirname(all_headers_file_path)
if not os.path.exists(all_headers_file_dir):
    os.makedirs(all_headers_file_dir)

GetAllHeadersTo(all_headers_file_path)
enums = ExtractEnums(all_headers_file_path)

if debug:
    print('dump enums')
    with open(enum_json_file_path, 'w') as f:
        f.write(json.dumps(enums, indent = 4))

template_str = '''#pragma once

#include <ReflectEnum.hpp>
#include <${header}>

namespace FishEngine
{

${functions}

} // namespace FishEngine
'''
template = Template(template_str)

runtime_root_dir = os.path.abspath('../../Engine/Source/Runtime')
editor_root_dir = os.path.abspath('../../Engine/Source/Editor')

output_hpp_dir = os.path.join(runtime_root_dir, "generate")
output_hpp_dir_editor = os.path.join(editor_root_dir, "generate")

for e in enums.keys():
    item = enums[e]
    enum_functions = GenEnumFunctions(e, item['scope_prefix'], enums[e]['data'])
    s = template.render(header=item['header_file'], scope=item['scope_prefix'], functions=enum_functions)
    d = output_hpp_dir
    #print(item['path'], editor_root_dir)
    if item['path'].startswith(editor_root_dir):
    	d = output_hpp_dir_editor
    output_file_path = os.path.join(d, "Enum_{}.hpp".format(e))
    UpdateFile(output_file_path, s)
