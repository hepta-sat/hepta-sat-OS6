add_library(hepta_eps INTERFACE)
target_include_directories(hepta_eps INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/HeptaBattery
    ${CMAKE_CURRENT_SOURCE_DIR}/HeptaBattery/PowerControl
)

add_subdirectory(HeptaBattery)
target_link_libraries(hepta_eps INTERFACE hepta_battery)