cmake_minimum_required(VERSION 3.8.0)

add_executable(Tester Tester.cpp)
target_link_libraries(Tester wkhtmltox)