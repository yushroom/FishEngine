http://szelei.me/code-generator/

https://www.youtube.com/watch?v=QngYWfNXWeI

https://nativecoding.wordpress.com/2016/10/25/c-reflection-using-clang/

http://llvm.org/devmtg/2012-04-12/Slides/Wayne_Palmer.pdf

https://github.com/AustinBrunkhorst/CPP-Reflection

https://github.com/Celtoys/clReflect



## How to get clang include paths

http://stackoverflow.com/questions/22480534/how-to-use-libclang-with-stl

echo "" | g++ -v -x c++ -E -



http://stackoverflow.com/questions/11946294/dump-include-paths-from-g/11946295#11946295

clang++ -E -x c++ - -v < /dev/null



## libclang

为了加速用libclang的解析，首先将所有的.hpp include到一个临时文件（AllHeaders.hpp），然后解析会快很多。碰到一个问题，对于std::vector和std::map之类的stl容器（或者说是forward declared的类型？）解析会遇到问题，libclang会判断成int，但是如果说是error type应该是直接不存在AST中才对。比如，如果不设置系统include的路径，大部分都不会进AST。奇怪的是直接在命令行中clang -E得到的预处理之后的文件再去解析的话类型全是对的。