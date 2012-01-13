SET (qlibxmlnodemodel_SRCS qlibxmlnodemodel.cpp)

add_library(qlibxmlnodemodel ${qlibxmlnodemodel_SRCS})
set_target_properties(qlibxmlnodemodel PROPERTIES VERSION 0.1 SOVERSION 0.1)
set_target_properties(qlibxmlnodemodel PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
target_link_libraries (qlibxmlnodemodel
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${LIBXML2_LIBRARIES}
    )

install(TARGETS qlibxmlnodemodel DESTINATION ${LIB_INSTALL_DIR})
install(FILES qlibxmlnodemodel.h ${PROJECT_BINARY_DIR}/src/qlibxmlnodemodel.h DESTINATION ${INCLUDE_INSTALL_DIR})

