set(ONION_PREFIX onion04)


ExternalProject_Add(
  ${ONION_PREFIX}
  GIT_REPOSITORY "https://github.com/davidmoreno/onion.git"
  GIT_TAG "master"
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_IN_SOURCE 0
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/onion"
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/${ONION_PREFIX} -DONION_EXAMPLES=false
  TEST_COMMAND ""
)



set(ONION_DIR ${CMAKE_CURRENT_BINARY_DIR}/${ONION_PREFIX})

include_directories("${ONION_DIR}/include")

link_directories("${ONION_DIR}/lib")
