cmake_minimum_required(VERSION 3.8.0)

set(CMAKE_USER_MAKE_RULES_OVERRIDE
    ${CMAKE_SOURCE_DIR}/scripts/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
   ${CMAKE_SOURCE_DIR}/scripts/cxx_flag_overrides.cmake)

project(wkhtmltox)

# 输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

# 可执行文件后缀
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "d")

# 库文件后缀
set(CMAKE_DEBUG_POSTFIX "d")

# 取消CMake默认生成的工程选项，仅保留RelWithDebugInfo与Release，默认不构建Debug版的wk-Qt4静态库
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES RelWithDebInfo Release)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Reset the configurations to what we need"
        FORCE)
endif()

# wkhtmltox
add_subdirectory(src/lib)
# watermark tester
add_subdirectory(src/watermark_test)