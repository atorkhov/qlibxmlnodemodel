SET (qlibxmlnodemodel_SRCS qlibxmlnodemodel.cpp)

add_library(qlibxmlnodemodel ${SHARED_OR_STATIC} ${qlibxmlnodemodel_SRCS})
target_link_libraries (qlibxmlnodemodel
    ${QT_QTXML_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${LIBXML2_LIBRARIES}
    )

