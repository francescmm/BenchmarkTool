SOURCES += $$PWD/QBenchmark.cpp \
   $$PWD/QBenchmarkNode.cpp \
   $$PWD/QBenchmarkTimeProvider.cpp

HEADERS += $$PWD/QBenchmark.h \
   $$PWD/QBenchmarkNode.h \
   $$PWD/ITimeProvider.h \
   $$PWD/QBenchmarkTimeProvider.h

msvc {
  DEFINES += PLATFORM_WINDOWS
}
