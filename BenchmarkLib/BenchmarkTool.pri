SOURCES += \
   $$PWD/BenchmarkTool.cpp \
   $$PWD/Node.cpp \
   $$PWD/TimeProvider.cpp

HEADERS += \
   $$PWD/BenchmarkTool.h \
   $$PWD/ITimeProvider.h \
   $$PWD/Node.h \
   $$PWD/TimeProvider.h

msvc {
  DEFINES += PLATFORM_WINDOWS
}
