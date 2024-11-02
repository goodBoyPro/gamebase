CMAKE_MINIMUM_REQUIRED(VERSION 3.26.4)
set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME game)
project(${PROJECT_NAME})
########################################################################################
#设置项目绝对路径
# set(ABS_DIR G:/codeLearn/vsCode/gameX)
set(ABS_DIR ../)

get_filename_component(ABS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ABS_DIR} ABSOLUTE)
message(RootDirectory:${ABS_DIR})
########################################################################################
SET(CMAKE_CXX_COMPILE_FEATURES "gcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK ")

include_directories(
    ${PROJECT_SOURCE_DIR}/work/head
    ${ABS_DIR}/gamebase/extern/include
   ${ABS_DIR}/engine/head
   ${ABS_DIR}/gamebase/engineEdit/head
   ${ABS_DIR}/gamebase/lib/head
   ${ABS_DIR}/gamebase/workCode/    #测试用 完成后去掉
)

#自定义设置
# set(SFML_DIR ../extern/sfml/lib/cmake/SFML)
# find_package(SFML COMPONENTS window system graphics network audio REQUIRED)
add_definitions(-DUNICODE )  #预定义前需要加-D

set(CMAKE_BUILD_RPATH ${ABS_DIR}/build)

#启动程序
set(EXECUTABLE_OUTPUT_PATH ${ABS_DIR}/gamebase/output)
add_executable(Start start.cpp version.rc)

add_subdirectory(./work/src)