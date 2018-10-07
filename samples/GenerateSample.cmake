##
# MIT License
#
# Copyright (c) 2018 Matt Chiasson
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
##
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
# Get the current year
##
string(TIMESTAMP SAMPLE_YEAR %Y)

##
# Generate the sample
##
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/template/CMakeLists.txt.in ${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_NAME}/CMakeLists.txt @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/template/SampleApp.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_NAME}/SampleApp.cpp @ONLY)

##
# Add the sample to the list of samples
##
file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt "add_subdirectory(${SAMPLE_NAME})\n")

##
# Save the sample count
##
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/template/sample_count "${sample_count}")
