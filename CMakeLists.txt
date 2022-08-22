project (eli_env_extra)

file(GLOB eli_env_extra_sources ./src/**.c)
set(eli_env_extra ${eli_env_extra_sources})

add_library(eli_env_extra ${eli_env_extra})
target_link_libraries(eli_env_extra)