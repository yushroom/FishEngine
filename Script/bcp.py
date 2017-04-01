from __future__ import print_function
import os

boost_headers = (
	"boost/lexical_cast.hpp",
	"boost/uuid/uuid.hpp",
	"boost/uuid/uuid_generators.hpp",
	"boost/algorithm/string.hpp",
	"boost/filesystem.hpp",
	"boost/any.hpp",
	"boost/dll.hpp",
	"boost/format.hpp"
	)

output_dir = R"D:\program\github\FishEngine\Engine\ThirdParty\boost"

if not os.path.exists(output_dir):
	os.makedirs(output_dir)

boost_root = R"D:\misc\boost_1_63_0\boost_1_63_0"
bcp_exe = boost_root + R"\dist\bin\bcp"

cmd = bcp_exe + ' --boost=' + boost_root + ' ' + ' '.join(boost_headers) + ' ' + output_dir
print(cmd)
os.system(cmd)