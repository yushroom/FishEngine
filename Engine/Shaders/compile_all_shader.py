import os
import sys

#compiler = r'..\Binary\RelWithDebInfo\ShaderCompiler'
compiler = r'../Binary/Debug/ShaderCompiler'
shader_dirs = ['.', './Editor']
count = 0;

for d in shader_dirs:
    for fn in os.listdir(d):
        print(fn)
        ext = fn.split('.')[-1]

        if ext in ['surf', 'shader']:
            cmd = compiler + ' ' + os.path.abspath(os.path.join(d, fn))
            print(cmd)
            if os.system(cmd) != 0:
                print("Compile ERROR: ", fn)
                sys.exit()
            count += 1

print("Done. {} shaders compiled.".format(count))