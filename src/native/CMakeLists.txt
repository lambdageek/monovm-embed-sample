project(native)
cmake_minimum_required(VERSION 3.22)

set (MONO_INCLUDE_DIR "../../vendor/mono-headers/include/mono-2.0")

add_library(libmono-2.0 SHARED IMPORTED)
set_property(TARGET libmono-2.0 PROPERTY IMPORTED_LOCATION "../../out/native/libcoreclr.so")
target_include_directories(libmono-2.0 INTERFACE "${MONO_INCLUDE_DIR}")

cmake_path (SET CORELIB_FULL_PATH "../../out/native/System.Private.CoreLib.dll")
cmake_path (ABSOLUTE_PATH CORELIB_FULL_PATH)

add_executable(main main.c)
target_include_directories(main PRIVATE "../../out/native/generated/")
target_compile_definitions(main PRIVATE "CORELIB_FULL_PATH=\"${CORELIB_FULL_PATH}\"")
target_link_libraries(main libmono-2.0)

cmake_path (SET CMAKE_INSTALL_PREFIX "../../out/native")
cmake_path (ABSOLUTE_PATH CMAKE_INSTALL_PREFIX)
install(TARGETS main RUNTIME DESTINATION .)