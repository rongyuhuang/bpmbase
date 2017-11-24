by和哥 2015-2016

准备
======
1 vc build tools(vc2015 sp3) + windows sdk 10.0

2 安装python27-x86 和activeperl512-x86
http://www.activestate.com/activeperl/
http://www.activestate.com/activepython/

3 下载qtbase
http://download.qt.io/official_releases/qt/5.8/5.8.0/submodules/

4 打补丁
C:\projects\qtbase-opensource-src-5.8.0\mkspecs\common\msvc-base.conf
+ -utf-8

greaterThan(MSC_VER, 1899) {
    # Visual Studio 2015 (14.0) / Visual C++ 19.0 and up
    QMAKE_CFLAGS       += -Zc:strictStrings -utf-8
    QMAKE_CFLAGS_WARN_ON += -w44456 -w44457 -w44458
    QMAKE_CFLAGS_AVX2   = -arch:AVX2
    QMAKE_CXXFLAGS     += -Zc:strictStrings -Zc:throwingNew -utf-8
    QMAKE_CXXFLAGS_WARN_ON += -w44456 -w44457 -w44458 -wd4577 -wd4467
}

5 编译
打开vc的x86 native tools命令行
qtbase：x86 staic +release编译
======
configure -prefix "C:\qtbase" -confirm-license -opensource -release -force-debug-info -static -static-runtime -accessibility -nomake tests -nomake examples -no-compile-examples -no-freetype -no-opengl -no-qml-debug -no-icu -no-angle -qt-zlib -qt-libpng -qt-libjpeg -no-openssl -no-dbus -no-style-fusion -mp -platform win32-msvc2015
nmake
nmake install
nmake docs
nmake install_docs

6 配置qtcreator
下载4.2.1版本qt
http://download.qt.io/official_releases/qt-creator/
地球人都知道怎么搞

（完）