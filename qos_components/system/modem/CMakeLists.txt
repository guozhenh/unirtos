# Define a global property to save differential configuration

define_property(GLOBAL PROPERTY project_config_modem
    BRIEF_DOCS "project_config_modem"
    FULL_DOCS "project_config_modem"
)

if(NOT DEFINED SOURCE_UNIRTOS_PLATFORM_OPT_DIR AND NOT DEFINED CONFIG_QOSA_LINUX_PLATFORM_FUNC)
    message(FATAL_ERROR "Please define either SOURCE_UNIRTOS_PLATFORM_OPT_DIR or CONFIG_QOSA_LINUX_PLATFORM_CFG")
endif()

# out_inc_dir
# BINARY_TOP_DIR
# CMAKECONFIG_PY
# PLATFORM_MODEM_DIR
if(CONFIG_QOSA_ASR1903_PLATFORM_FUNC)
    message("modem CONFIG_QOSA_ASR1903_PLATFORM_FUNC=========")
    set(out_inc_dir ${UNIRTOS_PREBUILD_DIR})
    set(BINARY_TOP_DIR ${UNIRTOS_PREBUILD_DIR})
    set(CMAKECONFIG_PY ${UNIRTOS_TOOLS_DIR}/python/cmakeconfig.py)
    set(PLATFORM_MODEM_DIR ${SOURCE_UNIRTOS_PLATFORM_OPT_DIR}/modem CACHE PATH "Path to the modem directory under platform")
elseif(CONFIG_QOSA_ASR1805_PLATFORM_FUNC)
    message("modem CONFIG_QOSA_ASR1805_PLATFORM_FUNC=========")
    set(out_inc_dir ${UNIRTOS_PREBUILD_DIR})
    set(BINARY_TOP_DIR ${UNIRTOS_PREBUILD_DIR})
    set(CMAKECONFIG_PY ${UNIRTOS_TOOLS_DIR}/python/cmakeconfig.py)
    set(PLATFORM_MODEM_DIR ${SOURCE_UNIRTOS_PLATFORM_OPT_DIR}/modem CACHE PATH "Path to the modem directory under platform")
elseif(CONFIG_QOSA_MTK_6813_PLATFORM_FUNC)
    message("modem CONFIG_QOSA_MTK_6813_PLATFORM_FUNC=========")
    if(DEFINED UNIRTOS_PROJECT)
        message(STATUS "UNIRTOS_PROJECT: ${UNIRTOS_PROJECT}")
    else()
        message(FATAL_ERROR "NOT DEFINED UNIRTOS_PROJECT VARIABLES")
    endif()
    set(CMAKECONFIG_PY ${tools_dir}/cmakeconfig.py)
    set(PLATFORM_MODEM_DIR ${SOURCE_UNIRTOS_PLATFORM_OPT_DIR}/modem CACHE PATH "Path to the modem directory under platform")
elseif(SOURCE_UNIRTOS_PLATFORM_OPT_DIR)
    message("modem SOURCE_UNIRTOS_PLATFORM_OPT_DIR=========")
    set(CMAKECONFIG_PY ${tools_dir}/cmakeconfig.py)
    set(PLATFORM_MODEM_DIR ${SOURCE_UNIRTOS_PLATFORM_OPT_DIR}/modem CACHE PATH "Path to the modem directory under platform")
elseif(CONFIG_QOSA_LINUX_PLATFORM_FUNC)
    message("modem CONFIG_QOSA_LINUX_PLATFORM_FUNC=========")
    set(CMAKECONFIG_PY ${tools_dir}/python/cmakeconfig.py)
    set(PLATFORM_MODEM_DIR ${SOURCE_TOP_DIR}/linux_adapter/plat_source/modem CACHE PATH "Path to the modem directory under platform")
else()
    message(FATAL_ERROR "Please define either SOURCE_UNIRTOS_PLATFORM_OPT_DIR or CONFIG_QOSA_LINUX_PLATFORM_FUNC")
endif()

message(STATUS "### out_inc_dir: ${out_inc_dir}")
message(STATUS "### BINARY_TOP_DIR: ${BINARY_TOP_DIR}")
message(STATUS "### CMAKECONFIG_PY: ${CMAKECONFIG_PY}")
message(STATUS "### PLATFORM_MODEM_DIR: ${PLATFORM_MODEM_DIR}")
message(STATUS "### UNIRTOS_PROJECT: ${UNIRTOS_PROJECT}")

# generate band def.h file (not supported for some platforms, so disable it for now)
# set(banddef_h ${out_inc_dir}/qosa_nw_band_def.h)
# file(REMOVE ${banddef_h})
# execute_process(
#      COMMAND python3 ${SOURCE_UNIRTOS_PLATFORM_OPT_DIR}/scripts/bandgen.py --json ${PLATFORM_MODEM_DIR}/port/config/${UNIRTOS_PROJECT}/band_cfg.json --banddef ${banddef_h}
# )

# step1 Merge platform layer .conf file ${PLATFORM_MODEM_DIR}/port/config/.config.modem
file(COPY ${PLATFORM_MODEM_DIR}/port/config/.config.modem DESTINATION ${BINARY_TOP_DIR})
set(MODEM_CONFIG ${BINARY_TOP_DIR}/.config.modem)

# step2 Merge project layer .config.modem file
# Use property to handle
file(APPEND ${MODEM_CONFIG} "\n")
get_property(project_config_modem GLOBAL PROPERTY project_config_modem)
message(STATUS "\rproject_config_modem: ${project_config_modem}\r\n\r\n")

foreach(config_item ${project_config_modem})
    file(APPEND ${MODEM_CONFIG} ${config_item}\n)
endforeach()

set(TARGET_CMAKE ${BINARY_TOP_DIR}/.config.modem.cmake)
execute_process(
    COMMAND python3 ${CMAKECONFIG_PY} --kconfig "Kconfig.modem" ${MODEM_CONFIG} ${TARGET_CMAKE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

include(${TARGET_CMAKE})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/qosa_modem_at_config.h.in ${out_inc_dir}/qosa_modem_at_config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/qosa_datacall_config.h.in ${out_inc_dir}/qosa_datacall_config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/qosa_network_config.h.in ${out_inc_dir}/qosa_network_config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/qosa_usbnet_config.h.in ${out_inc_dir}/qosa_usbnet_config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/qosa_sim_config.h.in ${out_inc_dir}/qosa_sim_config.h)
