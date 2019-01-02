cmake_minimum_required(VERSION 3.12)
project(Substrings)

set(Qt5Core_DIR "/usr/local/opt/qt/lib/cmake/Qt5Core/")
find_package(Qt5Core REQUIRED)
set(Qt5Widgets_DIR "/usr/local/opt/qt/lib/cmake/Qt5Widgets/")
find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/opt/qt/include")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(Substrings main.cpp
        gui/MainWindow.cpp gui/MainWindow.h
        core/FileIndexer.cpp core/FileIndexer.h
        core/Indexer.cpp core/Indexer.h
        core/Index.cpp core/Index.h
        core/Searcher.cpp core/Searcher.h
        core/Watcher.cpp core/Watcher.h core/FileSearcher.cpp core/FileSearcher.h)
target_link_libraries(Substrings Qt5::Widgets)