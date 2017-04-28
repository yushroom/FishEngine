import os
from mako.template import Template


CMakeLists_txt_tempalte_str2 = '''
# Target at least C++14
set(CMAKE_CXX_STANDARD 14)

# aux_source_directory( ${CMAKE_CURRENT_LIST_DIR} SRCS )
# FILE(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/Assets/*.hpp)

SET(FishEngineRootDir ${CMAKE_CURRENT_LIST_DIR}/../../Engine)
IF (MSVC)
	SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${FishEngineRootDir}/ThirdParty/boost/)
	SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${FishEngineRootDir}/ThirdParty/boost/lib64-msvc-14.0)
ENDIF()

set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.59 REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/Assets)
INCLUDE_DIRECTORIES(${FishEngineRootDir}/Source/Runtime)
LINK_DIRECTORIES(${FishEngineRootDir}/Binary/lib/Debug)

add_library (${Project_Name} SHARED ${SRCS} ${HEADERS} ${Serialization_SRCS})
target_link_libraries(${Project_Name} FishEngine)
'''


project_generate_cpp_template_str = '''#include <iostream>
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT
//#include <boost/dll/alias.hpp> // for BOOST_DLL_ALIAS

% for h in headers:
#include "${h}" 
% endfor

// `extern "C"` - specifies C linkage: forces the compiler to export function/variable by a pretty (unmangled) C name.
#define API extern "C" BOOST_SYMBOL_EXPORT

% for c in classNames:
using ${c}Ptr = std::shared_ptr<${c}>;
% endfor

API FishEngine::Script* CreateCustomScript(const char* className)
{
	std::cout << className << std::endl;
% for c in classNames:
	if (std::string(className) == "${c}")
	{
		return new ${c}();
	}
% endfor
	return nullptr;
}

API void DestroyCustomScript(FishEngine::Script * script)
{
	delete script;
}
'''
project_generate_cpp_template = Template(project_generate_cpp_template_str)

def UpdateFile(out_path, content):
	need_update = True

	directory = os.path.dirname(out_path)
	if not os.path.exists(directory):
		os.makedirs(directory)

	if os.path.exists(out_path):
		with open(out_path) as f:
			old_content = f.read()
			need_update = (content != old_content)
	if need_update:
		print("update", out_path)
		with open(out_path, 'w') as f:
			f.write(content)
	else:
		print("no update", out_path)

def internal_prepareCMakeList_txt():
	pass

def prepareProject(project_path):
	project_name = os.path.split(project_path)[-1]

	# get all source files
	list_dirs = os.walk( os.path.join(project_path, 'Assets') )
	headers, sources, classNames = [], [], []
	for root, dirs, files in list_dirs:
		for f in files:
			path = os.path.join(root, f)
			path = os.path.relpath(path, project_path)
			path = path.replace('\\', '/')
			ext = os.path.splitext(f)[1]
			if ext == '.hpp' or ext == '.h':
				headers.append(path)
				classNames.append(os.path.splitext(f)[0])
			elif ext == '.cpp':
				sources.append(path)
	print( headers, sources, classNames )

	# prepare CMakeList.txt
	CMakeLists_txt_tempalte = '''cmake_minimum_required(VERSION 3.0)\nset(Project_Name {0})\nSET(SRCS "./project.generate.cpp")\n'''.format(project_name)
	CMakeLists_txt_tempalte += '\n'.join(['SET(HEADERS ${{HEADERS}} "{0}")'.format(h) for h in headers])
	CMakeLists_txt_tempalte += '\n'.join(['SET(SRCS ${{SRCS}} {0})\n'.format(c) for c in sources])
	CMakeLists_txt_tempalte += CMakeLists_txt_tempalte_str2
	UpdateFile( os.path.join(project_path, 'CMakeLists.txt'), CMakeLists_txt_tempalte)
	
	# prepare project.gererate.cpp
	project_generate_cpp_content = project_generate_cpp_template.render(headers=headers, classNames=classNames)
	UpdateFile( os.path.join(project_path, 'project.generate.cpp'), project_generate_cpp_content )


def buildProject(project_path, build_type):
	prepareProject( project_path )

	build_path = os.path.join( project_path, 'build' )
	if not os.path.exists(build_path):
		os.makedirs(build_path)
	cmd = 'cd {0} & cmake -G "Visual Studio 14 Win64" ..'.format(build_path)
	print(cmd)
	result = os.system(cmd)
	log_file_path = build_path + '/../build.log'
	cmd = 'cmake --build {0} --config {1} > {2}'
	cmd = cmd.format(build_path, build_type, log_file_path)
	print(cmd)
	result = os.system(cmd)
	if result == 0:
		print('build succeeded')
	else:
		print('build failed. see log file in ' + build_path)
	return result

if __name__ == "__main__":
	project_path = r'D:\program\github\FishEngine\Example\TestScript'
	# buildProject(project_path, 'RelWithDebInfo')
	# buildProject(project_path, 'Release')
	buildProject(project_path, 'Debug')