cmake_minimum_required(VERSION 3.5)

project(Pifagor_IDE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES Pifagor_IDE_en_150.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        redactormainwindow.cpp
        redactormainwindow.h
        redactormainwindow.ui
        textform.cpp
        textform.h
        textform.ui
        QSourceHighlite.pri
        Highlighter.h
        Highlighter.cpp
        libPifagorTrans.a
        mythread.h
        mythread.cpp

        Resource.qrc
# Транслятор
#        QTTranslator/ArgScaner.cpp
#        QTTranslator/ArgScaner.h
#        QTTranslator/ArgTranslator.cpp
#        QTTranslator/ArgTranslator.h
#        QTTranslator/Atom.cpp
#        QTTranslator/Atom.h
#        QTTranslator/AtomFishka.cpp
#        QTTranslator/AtomFishka.h
#        QTTranslator/Block.cpp
#        QTTranslator/Block.h
#        QTTranslator/Exeption.cpp
#        QTTranslator/Expression.h
#        QTTranslator/Fishka.cpp
#        QTTranslator/Fishka.h
#        QTTranslator/Function.cpp
#        QTTranslator/Function.h
#        QTTranslator/GeneralObject.cpp
#        QTTranslator/GeneralObject.h
#        QTTranslator/Identifier.cpp
#        QTTranslator/Identifier.h
#        QTTranslator/Interface.cpp
#        QTTranslator/Interface.h
#        QTTranslator/Interpretator.cpp
#        QTTranslator/Interpretator.h
#        QTTranslator/KeyWord.cpp
#        QTTranslator/KeyWord.h
#        QTTranslator/List.cpp
#        QTTranslator/List.h
#        QTTranslator/ListFishka.cpp
#        QTTranslator/ListFishka.h
#        QTTranslator/ObjectFishka.cpp
#        QTTranslator/ObjectFishka.h
#        QTTranslator/Precompiled.cpp
#        QTTranslator/Precompiled.h
#        QTTranslator/Program.cpp
#        QTTranslator/Program.h
#        QTTranslator/Scaner.cpp
#        QTTranslator/Scaner.h
#        QTTranslator/SignalFishka.cpp
#        QTTranslator/SignalFishka.h
#        QTTranslator/Term.cpp
#        QTTranslator/Term.h
#        QTTranslator/Translator.cpp
#        QTTranslator/Translator.h
#        QTTranslator/Types.cpp
#        QTTranslator/Types.h
#        QTTranslator/WrapFishka.cpp
#        QTTranslator/WrapFishka.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Pifagor_IDE
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Pifagor_IDE APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Pifagor_IDE SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Pifagor_IDE
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Pifagor_IDE PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${CMAKE_CURRENT_LIST_DIR}/libPifagorTrans.a)

# target_link_libraries(Pifagor_IDE PRIVATE build-trans-Desktop-Debug/libPifagorTrans.a)

set_target_properties(Pifagor_IDE PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Pifagor_IDE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Pifagor_IDE)
endif()
