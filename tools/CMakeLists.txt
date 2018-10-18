include(ExternalProject)

file(GLOB tools RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(tool ${tools})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${tool} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${tool}/CMakeLists.txt)
        if (CMAKE_CROSSCOMPILING)
            set(hostTargetName Tunis${tool})
            set(externalProjectName ExternalProject_${hostTargetName})
            get_filename_component(suffix ${CMAKE_COMMAND} EXT)
            set(hostBinaryName "${hostTargetName}${suffix}")
            ExternalProject_add(${externalProjectName}
                SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${tool}
                INSTALL_COMMAND ""
                BUILD_BYPRODUCTS <BINARY_DIR>/${hostBinaryName})
            ExternalProject_Get_Property(${externalProjectName} BINARY_DIR)
            add_executable(${hostTargetName} IMPORTED GLOBAL)
            set_property(TARGET ${hostTargetName} PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/${hostBinaryName})
            add_dependencies(${hostTargetName} ${externalProjectName})
        else()
            add_subdirectory(${tool})
        endif()
    endif()
endforeach()
