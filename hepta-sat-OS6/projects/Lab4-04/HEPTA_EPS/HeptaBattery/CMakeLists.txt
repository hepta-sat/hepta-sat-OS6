add_library(hepta_battery INTERFACE)
target_include_directories(hepta_battery INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/PowerControl
)

add_subdirectory(PowerControl)
target_link_libraries(hepta_battery INTERFACE power_control)