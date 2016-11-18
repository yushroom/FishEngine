import os
import sys

compiler = "../Binary/Debug/ShaderCompiler"
shader_dirs = ['.', './Editor']

for d in shader_dirs:
    for fn in os.listdir(d):
        ext = fn.split('.')[-1]
        if ext in ['surf', 'shader']:
            cmd = compiler + ' ' + os.path.abspath(os.path.join(d, fn))
            if os.system(cmd) != 0:
                print("Compile ERROR: ", fn)
                sys.exit()