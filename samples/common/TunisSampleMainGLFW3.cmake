hunter_add_package(glfw)
find_package(glfw3 REQUIRED)

add_library(TunisSampleMain
    mainGLFW3.cpp
)

target_link_libraries(TunisSampleMain PUBLIC glfw)
