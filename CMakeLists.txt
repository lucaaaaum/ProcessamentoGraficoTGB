cmake_minimum_required(VERSION 3.16)
project(ProcessamentoGraficoTGB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.ui src/*.qrc)

add_executable(tgb ${SOURCES})
target_link_libraries(tgb PRIVATE Qt6::Widgets Qt6::OpenGLWidgets ${OpenCV_LIBS})
