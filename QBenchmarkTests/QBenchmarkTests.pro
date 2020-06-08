QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

TEMPLATE = app

INCLUDEPATH += $$PWD/../QBenchmarkLib
INCLUDEPATH += $$PWD/catch2

include($$PWD/../QBenchmarkLib/QBenchmark.pri)

SOURCES +=  \
    main.cpp \
    test_QBenchmarkNode.cpp
