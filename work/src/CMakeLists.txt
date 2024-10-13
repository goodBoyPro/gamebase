#主程序
set(EXECUTABLE_OUTPUT_PATH ${ABS_DIR}/output/bin)

file(GLOB_RECURSE src   ${CMAKE_CURRENT_SOURCE_DIR}/*.c *.cpp 
                        ${ABS_DIR}/engine/src/*.cpp *.c
                        ${ABS_DIR}/project/engineEdit/src/*.cpp *.c)

#aux_source_directory(. src)

if(0)
link_directories(${ABS_DIR}/lib)
add_executable(${PROJECT_NAME} 
                                ${src} 
                                ${ABS_DIR}/project/version.rc)
target_link_libraries(${PROJECT_NAME} opengl32 sfml-graphics sfml-window sfml-system sfml-audio timer winmm glew32 glfw3dll)

#生成库文件
else()
add_definitions(-DEXPORT )
set(libname timer)#设置
add_library(${libname} SHARED ${ABS_DIR}/project/lib/${libname}.cpp)#设置
target_link_libraries(${libname}  winmm)
add_custom_command(TARGET timer POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy lib${libname}.dll lib${libname}.dll.a ${ABS_DIR}/lib
                    COMMAND ${CMAKE_COMMAND} -E copy lib${libname}.dll ${ABS_DIR}/output/bin)
                    
endif()
