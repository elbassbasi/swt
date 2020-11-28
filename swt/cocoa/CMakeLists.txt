    file(GLOB src_cocoa_runtime "runtime/*.c")
    file(GLOB src_cocoa_graphics "graphics/*.c")
    file(GLOB src_cocoa_graphics_m "graphics/*.m")
    file(GLOB src_cocoa_widgets "widgets/*.c")
    file(GLOB src_cocoa_widgets_m "widgets/*.m")
    file(GLOB src_cocoa_controls "controls/*.c")
    file(GLOB src_cocoa_controls_m "controls/*.m")
set(src_cocoa ${src_cocoa_runtime} ${src_cocoa_graphics} ${src_cocoa_graphics_m} ${src_cocoa_widgets} ${src_cocoa_widgets_m} ${src_cocoa_controls} ${src_cocoa_controls_m})

    
    find_package(OpenGL REQUIRED)
    find_library(COCOA_LIBRARY Cocoa)

    add_library(swt SHARED ${src_cocoa} ${src_common})
    target_include_directories(swt PRIVATE ../../swt ${OPENGL_INCLUDE_DIRS})
	target_compile_definitions(swt PRIVATE SWTBUILD)
    target_link_libraries(swt ${COCOA_LIBRARY} ${OPENGL_LIBRARIES})
