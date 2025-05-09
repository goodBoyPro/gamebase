#主程序
set(EXECUTABLE_OUTPUT_PATH ${ABS_DIR}/gamebase/output/bin)

file(GLOB_RECURSE src   ${CMAKE_CURRENT_SOURCE_DIR}/*.c *.cpp 
                        ${ABS_DIR}/engine/src/*.cpp *.c
                        ${ABS_DIR}/gamebase/engineEdit/src/*.cpp *.c
                        ${ABS_DIR}/gamebase/engineEdit/mainGame/*.cpp *.c
                        ${ABS_DIR}/gamebase/workCode/*.cpp *.c)

file(GLOB_RECURSE srcEditor   ${CMAKE_CURRENT_SOURCE_DIR}/*.c *.cpp 
                        ${ABS_DIR}/engine/src/*.cpp *.c
                        ${ABS_DIR}/gamebase/engineEdit/src/*.cpp *.c
                        ${ABS_DIR}/gamebase/engineEdit/mainEditor/*.cpp *.c
                        ${ABS_DIR}/gamebase/workCode/*.cpp *.c)

#aux_source_directory(. src)
#游戏exe
if(1)
link_directories(${ABS_DIR}/gamebase/lib/outlib
                ${ABS_DIR}/gamebase/extern/lib
                ${ABS_DIR}/gamebase/extern/lib/sfml)
add_executable(${PROJECT_NAME}  ${src} 
                                ${ABS_DIR}/gamebase/version.rc)
target_link_libraries(${PROJECT_NAME} opengl32 sfml-graphics sfml-window sfml-system sfml-audio timemanager winmm glew32 glfw3dll)
endif()
#编辑器exe
if(0)
link_directories(${ABS_DIR}/gamebase/lib/outlib
                ${ABS_DIR}/gamebase/extern/lib
                ${ABS_DIR}/gamebase/extern/lib/sfml)
add_executable(${PROJECT_NAME}Editor  ${srcEditor} 
                                ${ABS_DIR}/gamebase/version.rc)
target_include_directories(${PROJECT_NAME}Editor  PUBLIC ${ABS_DIR}/gamebase/engineEdit/mainEditor)
target_link_libraries(${PROJECT_NAME}Editor opengl32 sfml-graphics sfml-window sfml-system sfml-audio timemanager winmm glew32 glfw3dll ws2_32)
endif()
#生成库文件
if(0)  
# add_definitions(-DEXPORT )
set(libname timeManager)#设置
add_library(${libname} SHARED ${ABS_DIR}/gamebase/lib/src/${libname}.cpp)#设置
target_compile_definitions(${libname} PRIVATE -DEXPORT)
target_link_libraries(${libname}  winmm)
add_custom_command(TARGET ${libname} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy lib${libname}.dll lib${libname}.dll.a ${ABS_DIR}/gamebase/lib/outlib
                    COMMAND ${CMAKE_COMMAND} -E copy lib${libname}.dll ${ABS_DIR}/gamebase/output/bin)                    
endif()
