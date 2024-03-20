# 设置cmake的最低版本要求
cmake_minimum_required(VERSION 3.20.0)

# 设置项目名称
project(Example_Project VERSION 0.1.0 LANGUAGES C CXX)

# 由于project已经设置了语言，所以设置语言的编译器和语言的标准是多余的
# set(CMAKE_CXX_COMPILE "gcc")
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(CTest)
# enable_testing()

# 添加可执行文件
add_executable(math src/add.cpp src/sub.cpp src/multi.cpp src/divv.cpp src/main.cpp)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# include(CPack)
