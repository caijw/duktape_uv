set(DUKTAPEDIR ${CMAKE_CURRENT_LIST_DIR}/duktape-2.7.0)

include_directories(
  ${DUKTAPEDIR}/src
)

add_library(duktape STATIC ${DUKTAPEDIR}/src/duktape.c)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  target_link_libraries(duktape
    m
  )
endif()
