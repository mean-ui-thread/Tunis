include(TunisSampleMain${TUNIS_SAMPLES_MAIN}.cmake)

add_library(TunisSampleCommon
    TunisSampleMain${TUNIS_SAMPLES_MAIN}.cmake
    SampleApp.cpp
    SampleApp.h
)

target_link_libraries(TunisSampleMain PUBLIC Tunis)
target_link_libraries(TunisSampleCommon PUBLIC TunisSampleMain)
target_include_directories(TunisSampleCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

