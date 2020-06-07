QT       -= gui

TARGET = QBenchmark
TEMPLATE = lib
CONFIG += static

include(QBenchmark.pri)

QMAKE_CXXFLAGS += -std=c++17

debug {
   TARGET = QBenchmarkd
}
