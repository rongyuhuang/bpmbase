QT_CPU_FEATURES.i386 = sse sse2
QT.global_private.enabled_features = alloca_malloc_h alloca sse2 gui network release_tools sql testlib widgets xml
QT.global_private.disabled_features = alloca_h android-style-assets private_tests dbus dbus-linked libudev posix_fallocate qml-debug reduce_exports reduce_relocations stack-protector-strong system-zlib
QT_COORD_TYPE = double
CONFIG += sse2 sse3 ssse3 sse4_1 sse4_2 avx avx2 f16c force_debug_info largefile msvc_mp precompile_header
QT_BUILD_PARTS += libs tools
QT_HOST_CFLAGS_DBUS += 
