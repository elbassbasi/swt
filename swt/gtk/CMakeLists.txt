    file(GLOB src_gtk_runtime "runtime/*.c")
    file(GLOB src_gtk_graphics "graphics/*.c")
    file(GLOB src_gtk_widgets "widgets/*.c")
    file(GLOB src_gtk_controls "controls/*.c")
set(src_gtk ${src_gtk_runtime} ${src_gtk_graphics} 
	${src_gtk_widgets} ${src_gtk_controls})

    FIND_PACKAGE(PkgConfig REQUIRED)
    PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
    find_package(OpenGL REQUIRED)

    add_library(swt SHARED ${src_gtk} ${src_common})
    target_include_directories(swt PRIVATE ../../include ${GTK3_INCLUDE_DIRS} 
    										${OPENGL_INCLUDE_DIRS})
	target_compile_definitions(swt PRIVATE SWTBUILD)
    target_link_libraries(swt ${GTK3_LIBRARIES} ${OPENGL_LIBRARIES})