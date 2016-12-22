from __future__ import print_function
import os
import clang.cindex
import json
import sys

debug = False

if sys.platform == 'darwin':
    libclang_path = R'/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/libclang.dylib'
else:
    libclang_path = R'D:\Program Files (x86)\LLVM\3.8.0\bin\libclang.dll'
src_dirs = ['../Engine/Source/Runtime']

clang.cindex.Config.set_library_file(libclang_path)
index = clang.cindex.Index.create()

namespaces = ('FishEngine', 'FishEditor')

enum_types = {}

count = {}

skip_cursor_types = (clang.cindex.CursorKind.CLASS_DECL, \
    clang.cindex.CursorKind.STRUCT_DECL, \
    clang.cindex.CursorKind.FUNCTION_TEMPLATE, \
    clang.cindex.CursorKind.UNEXPOSED_DECL,\
    clang.cindex.CursorKind.TEMPLATE_REF
    )

def find_typerefs(node):
    global enum_types
    """ Find all references to the type named 'typename'
    """
    if (node.location.file is not None) and ('FishEngine' not in node.location.file.name):
        return
    if node.kind in skip_cursor_types:
        return
    if debug:
        if node.kind not in count:
            count[node.kind] = 1
        else:
            count[node.kind] += 1
        print(node.kind, node.spelling, node.location.file)

    #print node.spelling, str(node.kind)[str(node.kind).index('.')+1:], node.access_specifier
    short_kind = str(node.kind)[str(node.kind).index('.')+1:]
    #short_kind = str(node.kind).split('.')
    #print 'short kind', short_kind

    if node.kind == clang.cindex.CursorKind.ENUM_DECL:
        if not node.is_definition():
            return
        scope_prefix = '::'.join( node.type.spelling.split('::')[:-1])
        if scope_prefix not in namespaces:
            return
        print('enum:', node.type.spelling)
        #print type(node.location.file.name)
        header_file = os.path.basename( node.location.file.name )
        #print header_file
        #print 'scope ' + scope_prefix
        enum_member_list = []
        for c in node.get_children():
            #print c.spelling, c.kind, c.access_specifier
            if c.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL:
                enum_member_list.append(c.spelling)
        enum_types[node.spelling] = { 'data' : enum_member_list, 'header_file' : header_file, 'scope_prefix' : scope_prefix}
        return

    for c in node.get_children():
        find_typerefs(c)

def ExtractEnums(path):
    global enum_types
    enum_types = {}
    print(path)
    tu = index.parse(path, ['-x', 'c++', '-std=c++14', '-fsyntax-only', '-D_CODE_GENERATOR_'])
    find_typerefs(tu.cursor)
    return enum_types

if __name__ == "__main__":
    print(ExtractEnums("temp/AllHeaders.hpp"))
    if debug:
        for key in count:
            print(key, count[key])
