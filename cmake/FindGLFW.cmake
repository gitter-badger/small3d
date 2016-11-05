find_path(GLFW_INCLUDE_DIR NAMES GLFW PATHS include)
find_library(GLFW_LIBRARY NAMES glfw3 PATHS lib)

SET(GLFW_FOUND TRUE)

set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
set(GLFW_LIBRARIES ${GLFW_LIBRARY})

mark_as_advanced(GLFW_LIBRARY GLFW_INCLUDE_DIR)

