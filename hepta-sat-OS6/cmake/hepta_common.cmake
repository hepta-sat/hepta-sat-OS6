# cmake/hepta_common.cmake
option(HEPTA_USE_SD "Enable SD/FAT filesystem for LPC1768" OFF)

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET} ${APP_SOURCES})

target_link_libraries(${APP_TARGET}
    mbed-os
    ${APP_LIBS}
)

# --- SD/FAT conditional injection for LPC1768 ---
if(HEPTA_USE_SD AND MBED_TARGET STREQUAL "LPC1768")
  set(FAT_SD_SRCS
    ${MBED_PATH}/storage/filesystem/fat/source/FATFileSystem.cpp
    ${MBED_PATH}/storage/filesystem/fat/ChaN/ff.cpp
    ${MBED_PATH}/storage/filesystem/fat/ChaN/ffunicode.cpp
    ${MBED_PATH}/storage/blockdevice/COMPONENT_SD/source/SDBlockDevice.cpp
  )
  target_sources(${APP_TARGET} PRIVATE ${FAT_SD_SRCS})
  
  file(GLOB MBED_FS_COMMON_SRC
      "${MBED_PATH}/storage/filesystem/source/*.cpp"
  )
  target_sources(${APP_TARGET} PRIVATE ${MBED_FS_COMMON_SRC})

  target_include_directories(${APP_TARGET} PRIVATE
    ${MBED_PATH}/storage/filesystem
    ${MBED_PATH}/storage/filesystem/include
    ${MBED_PATH}/storage/filesystem/include/filesystem
    ${MBED_PATH}/storage/filesystem/fat/include       
    ${MBED_PATH}/storage/filesystem/fat/include/fat
    ${MBED_PATH}/storage/filesystem/fat/ChaN
    ${MBED_PATH}/storage/blockdevice/include
    ${MBED_PATH}/storage/blockdevice/include/blockdevice
    ${MBED_PATH}/storage/blockdevice/COMPONENT_SD/include
    ${MBED_PATH}/storage/blockdevice/COMPONENT_SD/include/SD
  )
    
  set_source_files_properties(${FAT_SD_SRCS} PROPERTIES COMPILE_DEFINITIONS
    MBED_CONF_FAT_CHAN_FF_MIN_SS=512
    MBED_CONF_FAT_CHAN_FF_MAX_SS=4096
    MBED_CONF_FAT_CHAN_FF_USE_LFN=0
    MBED_CONF_FAT_CHAN_FF_FS_EXFAT=0
    MBED_CONF_FAT_CHAN_FF_FS_RPATH=0
    MBED_CONF_FAT_CHAN_FF_VOLUMES=1
    MBED_CONF_FAT_CHAN_FF_LBA64=0
    MBED_CONF_FAT_CHAN_FF_CODE_PAGE=437
    MBED_CONF_FAT_CHAN_FF_MULTI_PARTITION=0
    MBED_CONF_FAT_CHAN_FF_FS_READONLY=0
    MBED_CONF_FAT_CHAN_FFS_DBG=0        
    MBED_CONF_FAT_CHAN_FF_SFN_BUF=12
    MBED_CONF_FAT_CHAN_FF_USE_MKFS=1
  )
endif()

mbed_set_post_build(${APP_TARGET})
