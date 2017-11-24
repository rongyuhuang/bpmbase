by和哥 2017

准备
======
1 vc build tools(vc2017 u3) + windows sdk 10.0.16299

2 安装python27-x86 和activeperl512-x86
http://www.activestate.com/activeperl/
http://www.activestate.com/activepython/

3 下载qtbase
http://download.qt.io/official_releases/qt/5.9/5.9.2/submodules/

4 编译
打开vc的x86 native tools命令行

qtbase：x86 static + release编译
======
configure -prefix "C:\qtbase" -confirm-license -opensource -release -force-debug-info -static -static-runtime -accessibility -nomake tests -nomake examples -no-compile-examples -no-freetype -no-opengl -no-qml-debug -no-icu -no-angle -qt-zlib -qt-libpng -qt-libjpeg -no-openssl -no-dbus -no-style-fusion -mp -platform win32-msvc2017
nmake
nmake install

qtbase：x86 static + release/debug编译
======
configure -prefix "C:\qtbase" -confirm-license -opensource -debug-and-release -force-debug-info -static -static-runtime -accessibility -nomake tests -nomake examples -no-compile-examples -no-freetype -no-opengl -no-qml-debug -no-icu -no-angle -qt-zlib -qt-libpng -qt-libjpeg -no-openssl -no-dbus -no-style-fusion -mp -platform win32-msvc2017
nmake
nmake install

说明：qmake现在支持utf8_source了，config+=utf8_source就强制了代码是utf-8编码

5 配置qtcreator
下载4.4.1版本qt
http://download.qt.io/official_releases/qt-creator/
地球人都知道怎么搞

（完）