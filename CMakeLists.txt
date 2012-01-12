project(qlibxmlnodemodel)

find_package(Qt4 REQUIRED)

add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES})

add_subdirectory(src)
