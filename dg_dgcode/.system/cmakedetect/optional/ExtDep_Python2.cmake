# Python2 exists as an external only because some scripts need to test that they
# still support python2. Those scripts usually need numpy or matplotlib as well,
# so for simplicity we say Python2 is present if and only if there is a python2
# command and if it can import those modules.#

EXECUTE_PROCESS(COMMAND "python2" "-c" "import numpy,matplotlib;import sys;print('.'.join(str(e) for e in sys.version_info[0:3]))"
                OUTPUT_VARIABLE tmp RESULT_VARIABLE tmp_ec ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

if ("x${tmp_ec}" STREQUAL "x0")
  set(HAS_Python2 1)
  string(STRIP "${tmp}" ExtDep_Python_VERSION)
  set(ExtDep_Python2_COMPILE_FLAGS "")
  set(ExtDep_Python2_LINK_FLAGS "")
  set(ExtDep_Python2_VERSION "-")
else()
  set(HAS_Python2 0)
endif()
