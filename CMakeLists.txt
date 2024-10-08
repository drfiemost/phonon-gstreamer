cmake_minimum_required(VERSION 3.5)
project(PhononGStreamer)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options("$<$<CONFIG:DEBUG>:-Wall;-Wextra;-pedantic>")
ENDIF()

option(USE_INSTALL_PLUGIN "Use GStreamer codec installation API" TRUE)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(PHONON_GST_MAJOR_VERSION "4")
set(PHONON_GST_MINOR_VERSION "9")
set(PHONON_GST_PATCH_VERSION "1")
set(PHONON_GST_VERSION "${PHONON_GST_MAJOR_VERSION}.${PHONON_GST_MINOR_VERSION}.${PHONON_GST_PATCH_VERSION}")

add_definitions(-DPHONON_GST_VERSION="${PHONON_GST_VERSION}")

find_package(Phonon REQUIRED)
macro_ensure_version("4.7.0" ${PHONON_VERSION} PHONON_FOUND)
macro_log_feature(PHONON_FOUND "Phonon" "Core Library" "git://anongit.kde.org/phonon.git" TRUE "4.7.0")

find_package(OpenGL)
macro_log_feature(OPENGL_FOUND "OpenGL" "OpenGL support is required to compile the gstreamer backend for Phonon" "" FALSE)

macro_log_feature(QT_QTOPENGL_FOUND "Qt4 OpenGL" "Qt4 OpenGL support is required to compile the gstreamer backend for Phonon" "" FALSE)

if (NOT QT_QTOPENGL_FOUND)
    set(OPENGL_FOUND FALSE)
endif ()

add_definitions(${QT_DEFINITIONS})
include_directories(${PHONON_INCLUDES} ${QT_INCLUDES})

find_package(GStreamer REQUIRED)
find_package(GStreamerPlugins REQUIRED)
macro_log_feature(GSTREAMER_FOUND "GStreamer" "gstreamer 1.0 is required for the multimedia backend" "http://gstreamer.freedesktop.org/modules/" TRUE "1.0")
macro_log_feature(GSTREAMER_PLUGIN_VIDEO_FOUND "GStreamer video plugin" "The gstreamer video plugin (part of gstreamer-plugins-base 1.0) is required for the multimedia gstreamer backend" "http://gstreamer.freedesktop.org/modules/" TRUE "1.0")
macro_log_feature(GSTREAMER_PLUGIN_AUDIO_FOUND "GStreamer audio plugin" "The gstreamer audio plugin (part of gstreamer-plugins-base 1.0) is required for the multimedia gstreamer backend" "http://gstreamer.freedesktop.org/modules/" TRUE "1.0")
macro_log_feature(GSTREAMER_PLUGIN_PBUTILS_FOUND "GStreamer pbutils plugin" "The gstreamer pbutils plugin (part of gstreamer-plugins-base 1.0) is required for the multimedia gstreamer backend" "http://gstreamer.freedesktop.org/modules/" TRUE "1.0")

find_package(GLIB2 REQUIRED)
macro_log_feature(GLIB2_FOUND "GLib2" "GLib 2 is required to compile the gstreamer backend for Phonon" "http://www.gtk.org/download/" TRUE)

find_package(GObject REQUIRED)

find_package(LibXml2 REQUIRED)
macro_log_feature(LIBXML2_FOUND "LibXml2" "LibXml2 is required to compile the gstreamer backend for Phonon" "http://xmlsoft.org/downloads.html" TRUE)

add_subdirectory(gstreamer)

macro_display_feature_log()
