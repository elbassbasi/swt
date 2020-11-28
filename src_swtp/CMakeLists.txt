project(swtp VERSION 1.0)

    file(GLOB src_swtp "*.cpp")

    add_library(swtp STATIC  ${src_swtp})
    target_include_directories(swtp PRIVATE ..)
	target_compile_definitions(swtp PRIVATE -DSWTPBUILD)