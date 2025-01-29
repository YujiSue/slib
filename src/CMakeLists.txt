if (BUILD_SOBJ_LIB)
  set(SOBJ_SRC 
    3rd/crypto/md5c.c
    3rd/crypto/sha256c.c
    3rd/crypto/sha512c.c
    3rd/sqlite/sqlite3.c
    3rd/zlib/adler32.c
    3rd/zlib/compress.c
    3rd/zlib/crc32.c 
    3rd/zlib/deflate.c
    3rd/zlib/gzclose.c
    3rd/zlib/gzlib.c
    3rd/zlib/gzread.c
    3rd/zlib/gzwrite.c
    3rd/zlib/infback.c
    3rd/zlib/inffast.c
    3rd/zlib/inflate.c
    3rd/zlib/inftrees.c
    3rd/zlib/trees.c
    3rd/zlib/uncompr.c
    3rd/zlib/zutil.c
    3rd/libtiff/tif_aux.c
    3rd/libtiff/tif_close.c
    3rd/libtiff/tif_codec.c
    3rd/libtiff/tif_color.c
    3rd/libtiff/tif_compress.c
    3rd/libtiff/tif_dir.c
    3rd/libtiff/tif_dirinfo.c
    3rd/libtiff/tif_dirread.c
    3rd/libtiff/tif_dirwrite.c
    3rd/libtiff/tif_dumpmode.c
    3rd/libtiff/tif_error.c
    3rd/libtiff/tif_extension.c
    3rd/libtiff/tif_fax3.c
    3rd/libtiff/tif_fax3sm.c
    3rd/libtiff/tif_flush.c
    3rd/libtiff/tif_getimage.c
    3rd/libtiff/tif_hash_set.c
    3rd/libtiff/tif_jpeg.c
    3rd/libtiff/tif_jpeg_12.c
    3rd/libtiff/tif_luv.c
    3rd/libtiff/tif_lzw.c
    3rd/libtiff/tif_next.c
    3rd/libtiff/tif_ojpeg.c
    3rd/libtiff/tif_open.c
    3rd/libtiff/tif_packbits.c
    3rd/libtiff/tif_pixarlog.c
    3rd/libtiff/tif_predict.c
    3rd/libtiff/tif_print.c
    3rd/libtiff/tif_read.c
    3rd/libtiff/tif_strip.c
    3rd/libtiff/tif_swab.c
    3rd/libtiff/tif_thunder.c
    3rd/libtiff/tif_tile.c
    3rd/libtiff/tif_version.c
    3rd/libtiff/tif_warning.c
    3rd/libtiff/tif_write.c
    3rd/libtiff/tif_zip.c
    3rd/libjpeg/jaricom.c
    3rd/libjpeg/jcapimin.c
    3rd/libjpeg/jcapistd.c
    3rd/libjpeg/jcarith.c
    3rd/libjpeg/jccoefct.c
    3rd/libjpeg/jccolor.c
    3rd/libjpeg/jcdctmgr.c
    3rd/libjpeg/jchuff.c
    3rd/libjpeg/jcinit.c
    3rd/libjpeg/jcmainct.c
    3rd/libjpeg/jcmarker.c
    3rd/libjpeg/jcmaster.c
    3rd/libjpeg/jcomapi.c
    3rd/libjpeg/jcparam.c
    3rd/libjpeg/jcprepct.c
    3rd/libjpeg/jcsample.c
    3rd/libjpeg/jctrans.c
    3rd/libjpeg/jdapimin.c
    3rd/libjpeg/jdapistd.c
    3rd/libjpeg/jdarith.c
    3rd/libjpeg/jdatadst.c
    3rd/libjpeg/jdatasrc.c
    3rd/libjpeg/jdcoefct.c
    3rd/libjpeg/jdcolor.c
    3rd/libjpeg/jddctmgr.c
    3rd/libjpeg/jdhuff.c
    3rd/libjpeg/jdinput.c
    3rd/libjpeg/jdmainct.c
    3rd/libjpeg/jdmarker.c
    3rd/libjpeg/jdmaster.c
    3rd/libjpeg/jdmerge.c
    3rd/libjpeg/jdpostct.c
    3rd/libjpeg/jdsample.c
    3rd/libjpeg/jdtrans.c
    3rd/libjpeg/jerror.c
    3rd/libjpeg/jfdctflt.c
    3rd/libjpeg/jfdctfst.c
    3rd/libjpeg/jfdctint.c
    3rd/libjpeg/jidctflt.c
    3rd/libjpeg/jidctfst.c
    3rd/libjpeg/jidctint.c
    3rd/libjpeg/jmemmgr.c
    3rd/libjpeg/jmemnobs.c
    3rd/libjpeg/jquant1.c
    3rd/libjpeg/jquant2.c
    3rd/libjpeg/jutils.c
    3rd/libpng/png.c
    3rd/libpng/pngerror.c
    3rd/libpng/pngget.c
    3rd/libpng/pngmem.c
    3rd/libpng/pngpread.c
    3rd/libpng/pngread.c
    3rd/libpng/pngrio.c
    3rd/libpng/pngrtran.c
    3rd/libpng/pngrutil.c
    3rd/libpng/pngset.c
    3rd/libpng/pngtrans.c
    3rd/libpng/pngwio.c
    3rd/libpng/pngwrite.c
    3rd/libpng/pngwtran.c
    3rd/libpng/pngwutil.c
  )
  if(MSVC)
    if (BUILD_SHARED_LIBS)
      add_compile_definitions(_BUILD_DLL)
    endif()
    link_directories($ENV{PATH})
    link_libraries(libcurl.lib)
    set(CMAKE_CXX_FLAGS "/utf-8 /EHsc")
    set(SOBJ_SRC ${SOBJ_SRC} 
      3rd/libtiff/tif_win32.c
    )
  elseif(UNIX)
    set(CMAKE_C_FLAGS "-std=c90")
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-parameter")
    link_directories($ENV{LD_LIBRARY_PATH})
    link_libraries(dl pthread curl)
    set(SOBJ_SRC ${SOBJ_SRC} 
      3rd/libtiff/tif_unix.c
    )
  endif()
  set(SOBJ_SRC ${SOBJ_SRC} 
    sbasic/char.cpp
    sbasic/exception.cpp
    sbasic/regex.cpp
    sbasic/string.cpp
    sbasic/time.cpp
    sbasic/u8iter.cpp
    smath/hash.cpp
    smath/numeric.cpp
    smath/random.cpp
    smath/smath.cpp
    sobj/sobject.cpp
    sobj/sobjptr.cpp
    sobj/selement.cpp
    sobj/siter.cpp
    sobj/snumber.cpp
    sobj/sstring.cpp
    sobj/sdate.cpp
    sobj/sdata.cpp
    sobj/sarray.cpp
    sobj/sdictionary.cpp
    sobj/stext.cpp
    sobj/scolumn.cpp
    sobj/srow.cpp
    sobj/stable.cpp
    sobj/stableiter.cpp
    sobj/svoid.cpp
    sio/sio.cpp
    sio/sfile.cpp
    sio/stream.cpp
    sutil/snet.cpp
    sutil/smail.cpp
    sutil/scode.cpp
    sutil/sdb.cpp
    sutil/sjson.cpp
    sutil/sxmlnode.cpp
    sutil/sxmldoc.cpp
    sutil/ssqlite.cpp
    sutil/ssys.cpp
    sutil/sthread.cpp
    sutil/schrono.cpp
    smedia/scanvas.cpp
    smedia/scolor.cpp
    smedia/sfigure.cpp
    smedia/sgraphic.cpp
    smedia/simage.cpp
    smedia/smedio.cpp
    smedia/spixel.cpp
  )
  add_library(sobj ${BUILD_MODE} ${SOBJ_SRC})
  install(TARGETS sobj ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
  if (NOT BUILD_ONLY)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/curl DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/libjpeg DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/libpng DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/libtiff DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sqlite DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/zlib DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sbasic DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sobj DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sio DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/smath DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/sutil DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/smedia DESTINATION include/slib FILES_MATCHING PATTERN "*.h")
    install(FILES ${PROJECT_SOURCE_DIR}/include/sconfig.h DESTINATION include/slib)
    install(FILES ${PROJECT_SOURCE_DIR}/include/sobj.h DESTINATION include/slib)
  endif()
endif()
if (BUILD_BIOINFO_LIB)
  add_subdirectory(sbioinfo)
endif()
if (BUILD_SAPP_LIB OR BUILD_SNAPP_LIB)
  add_subdirectory(sapp)
endif()
