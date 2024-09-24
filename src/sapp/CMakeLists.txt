if(MSVC)
  if (BUILD_SHARED_LIBS)
    add_compile_definitions(_BUILD_DLL)
  endif()
  set(CMAKE_CXX_FLAGS "/utf-8")
elseif(UNIX)
  link_directories($ENV{LD_LIBRARY_PATH})
  link_libraries(sobj)
  set(CMAKE_CXX_FLAGS "-std=c++11 -O2")
endif()
if (BUILD_SAPP_LIB)
  add_library(sapp ${BUILD_MODE} 
    sapp.cpp
    scuiapp.cpp
  )
  install(TARGETS sapp ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
elseif(BUILD_SNAPP_LIB)
  add_library(snodeapp ${BUILD_MODE} 
    sapp.cpp
    snodeapp.cpp
  )
  install(TARGETS snodeapp ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
  if (NOT BUILD_ONLY)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/node DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
  endif()
endif()
if (NOT BUILD_ONLY)
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sapp DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
endif()
