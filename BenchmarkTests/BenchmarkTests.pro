QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

TEMPLATE = app

INCLUDEPATH += $$PWD/../BenchmarkLib
INCLUDEPATH += $$PWD/catch2

include($$PWD/../BenchmarkLib/BenchmarkTool.pri)

SOURCES +=  \
    main.cpp \
    test_BenchmarkTool.cpp \
    test_BenchmarkTracker.cpp \
    test_Node.cpp
