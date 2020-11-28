    file(GLOB src_common_core "common/core/*.c")
    file(GLOB src_common_runtime "common/runtime/*.c")
    file(GLOB src_common_graphics "common/graphics/*.c")
    file(GLOB src_common_widgets "common/widgets/*.c")
    file(GLOB src_common_controls "common/controls/*.c")
    file(GLOB src_common_layouts "common/layouts/*.c")

set(src_common ${src_common_core} ${src_common_runtime} 
	${src_common_graphics} ${src_common_widgets} ${src_common_controls} 
	${src_common_layouts} )

if (WIN32)
add_subdirectory(windows)
endif(WIN32)

if (UNIX AND NOT APPLE)
 add_subdirectory(gtk)
endif()

if (UNIX AND APPLE)
 add_subdirectory(cocoa)
endif()