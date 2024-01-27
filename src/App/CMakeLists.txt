set(SRCS
        main.cpp
        fractal.cpp
        fractal.h

        shaders.qrc
        Shaders/fractal.fs
        Shaders/fractal.vs
        )

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(demo-app ${SRCS}


        mainwindow.h mainwindow.cpp)

target_link_libraries(demo-app
        PRIVATE
        Qt5::Widgets
        FGL::Base
        )