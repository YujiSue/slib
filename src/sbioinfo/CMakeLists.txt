if(MSVC)
  if (BUILD_SHARED_LIBS)
    add_compile_definitions(_BUILD_DLL)
  endif()
  set(CMAKE_CXX_FLAGS "/utf-8")
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "-std=c++11 -O2")
endif()
add_library(sbioinfo ${BUILD_MODE} 
  align.cpp
  annotation.cpp
  bam.cpp
  bed.cpp
  biutil.cpp
  cigar.cpp
  fasta.cpp
  gff.cpp
  ngs.cpp
  seq.cpp
  seqio.cpp
  seqlist.cpp
  seqsearch.cpp
  seqtrie.cpp
  sequtil.cpp
  varfilter.cpp
  variant.cpp
  varlist.cpp
  varutil.cpp
  vcf.cpp
)
install(TARGETS sbioinfo ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sbioinfo DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
install(FILES ${PROJECT_SOURCE_DIR}/include/sbioinfo.h DESTINATION include/slib)
