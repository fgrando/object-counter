cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(main main.cpp Control.cpp DetectorFSM.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include_directories("../opencv/build/")

include_directories("../opencv/opencv-master/include/")
include_directories("../opencv/opencv-master/modules/core/include/")
include_directories("../opencv/opencv-master/modules/imgproc/include/")
include_directories("../opencv/opencv-master/modules/highgui/include/")
include_directories("../opencv/opencv-master/modules/imgcodecs/include/")
include_directories("../opencv/opencv-master/modules/videoio/include/")
include_directories("../opencv/opencv-master/modules/video/include/")
include_directories("../opencv/opencv-master/modules/features2d/include/")
include_directories("../opencv/opencv-master/modules/text/include/")

target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/build/lib/libopencv_core.so")
target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/build/lib/libopencv_highgui.so")
target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/build/lib/libopencv_imgcodecs.so")
target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/build/lib/libopencv_imgproc.so")
target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/build/lib/libopencv_features2d.so")
target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/build/lib/libopencv_video.so")
target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/build/lib/libopencv_videoio.so")
target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/../opencv/build/lib/libopencv_text.so")

include_directories("../ini/inih-master/")