set(CGLM_SHARED OFF CACHE BOOL "" FORCE)
set(CGLM_STATIC ON CACHE BOOL "" FORCE)
set(CGLM_USE_C99 ON CACHE BOOL "" FORCE)
set(CGLM_USE_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_DEPENDENCIES_DIR}/cglm-0.9.1" "${PROJECT_DEPENDENCIES_BUILD_DIR}/cglm")