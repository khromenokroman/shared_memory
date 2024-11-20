set(APP_NAME consumer)

execute_process(
        COMMAND bash -c "grep ^ID= /etc/os-release | cut -d'=' -f2"
        OUTPUT_VARIABLE LINUX_DISTRIBUTION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Название ОС: ${LINUX_DISTRIBUTION}")


set(PLUG_SOURCES
        main.cpp
)
add_executable(${APP_NAME} ${PLUG_SOURCES})