add_library(hepta_sensor INTERFACE)
target_include_directories(hepta_sensor INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Hepta9axis
    ${CMAKE_CURRENT_SOURCE_DIR}/HeptaCamera_GPS
    ${CMAKE_CURRENT_SOURCE_DIR}/HeptaTemp
)

add_subdirectory(Hepta9axis)
add_subdirectory(HeptaCamera_GPS)
add_subdirectory(HeptaTemp)

target_link_libraries(hepta_sensor INTERFACE
    hepta9axis
    heptacamera_gps
    heptatemp
)