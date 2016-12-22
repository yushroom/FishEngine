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

classes = {}

count = {}

skip_cursor_types = (clang.cindex.CursorKind.ENUM_DECL,
    clang.cindex.CursorKind.STRUCT_DECL,
    clang.cindex.CursorKind.FUNCTION_TEMPLATE,
    clang.cindex.CursorKind.UNEXPOSED_DECL,
    clang.cindex.CursorKind.TEMPLATE_REF
    )

skip_class_internal_type = (clang.cindex.CursorKind.UNEXPOSED_DECL,
    clang.cindex.CursorKind.CXX_ACCESS_SPEC_DECL,
    clang.cindex.CursorKind.TYPEDEF_DECL,
    clang.cindex.CursorKind.FUNCTION_TEMPLATE,
    clang.cindex.CursorKind.CONSTRUCTOR,
    clang.cindex.CursorKind.DESTRUCTOR,
    clang.cindex.CursorKind.ENUM_DECL,
    #clang.cindex.CursorKind.UNION_DECL, # TODO
    clang.cindex.CursorKind.VAR_DECL, # TODO, static var
    clang.cindex.CursorKind.CONVERSION_FUNCTION
    )

Serialization_type = (
    'float',
    'double',
    'int',
    'uint32_t',
    'bool',
    'std::string',
    )

def internal_append_to_list_of_a_map(map, key, item):
    if key not in map:
        map[key] = [item]
    else:
        map[key].append(item)

def internal_get_annotation(node):
    for child in node.get_children():
        if child.kind == clang.cindex.CursorKind.ANNOTATE_ATTR:
            return child
    return None

def internal_parse_class(node):
    if not node.is_definition():
        return
    scope_prefix = '::'.join( node.type.spelling.split('::')[:-1] )
    if scope_prefix not in namespaces:
        return
    print('class:', node.type.spelling)
    header_file = os.path.basename( node.location.file.name )
    if node.spelling not in classes:
        classes[node.spelling] = {}
    classes[node.spelling]['scope_prefix'] = scope_prefix
    classes[node.spelling]['header_file'] = header_file
    for child in node.get_children():
        if child.kind == clang.cindex.CursorKind.CXX_BASE_SPECIFIER:
            base_class = next(child.get_children()).spelling
            base_class = base_class.split(' ')[-1].split('::')[-1]
            classes[node.spelling]['parent'] = base_class

        elif child.kind == clang.cindex.CursorKind.CXX_METHOD:
            #print('\t', 'method', child.spelling)
            pass
        # elif child.kind == clang.cindex.CursorKind.UNION_DECL:
        #     ''' eg. Vector4, Color...
        #     '''
        #     print('\t', 'union', child.spelling)
        #     for c in child.get_children():
        #         print('\t\t', c.spelling, c.kind)
        #         if c.kind == clang.cindex.CursorKind.STRUCT_DECL:
        #             for cc in c.get_children():
        #                 print('\t\t\t', cc.spelling, cc.kind, cc.type.spelling)
        #                 if cc.kind == clang.cindex.CursorKind.FIELD_DECL:
        #                     #annotation = internal_get_annotation(cc)
        #                     #if (annotation is not None) and (annotation.spelling == 'Serialize'):
        #                     #    internal_append_to_list_of_a_map(classes[node.spelling], 'member', {'name': cc.spelling, 'type': cc.type.spelling})
        #                     internal_append_to_list_of_a_map(classes[node.spelling], 'member', {'name': cc.spelling, 'type': cc.type.spelling})
                
        elif child.kind == clang.cindex.CursorKind.FIELD_DECL:
            print('\tFIELD_DECL', child.spelling, child.type.spelling)
            # for c in child.get_children():
            #     print('\t\t', c.spelling, c.kind)
            annotation = internal_get_annotation(child)
            #if child.type.spelling in Serialization_type:
            hidden = False
            if (annotation is not None) and (annotation.spelling == 'HideInInspector'):
                hidden = True
            if not hidden:
                internal_append_to_list_of_a_map(classes[node.spelling], 'member', {'name': child.spelling, 'type': child.type.spelling})
            # else:
            #     print("Unkown type,", child.type.spelling)

        elif child.kind in skip_class_internal_type:
            pass

        else:
            print('\t\t', child.kind, child.spelling)

def internal_find_typerefs(node):
    global classes
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

    if node.kind == clang.cindex.CursorKind.CLASS_DECL:
        internal_parse_class(node)

    for c in node.get_children():
        internal_find_typerefs(c)

def ExtractClasses(path):
    global classes
    classes = {}
    print(path)
    tu = index.parse(path, ['-x', 'c++', '-std=c++14', '-fsyntax-only', '-D__REFLECTION_PARSER__'])
    internal_find_typerefs(tu.cursor)
    return classes

if __name__ == "__main__":
    classes = ExtractClasses("temp/AllHeaders.hpp")
    #print(classes)
    #print(classes['Vector4'])
    #print(classes['Color'])
    #print(json.dumps(classes, indent=4))
    #print(json.dumps(classes['Animator'], indent=4))
    #if debug:
    print('dump classes')
    with open('temp/class.json', 'w') as f:
        f.write(json.dumps(classes, indent = 4))
    if debug:
        for key in count:
            print(key, count[key])
