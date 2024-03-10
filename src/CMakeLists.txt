set(src
        Tree.h
)
source_group("" FILES ${src})

target_sources(${PROJECT_NAME} INTERFACE ${src})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
