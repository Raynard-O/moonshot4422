cmake_minimum_required(VERSION 3.13)
project(NIBS)

set(CMAKE_STANDARD_CXX 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(SOURCE main.cpp ms_func.cpp ms_func.c)

#Set the directories to find cmake config files for libraries
set(OpenCV_DIR "/usr/share/OpenCV/")
set(raspicam_DIR "/usr/local/lib/cmake/")

#Find Packages needed
find_package(OpenCV REQUIRED)
find_package(raspicam REQUIRED)
find_library(WIRINGPI_LIBRARIES NAMES wiringPi REQUIRED)

#Test
if(OpenCV_FOUND AND raspicam_CV_FOUND)

    MESSAGE(STATUS "COMPILING OPENCV TESTS")

endif(OpenCV_FOUND AND raspicam_CV_FOUND)

add_executable(NIBS ${SOURCE})

target_include_directories(NIBS PRIVATE ~/main/include/)

target_link_libraries(NIBS ${raspicam_CV_LIBS} ${WIRINGPI_LIBRARIES} ${OpenCV_LIB}
        ${CMAKE_SOURCE_DIR}/libnfseg.a
        ${CMAKE_SOURCE_DIR}/libimage.a
        ${CMAKE_SOURCE_DIR}/libwsq.a
        ${CMAKE_SOURCE_DIR}/libfet.a
        ${CMAKE_SOURCE_DIR}/liban2k.a
        ${CMAKE_SOURCE_DIR}/libjpegl.a
        ${CMAKE_SOURCE_DIR}/libjpegb.a
        ${CMAKE_SOURCE_DIR}/libihead.a
        #${CMAKE_SOURCE_DIR}/libm.so.6
        ${CMAKE_SOURCE_DIR}/libmlp.a
        ${CMAKE_SOURCE_DIR}/libpca.a
        ${CMAKE_SOURCE_DIR}/libutil.a
        ${CMAKE_SOURCE_DIR}/libioutil.a
        ${CMAKE_SOURCE_DIR}/libnfiq.a
        ${CMAKE_SOURCE_DIR}/libpng.a
        ${CMAKE_SOURCE_DIR}/libf2c.a
        ${CMAKE_SOURCE_DIR}/libmindtct.a
        ${CMAKE_SOURCE_DIR}/libmlp.a
        ${CMAKE_SOURCE_DIR}/libcblas.a
        ${CMAKE_SOURCE_DIR}/libmindtct.a
        ${CMAKE_SOURCE_DIR}/libopenjp2.a
        ${CMAKE_SOURCE_DIR}/libz.a
        ${CMAKE_SOURCE_DIR}/libclapck.a
        ${CMAKE_SOURCE_DIR}/libfft.a


)

