set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
list(APPEND ALL_INCLUDE_DIRECTORIES ${OPENGL_INCLUDE_DIR})
list(APPEND ALL_LIBRARIES ${OPENGL_LIBRARIES})

