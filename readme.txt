2017-10-28
1. 增加myboost qmake qtbase：界面就用qtbase，其他主要用
    stl+boost，qtbase的容器很差，boost的system和asio可以
    用用，日期用用abseil，格式化可以用fmt

2017-10-23
1. 增加folly的port层代码，里面有很多相对于posix而已，
    win32实现不规范的地方，比如gettimeofday strptime 
    clock_gettime clock等
2017-10-20
1. 清理utils相关代码

2017-10-18
1. 增加python通用脚本
2. 移植qtbinpatcher

2017-10-17
1. 采用vcpkg的x86-windows-static (vcbuildtools 2017)编译
2. 使用vcpkg安装，如: vcpkg install fmt:x86-windows-static
   fmt gflags glog gtest libevent evpp nana zeromq imgui
   记得修改msvc.pri里面的VCPKG路径
