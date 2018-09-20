cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED SAMPLE_NAME)
    message(FATAL_ERROR "Missing mandatory -DSAMPLE_NAME=<name> parameter")
endif()

##
# prepend the new sample count to the sample name.
##
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/template/sample_count sample_count)
MATH(EXPR sample_count "${sample_count}+1")
set(SAMPLE_NAME "${sample_count}_${SAMPLE_NAME}")

##
# Generate the sample
##
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/template/CMakeLists.txt.in ${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_NAME}/CMakeLists.txt @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/template/SampleApp.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_NAME}/SampleApp.cpp @ONLY)

##
# Add the sample to the list of samples
##
file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt "add_subdirectory(${SAMPLE_NAME})")

##
# Save the sample count
##
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/template/sample_count "${sample_count}")
