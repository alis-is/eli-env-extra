cmake_minimum_required (VERSION 2.6)
project (eli_env_extra)

file(GLOB eli_env_extra_sources ./src/**.c)
SET(eli_env_extra ${eli_env_extra_sources})

ADD_LIBRARY (eli_env_extra ${eli_env_extra})
TARGET_LINK_LIBRARIES (eli_env_extra)