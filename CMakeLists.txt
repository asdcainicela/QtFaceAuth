cmake_minimum_required(VERSION 3.16)

project(QtFaceAuth VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Widgets Sql Multimedia)

qt_standard_project_setup()

# Resources
qt_add_executable(appQtFaceAuth
    src/main.cpp
    src/database/DatabaseManager.cpp
    src/database/DatabaseManager.h
    src/auth/UserManager.cpp
    src/auth/UserManager.h
)

qt_add_qml_module(appQtFaceAuth
    URI QtFaceAuth
    VERSION 1.0
    QML_FILES 
        ui/main.qml
        ui/views/LoginView.qml
        ui/views/DashboardView.qml
)

target_link_libraries(appQtFaceAuth
    PRIVATE Qt6::Quick Qt6::Widgets Qt6::Sql Qt6::Multimedia
)

# Set Windows subsystem to windows so no console window pops up (in release)
set_target_properties(appQtFaceAuth PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

install(TARGETS appQtFaceAuth
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
