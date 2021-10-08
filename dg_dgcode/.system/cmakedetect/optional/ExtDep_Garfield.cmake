#Make sure ROOT and Fortran extdeps are processed first:
#
#NB: Garfield++ now comes with CMake modules, so we could eventually try to
#migrate to a find_package implementation here.
#
if (NOT "x${HAS_ROOT}" STREQUAL "xpending" AND NOT "x${HAS_Fortran}" STREQUAL "xpending")
  set(HAS_Garfield 0)
  set(autoreconf_env_Garfield "GARFIELD_HOME;HEED_DATABASE")
  if (NOT "x$ENV{GARFIELD_HOME}" STREQUAL "x")
    if (NOT HAS_ROOT)
      message("-- Garfield requires ROOT to be present.")
    elseif (NOT HAS_ROOT_GEOM)
      message("-- Garfield requires the geom module for ROOT.")
    elseif (NOT HAS_ROOT_GDML)
      message("-- Garfield requires the gdml module for ROOT.")
    elseif (NOT HAS_Fortran)
      message("-- Garfield requires Fortran to be present.")
    else()
      if (NOT EXISTS $ENV{GARFIELD_HOME}/include/Garfield/GarfieldConstants.hh)
        message("-- GARFIELD_HOME set but header files not found (specifically failed to find $GARFIELD_HOME/include/Garfield/GarfieldConstants.hh).")
      else()
        if (NOT EXISTS $ENV{GARFIELD_HOME}/lib/libGarfield.so AND NOT EXISTS $ENV{GARFIELD_HOME}/lib/libGarfield.dylib)
          message("-- GARFIELD_HOME set but shared library libGarfield.so/libGarfield.dylib not found (looked in $GARFIELD_HOME/lib).")
        else()
          if ("x$ENV{HEED_DATABASE}" STREQUAL "x")
            message("-- GARFIELD_HOME is set but HEED_DATABASE is not.")
            message("--   Usually you would set it with:")
            message("--   export HEED_DATABASE=$GARFIELD_HOME/Heed/heed++/database")
          else()
            set(HAS_Garfield 1)
            #need ROOT compilation flags and both ROOT and Fortran link flags:
            set(ExtDep_Garfield_COMPILE_FLAGS "${ExtDep_ROOT_COMPILE_FLAGS} -I$ENV{GARFIELD_HOME}/include -I$ENV{GARFIELD_HOME}/Heed")
            set(ExtDep_Garfield_LINK_FLAGS "${ExtDep_ROOT_LINK_FLAGS} ${ExtDep_Fortran_LINK_FLAGS}")
            set(ExtDep_Garfield_LINK_FLAGS "${ExtDep_Garfield_LINK_FLAGS} -L$ENV{GARFIELD_HOME}/lib -lGarfield")
            #hackish way to extract Garfield version:
            execute_process(COMMAND "python3" "-c" "import os;import pathlib as p;l=[e for e in (p.Path(os.environ['GARFIELD_HOME'])/'lib/cmake/Garfield/GarfieldConfig.cmake').read_text().splitlines() if 'set(GARFIELD_VERSION' in e];print(l[0].split('\"')[1])" OUTPUT_VARIABLE tmp RESULT_VARIABLE tmp_ec ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
            if ("x${tmp_ec}" STREQUAL "x0")
              string(STRIP "${tmp}" ExtDep_Garfield_VERSION)
              if ("x${ExtDep_Garfield_VERSION}" STREQUAL "x")
                set(ExtDep_Garfield_VERSION "unknown")
              endif()
            else()
              set(ExtDep_Garfield_VERSION "unknown")
            endif()
            if ("x${ExtDep_Garfield_VERSION}" STREQUAL "xunknown")
              message("-- WARNING: Could not determine Garfield version!")
            endif()
          endif()
        endif()
      endif()
    endif()
  endif()
endif()
