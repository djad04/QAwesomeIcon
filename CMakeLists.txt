cmake_minimum_required(VERSION 3.16)

project(QAwesomeIcon VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt components
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Gui Svg Concurrent Qml Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui Svg Concurrent Qml Quick)

# Create the library
set(LIBRARY_SOURCES
    src/qawesomeicon.h
    src/qawesomeicon.cpp
    src/qawesomeicon_global.h
    src/animation/qawesomeanimationmanager.h
    src/animation/qawesomeanimationmanager.cpp
    src/animation/qawesomeanimationbackend.h
    src/animation/qawesomeanimationbackend.cpp
    src/animation/backends/qawesomegifbackend.h
    src/animation/backends/qawesomegifbackend.cpp
    src/animation/backends/qawesomespritebackend.h
    src/animation/backends/qawesomespritebackend.cpp
    src/animation/backends/qawesomesvgbackend.h
    src/animation/backends/qawesomesvgbackend.cpp
    src/icon/qawesomeiconupdater.h
    src/icon/qawesomeiconupdater.cpp
)

# Build the library
add_library(QAwesomeIconLib SHARED ${LIBRARY_SOURCES})

target_include_directories(QAwesomeIconLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

# Avoid QDataStream dependency in moc-generated code
target_compile_definitions(QAwesomeIconLib PRIVATE QT_NO_DATASTREAM)

target_link_libraries(QAwesomeIconLib PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Quick
)

# Set library properties
set_target_properties(QAwesomeIconLib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME QAwesomeIcon
)

# Define library macros
target_compile_definitions(QAwesomeIconLib PRIVATE QAWESOMEICON_LIBRARY)


# Platform-specific settings
if(WIN32)
    target_compile_definitions(QAwesomeIconLib PRIVATE Q_OS_WIN)
    # Link Windows-specific libraries for taskbar operations
    target_link_libraries(QAwesomeIconLib PRIVATE user32 shell32)
endif()

if(UNIX AND NOT APPLE)
    target_compile_definitions(QAwesomeIconLib PRIVATE Q_OS_LINUX)
endif()

if(APPLE)
    target_compile_definitions(QAwesomeIconLib PRIVATE Q_OS_MACOS)
    set_target_properties(QAwesomeIconExample PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS QAwesomeIconLib
    EXPORT QAwesomeIconTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QAwesomeIcon
    FILES_MATCHING PATTERN "*.h"
)



# Export targets
install(EXPORT QAwesomeIconTargets
    FILE QAwesomeIconTargets.cmake
    NAMESPACE QAwesomeIcon::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QAwesomeIcon
)

# Create config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    QAwesomeIconConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QAwesomeIconConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QAwesomeIcon
)


