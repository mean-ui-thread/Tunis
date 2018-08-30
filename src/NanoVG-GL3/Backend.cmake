
function(tunis_backend_dependencies)

    add_library(nanovg
        3rdparty/nanovg/src/fontstash.h
        3rdparty/nanovg/src/nanovg.c
        3rdparty/nanovg/src/nanovg.h
        3rdparty/nanovg/src/nanovg_gl.h
        3rdparty/nanovg/src/nanovg_gl_utils.h
        3rdparty/nanovg/src/stb_image.h
        3rdparty/nanovg/src/stb_truetype.h
    )

    target_include_directories(nanovg PUBLIC 3rdparty/nanovg/src)

    target_link_libraries(Tunis PUBLIC nanovg)

endfunction()
