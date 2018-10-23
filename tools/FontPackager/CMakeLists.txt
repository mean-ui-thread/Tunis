project(TunisFontPackager)

hunter_add_package(msdfgen)
hunter_add_package(PocoCpp)
hunter_add_package(OpenSSL)
hunter_add_package(RapidJSON)
hunter_add_package(PNG)

find_package(msdfgen CONFIG REQUIRED)
find_package(Poco REQUIRED NetSSL CONFIG)
find_package(OpenSSL REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(PNG CONFIG REQUIRED)

list(APPEND deps
    msdfgen::lib_msdfgen
    Poco::Net
    Poco::NetSSL
    OpenSSL::SSL
    OpenSSL::Crypto
    RapidJSON::rapidjson
    PNG::png
    TunisFonts
)

find_package(OpenMP)
if (TARGET OpenMP::OpenMP_CXX)
    list(APPEND deps OpenMP::OpenMP_CXX)
endif()

add_executable(${PROJECT_NAME}
    TunisFontGenerator.cpp
    TunisFontGenerator.h
    TunisFontLoader.cpp
    TunisFontLoader.h
    TunisGlyphLoader.cpp
    TunisGlyphLoader.h
    main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${Tunis_SOURCE_DIR}/include
        ${Tunis_BINARY_DIR}/schema
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${deps})

