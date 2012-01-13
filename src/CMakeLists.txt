SET (qlibxmlnodemodel_SRCS qlibxmlnodemodel.cpp)

add_library(qlibxmlnodemodel ${qlibxmlnodemodel_SRCS})
set_target_properties(qlibxmlnodemodel PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
target_link_libraries (qlibxmlnodemodel
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${LIBXML2_LIBRARIES}
    )

