include(TunisSampleMain${TUNIS_SAMPLES_MAIN}.cmake)

add_library(TunisSampleCommon
    SampleApp.cpp
)

target_link_libraries(TunisSampleCommon PUBLIC TunisSampleMain Tunis)
target_include_directories(TunisSampleCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

