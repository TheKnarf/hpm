file(GLOB SRCS *.c)

find_package("CURL" REQUIRED)
find_package("Jansson" REQUIRED)

add_executable (wpm
    install.c install.h
    list.c list.h
    wpm.c
variables.h process.h process.c filepath.h filepath.c variables.c fileformat.h fileformat.c init.h init.c)

target_include_directories(wpm PUBLIC
    ${CURL_INCLUDE_DIR}
    ${JANSSON_INCLUDE_DIR}
)
target_link_libraries(wpm
    ${CURL_LIBRARY}
    ${JANSSON_LIBRARIES}
)