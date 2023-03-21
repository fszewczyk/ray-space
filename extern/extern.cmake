include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/extern)

include(solveGlfw)
include(solveImGui)
include(solveImPlot)
include(solveGlad)
include(solveStbImage)
