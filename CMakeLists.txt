cmake_minimum_required(VERSION 3.10)
project(svgscene
        VERSION 0.0.2
        DESCRIPTION "This tool parses an SVG file into Qt graphics objects and provides tools to \
                    lookup elements based on xml attributes in the SVG file."
        )

find_package(Qt5 5.9
             REQUIRED COMPONENTS Core Widgets Gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(svgscene STATIC)
target_sources(svgscene
               PRIVATE
               src/svgscene/components/groupitem.cpp
               src/svgscene/components/groupitem.h
               src/svgscene/components/hyperlinkitem.cpp
               src/svgscene/components/hyperlinkitem.h
               src/svgscene/components/simpletextitem.cpp
               src/svgscene/components/simpletextitem.h
               src/svgscene/graphicsview/svggraphicsview.cpp
               src/svgscene/graphicsview/svggraphicsview.h
               src/svgscene/svgdocument.cpp
               src/svgscene/svgdocument.h
               src/svgscene/svggraphicsscene.cpp
               src/svgscene/svggraphicsscene.h
               src/svgscene/svghandler.cpp
               src/svgscene/svghandler.h
               src/svgscene/svgmetadata.cpp
               src/svgscene/svgmetadata.h
               src/svgscene/svgspec.h
               src/svgscene/utils/logging.h
               src/svgscene/utils/memory_ownership.h
               )

target_include_directories(svgscene
                           PUBLIC src
                           PRIVATE src/svgscene)

target_compile_features(svgscene PUBLIC cxx_std_11)

target_link_libraries(svgscene
                      PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

# To use Necrolog instead of Qt, add the following snippet to your CMakeLists.txt.
#   # Necrolog build
#
#	add_library(necrolog STATIC)
#	target_sources(necrolog
#				   PRIVATE
#				   necrolog/libnecrolog/necrolog.cpp
#				   necrolog/libnecrolog/necrolog.h
#				   necrolog/libnecrolog/necrologglobal.h
#				   )
#	target_include_directories(necrolog
#							   PUBLIC necrolog/include)
#
#   # Add Necrolog to svgscene
#
#	target_link_libraries(svgscene
#						  PUBLIC necrolog)
#	target_compile_definitions(svgscene
#							   PRIVATE
#							   SVGSCENE_USE_EXTERNAL_LOGGER=1
#							   SVGSCENE_USE_NECROLOG=1
#							   )
#

add_executable(svgscene-example EXCLUDE_FROM_ALL
               src/example/main.cpp
               src/example/mainwindow.cpp
               src/example/mainwindow.h
               src/example/mainwindow.ui
               )
target_link_libraries(svgscene-example
                      PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets svgscene)