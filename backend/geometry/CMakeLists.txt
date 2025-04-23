cmake_minimum_required(VERSION 3.10)
project(geometry VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
	geometry.cpp
	polygon.cpp
	vertex.cpp
	edge.cpp
)

set(HEADERS 
	geometry.h
	polygon.h
	vertex.h
	edge.h
)

add_library(
	${PROJECT_NAME} 
	STATIC 
	${SOURCES}
	${HEADERS}
        contact.h
        contact.cpp

)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC OFF AUTOUIC OFF)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_include_directories(${PROJECT_NAME} PUBLIC .)

