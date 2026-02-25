cmake_minimum_required(VERSION 3.10)

project(InstagramTasker LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)  # Если в будущем добавите .ui файлы, это пригодится

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка префикса по умолчанию в ~/.local (для локальной установки без root)
if(NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "Default install prefix" FORCE)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Sql)

add_executable(InstagramTasker
    main.cpp
    mainwindow.cpp
    mainwindow.h
    preferencesdialog.cpp
    preferencesdialog.h
    ts_translator.cpp
    ts_translator.h
    resources.qrc  # Если у вас есть ресурсы (иконка)
)

target_link_libraries(InstagramTasker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)

# Установка: executable в bin (будет ~/.local/bin/InstagramTasker)
install(TARGETS InstagramTasker RUNTIME DESTINATION bin)

# Опционально: для релиза добавьте оптимизации
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
