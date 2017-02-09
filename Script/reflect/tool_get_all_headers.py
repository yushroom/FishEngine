from __future__ import print_function
import os, glob
from tool_helper import UpdateFile

def GetAllHeadersTo(output_file_path):
    all_headers = glob.glob("../../Engine/Source/Runtime/*.hpp")
    all_headers += glob.glob("../../Engine/Source/Editor/*.hpp")
    lines = ['#include "{}"'.format(os.path.abspath(x)) for x in all_headers]
    string = '\n'.join(lines)
    UpdateFile(output_file_path, string)

if __name__ == "__main__":
    GetAllHeadersTo("temp/AllHeaders.hpp")