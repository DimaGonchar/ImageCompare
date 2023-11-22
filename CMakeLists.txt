cmake_minimum_required(VERSION 3.16)

project(ImageCompareApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(OpenCV_DIR ../opencv/build)
#find_package(OpenCV REQUIRED)

find_package(Qt6 6.5 COMPONENTS Gui Qml Quick Core Core5Compat Multimedia  Sql REQUIRED)

set(RESOURCES main.cpp)
qt6_add_resources(RESOURCES Resources.qrc)

qt_add_executable(ImageCompare
    main.cpp
    src/ScreenCaptureController.h
    src/ScreenCaptureController.cpp
    src/ScreenshotsModel.h
    src/ScreenshotsModel.cpp
    src/ScreenshotDetails.h
    ${RESOURCES}
)

qt_add_qml_module(ImageCompare
    URI ImageCompare
    VERSION 1.0
    QML_FILES main.qml
)

set_property(TARGET ImageCompare PROPERTY CXX_STANDARD 23)

target_link_libraries(ImageCompare
    PRIVATE Qt6::Quick Qt6::Core5Compat
            Qt6::Sql
            #${OpenCV_LIBS}
    )

install(TARGETS ImageCompare
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(CMAKE_AUTORCC ON)
