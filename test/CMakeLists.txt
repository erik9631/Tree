set(tests
    main_tests.cpp
)
add_executable(${PROJECT_NAME}_tests ${tests})

target_link_libraries(${PROJECT_NAME}_tests PUBLIC catch_amalgamated)
target_link_libraries(${PROJECT_NAME}_tests PUBLIC ${PROJECT_NAME})