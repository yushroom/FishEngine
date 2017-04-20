from __future__ import print_function
import os, glob
from tool_helper import UpdateFile

blacklist = (
	"AssetDataBase",
	"Editor",
	"IPreviewable",
	"SerializedObject",
	"SerializedProperty",
	"SerializedPropertyType",
	"AssetImporterInspector",
)

def GetEngineHeadersTo(output_file_path):
    all_headers = glob.glob("../../Engine/Source/Runtime/*.hpp")
    lines = ['#include "{}"'.format(os.path.abspath(x)) for x in all_headers]
    string = '\n'.join(lines)
    UpdateFile(output_file_path, string)

def GetAllHeadersTo(output_file_path):
    all_headers = glob.glob("../../Engine/Source/Runtime/*.hpp")
    all_headers += glob.glob("../../Engine/Source/Editor/*.hpp")
    lines = ['#include "{}"'.format(os.path.abspath(x)) for x in all_headers if os.path.basename(x).split('.')[0] not in blacklist]
    string = '\n'.join(lines)
    UpdateFile(output_file_path, string)

if __name__ == "__main__":
    GetAllHeadersTo("temp/AllHeaders.hpp")