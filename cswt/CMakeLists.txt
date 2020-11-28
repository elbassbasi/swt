project(cswt VERSION 1.0)

    file(GLOB src_custom_widgets "widgets/*.c")
    file(GLOB src_custom_controls "controls/*.c")
    file(GLOB src_custom_themes "themes/*.c")
set(src_custom ${src_custom_widgets} ${src_custom_controls} ${src_custom_themes})

    add_library(cswt SHARED ${src_custom})
    target_include_directories(cswt PRIVATE ../include)
	target_compile_definitions(cswt PRIVATE CSWTBUILD)
    target_link_libraries(cswt swt)