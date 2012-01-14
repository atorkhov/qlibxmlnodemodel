project(qlibxmlnodemodel)

set(BUILD_SHARED_LIBS ON)
if (NOT DEFINED ${LIB_INSTALL_DIR})
    set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
endif (NOT DEFINED ${LIB_INSTALL_DIR})
if (NOT DEFINED ${INCLUDE_INSTALL_DIR})
    set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
endif (NOT DEFINED ${INCLUDE_INSTALL_DIR})

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

find_package(LibXml2 REQUIRED)

add_definitions(${QT_DEFINITIONS} ${LIBXML2_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR} ${QT_INCLUDES} ${LIBXML2_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(examples)
