set(ONION_PREFIX onion04)
set(ONION_URL ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/onion-master.zip)

ExternalProject_Add(
  ${ONION_PREFIX}
  URL ${ONION_URL}
  BUILD_IN_SOURCE 0
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/onion"
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/${ONION_PREFIX} -DONION_EXAMPLES=false
  TEST_COMMAND ""
)



set(ONION_DIR ${CMAKE_CURRENT_BINARY_DIR}/${ONION_PREFIX})

include_directories("${ONION_DIR}/include")

link_directories("${ONION_DIR}/lib")
