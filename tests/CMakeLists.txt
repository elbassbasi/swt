#main
file(GLOB test_main "main/*.cpp")
#controls
file(GLOB test_controls_main "controls/*.cpp")
file(GLOB test_controls_browser "controls/Browser/*.cpp")
file(GLOB test_controls_button "controls/Button/*.cpp")
file(GLOB test_controls_combo "controls/Combo/*.cpp")
file(GLOB test_controls_coolbar "controls/CoolBar/*.cpp")
file(GLOB test_controls_datetime "controls/DateTime/*.cpp")
file(GLOB test_controls_expandbar "controls/ExpandBar/*.cpp")
file(GLOB test_controls_group "controls/Group/*.cpp")
file(GLOB test_controls_label "controls/Label/*.cpp")
file(GLOB test_controls_link "controls/Link/*.cpp")
file(GLOB test_controls_list "controls/List/*.cpp")
file(GLOB test_controls_progressbar "controls/ProgressBar/*.cpp")
file(GLOB test_controls_sash "controls/Sash/*.cpp")
file(GLOB test_controls_scale "controls/Scale/*.cpp")
file(GLOB test_controls_slider "controls/Slider/*.cpp")
file(GLOB test_controls_spinner "controls/Spinner/*.cpp")
file(GLOB test_controls_tabfolder "controls/TabFolder/*.cpp")
file(GLOB test_controls_table "controls/Table/*.cpp")
file(GLOB test_controls_text "controls/Text/*.cpp")
file(GLOB test_controls_toolbar "controls/ToolBar/*.cpp")
file(GLOB test_controls_tree "controls/Tree/*.cpp")
set(test_controls ${test_controls_main} ${test_controls_browser} ${test_controls_button} ${test_controls_combo} 
${test_controls_coolbar} ${test_controls_datetime} ${test_controls_expandbar} ${test_controls_group} ${test_controls_label}
 ${test_controls_link} ${test_controls_list} ${test_controls_progressbar} ${test_controls_sash} ${test_controls_scale}
  ${test_controls_slider} ${test_controls_spinner} ${test_controls_tabfolder} ${test_controls_table} ${test_controls_text} 
  ${test_controls_toolbar} ${test_controls_tree})
#graphics
file(GLOB test_graphics_main "graphics/*.cpp")
file(GLOB test_graphics_cursor "graphics/Cursor/*.cpp")
file(GLOB test_graphics_font "graphics/Font/*.cpp")
file(GLOB test_graphics_gc "graphics/GC/*.cpp")
file(GLOB test_graphics_image "graphics/Image/*.cpp")
file(GLOB test_graphics_imagelist "graphics/ImageList/*.cpp")
file(GLOB test_graphics_path "graphics/Path/*.cpp")
file(GLOB test_graphics_pattern "graphics/Pattern/*.cpp")
file(GLOB test_graphics_region "graphics/Region/*.cpp")
file(GLOB test_graphics_textlayout "graphics/TextLayout/*.cpp")
file(GLOB test_graphics_transform "graphics/Transform/*.cpp")
set(test_graphics ${test_graphics_main} ${test_graphics_cursor} ${test_graphics_font} ${test_graphics_gc} 
${test_graphics_image} ${test_graphics_imagelist} ${test_graphics_path} ${test_graphics_pattern} 
${test_graphics_region} ${test_graphics_textlayout} ${test_graphics_transform})
#runtime
file(GLOB test_runtime "runtime/*.cpp")
#widgets
file(GLOB test_widgets_main "widgets/*.cpp")
file(GLOB test_widgets_canvas "widgets/Canvas/*.cpp")
file(GLOB test_widgets_caret "widgets/Caret/*.cpp")
file(GLOB test_widgets_composite "widgets/Composite/*.cpp")
file(GLOB test_widgets_control "widgets/Control/*.cpp")
file(GLOB test_widgets_dialogs "widgets/Dialogs/*.cpp")
file(GLOB test_widgets_dnd "widgets/Dnd/*.cpp")
file(GLOB test_widgets_glcanvas "widgets/GLCanvas/*.cpp")
file(GLOB test_widgets_menu "widgets/Menu/*.cpp")
file(GLOB test_widgets_scrollable "widgets/Scrollable/*.cpp")
file(GLOB test_widgets_shell "widgets/Shell/*.cpp")
file(GLOB test_widgets_taskbar "widgets/TaskBar/*.cpp")
file(GLOB test_widgets_theme "widgets/Theme/*.cpp")
file(GLOB test_widgets_toolkit "widgets/Toolkit/*.cpp")
file(GLOB test_widgets_tooltip "widgets/ToolTip/*.cpp")
file(GLOB test_widgets_tracker "widgets/Tracker/*.cpp")
file(GLOB test_widgets_tray "widgets/Tray/*.cpp")

set(test_widgets ${test_widgets_main} ${test_widgets_canvas} ${test_widgets_caret} ${test_widgets_composite} 
${test_widgets_control} ${test_widgets_dialogs} ${test_widgets_dnd} ${test_widgets_glcanvas} ${test_widgets_menu} ${test_widgets_scrollable} 
${test_widgets_shell} ${test_widgets_taskbar} ${test_widgets_theme} ${test_widgets_toolkit} ${test_widgets_tooltip}
 ${test_widgets_tracker} ${test_widgets_tray})
#resource
set(test_res )
if (WIN32)
if(MINGW)
  set(CMAKE_RC_COMPILER_INIT windres)
  ENABLE_LANGUAGE(RC)
  SET(CMAKE_RC_COMPILE_OBJECT
    "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
	file(GLOB test_res "main/*.rc")
endif(MINGW)
endif(WIN32)
find_package(OpenGL REQUIRED)
add_executable(swttest ${test_main} ${test_controls} ${test_graphics} ${test_runtime} ${test_widgets} ${test_res})
target_include_directories(swttest PRIVATE .. ${OPENGL_INCLUDE_DIRS})
target_link_libraries(swttest swt cswt swtp ${OPENGL_LIBRARIES})

 add_custom_target(swttest-copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/images
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/images
    )