cmake_minimum_required(VERSION 3.16)

project(BreweryFinder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(FetchContent)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Gui Location Positioning QuickControls2 Network)

find_package(Qt6 COMPONENTS PlatformGraphics QUIET)
if(Qt6PlatformGraphics_FOUND)
    target_link_libraries(appBreweryFinder PRIVATE Qt6::PlatformGraphics)
endif()

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appBreweryFinder
    main.cpp
)

qt_add_qml_module(appBreweryFinder
    URI BreweryFinder
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES brewerymanager.h brewerymanager.cpp
    QML_FILES FirstPage.qml
    QML_FILES SecondPage.qml
    RESOURCES Fonts.qrc
    RESOURCES Images.qrc
    QML_FILES MapPage.qml
    RESOURCES android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml
)

set_target_properties(appBreweryFinder PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
# Android-specific configuration
if(ANDROID)
    # Include the OpenSSL configuration for Android
    include(${CMAKE_CURRENT_SOURCE_DIR}/android_openssl/android_openssl.cmake)
    # Call the function to add OpenSSL libraries to your target
    add_android_openssl_libraries(appBreweryFinder)
endif()

target_link_libraries(appBreweryFinder
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Location
    PRIVATE Qt6::Positioning
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE Qt6::Network
)

if(ANDROID)
    # This ensures proper SSL setup for Android
    qt_finalize_executable(appBreweryFinder)
endif()

include(GNUInstallDirs)
install(TARGETS appBreweryFinder
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
