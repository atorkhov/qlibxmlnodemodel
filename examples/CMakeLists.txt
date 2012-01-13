SET (htmlquery_SRCS htmlquery.cpp)

add_executable(htmlquery ${htmlquery_SRCS})
target_link_libraries (htmlquery
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${LIBXML2_LIBRARIES}
    qlibxmlnodemodel
    )

