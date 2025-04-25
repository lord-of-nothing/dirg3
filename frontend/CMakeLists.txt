set(UI_HEADERS
	mainwindow/mainwindow.ui
	editor/editor.ui
)

set(HEADERS
	mainwindow/mainwindow.h
	editor/editor.h
	area/area.h
	linkeditor/linkeditor.h
)

set(SOURCES 
	mainwindow/mainwindow.cpp
	editor/editor.cpp
	area/area.cpp
	linkeditor/linkeditor.cpp
	main.cpp
)

qt_add_executable(${PROJECT_NAME}
	MANUAL_FINALIZATION
	${SOURCES}
	${HEADERS}
	${UI_HEADERS}
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOGEN_ORIGIN_DEPENDS OFF)

add_custom_target(run
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}$<IF:${WIN32},.exe,>
)

add_dependencies(run ${PROJECT_NAME})

target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	Qt6::Core 
	Qt6::Widgets 
	Qt6::Gui 
	geometry
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	area
	editor
	mainwindow
	linkeditor
)

