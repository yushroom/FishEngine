import os, glob

def GetAllHeadersTo(src_dir, output_file):
    all_headers = glob.glob(src_dir + '/*.hpp')
    lines = ['#include "{}"'.format(os.path.abspath(x)) for x in all_headers]
    with open(output_file, 'w') as f:
        f.write('\n'.join(lines))

if __name__ == "__main__":
    GetAllHeadersTo("../../Engine/Source/Runtime", "temp/AllHeaders.hpp")