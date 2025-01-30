if(MSVC)
  if (BUILD_SHARED_LIBS)
    add_compile_definitions(_BUILD_DLL)
  endif()
  add_dependencies(sobj)
  link_libraries(sobj)
elseif(UNIX)
  link_libraries(sobj)
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
if (NOT BUILD_ONLY)
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sbioinfo DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
  install(FILES ${PROJECT_SOURCE_DIR}/include/sbioinfo.h DESTINATION include/slib)
endif()
