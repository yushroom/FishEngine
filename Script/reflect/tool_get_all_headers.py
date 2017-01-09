from __future__ import print_function
import os, glob

def GetAllHeadersTo(src_dir, output_file_path):
    all_headers = glob.glob(src_dir + '/*.hpp')
    lines = ['#include "{}"'.format(os.path.abspath(x)) for x in all_headers]
    string = '\n'.join(lines)
    if os.path.exists(output_file_path):
	    with open(output_file_path) as f:
	        content = f.read()
	        if content == string:
	            print("no update for", output_file_path)
	            return
    with open(output_file_path, 'w') as f:
        f.write(string)

if __name__ == "__main__":
    GetAllHeadersTo("../../Engine/Source/Runtime", "temp/AllHeaders.hpp")