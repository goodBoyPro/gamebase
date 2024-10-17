#主程序
set(EXECUTABLE_OUTPUT_PATH ${ABS_DIR}/project/output/bin)

file(GLOB_RECURSE src   ${CMAKE_CURRENT_SOURCE_DIR}/*.c *.cpp 
                        ${ABS_DIR}/engine/src/*.cpp *.c
                        ${ABS_DIR}/project/engineEdit/src/*.cpp *.c)

#aux_source_directory(. src)

if(1)
link_directories(${ABS_DIR}/project/lib/outlib
                ${ABS_DIR}/project/extern/lib
                ${ABS_DIR}/project/extern/lib/sfml)
add_executable(${PROJECT_NAME} 
                                ${src} 
                                ${ABS_DIR}/project/version.rc)
target_link_libraries(${PROJECT_NAME} opengl32 sfml-graphics sfml-window sfml-system sfml-audio timer winmm glew32 glfw3dll)
endif()
#生成库文件
if(0)
add_definitions(-DEXPORT )
set(libname timer)#设置
add_library(${libname} SHARED ${ABS_DIR}/project/lib/src/${libname}.cpp)#设置
target_link_libraries(${libname}  winmm)
add_custom_command(TARGET timer POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy lib${libname}.dll lib${libname}.dll.a ${ABS_DIR}/project/lib/outlib
                    COMMAND ${CMAKE_COMMAND} -E copy lib${libname}.dll ${ABS_DIR}/project/output/bin)                    
endif()
