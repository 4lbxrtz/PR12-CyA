project(${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cc")
list(REMOVE_ITEM SOURCES "main.cc")
message(STATUS "Found sources: ${SOURCES}")

add_executable("cya_p12")
set_target_properties("cya_p12" PROPERTIES ENABLE_EXPORTS TRUE)

target_sources("cya_p12"
    PRIVATE
      "main.cc"
    PUBLIC
      ${SOURCES}
)

target_include_directories(
    "cya_p12"
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)