from __future__ import print_function
import os
import json
from mako.template import Template
from tool_get_all_headers import GetAllHeadersTo
from tool_get_class import ExtractClasses
from tool_gen_class_reflections_functions import GenSerialization

debug = False

all_headers_file_path = "temp/AllHeaders.hpp"
class_json_file_path = 'temp/class.json'

all_headers_file_dir = os.path.dirname(all_headers_file_path)
if not os.path.exists(all_headers_file_dir):
    os.makedirs(all_headers_file_dir)

GetAllHeadersTo("../../Engine/Source/Runtime", all_headers_file_path)
class_info = ExtractClasses(all_headers_file_path)

if debug:
    print('dump class info')
    with open(class_json_file_path, 'w') as f:
        f.write(json.dumps(class_info, indent = 4))

with open('../../Engine/Source/Runtime/generate/Class_Serialization.hpp', 'w') as f:
    f.write(GenSerialization(class_info))
