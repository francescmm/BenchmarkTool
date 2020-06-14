SOURCES += \
   $$PWD/BenchmarkTool.cpp \
   $$PWD/BenchmarkTracker.cpp \
   $$PWD/Node.cpp \
   $$PWD/TimeProvider.cpp

HEADERS += \
   $$PWD/BenchmarkCallback.h \
   $$PWD/BenchmarkTool.h \
   $$PWD/BenchmarkTracker.h \
   $$PWD/ITimeProvider.h \
   $$PWD/Node.h \
   $$PWD/TimeProvider.h

msvc {
  DEFINES += PLATFORM_WINDOWS
}
