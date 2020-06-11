QT       -= gui

TARGET = BenchmarkTool
TEMPLATE = lib
CONFIG += static

include(BenchmarkTool.pri)

QMAKE_CXXFLAGS += -std=c++17

debug {
   TARGET = BenchmarkToold
}
