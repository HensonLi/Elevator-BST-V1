DEPENDPATH += $$PWD/..
INCLUDEPATH +=  $$PWD/

HEADERS +=  $$PWD/iodevicebase.h \
            $$PWD/physicalbase.h \
            $$PWD/physicalnet.h \
            $$PWD/physicalrtc.h \
            $$PWD/physicalserial.h \
            $$PWD/physicalparallel.h \
            $$PWD/physicalnfc.h \
            $$PWD/physicaldisk.h \
            $$PWD/physicalmem.h \
            $$PWD/physicalled.h \
            $$PWD/physicallcd.h \
            $$PWD/physicalinput.h \
            $$PWD/physicalcpu.h \
            $$PWD/physicalcamera.h \
            $$PWD/physicalaudio.h \
            $$PWD/physicalbluetooth.h \
            $$PWD/physicalinc.h \
            $$PWD/physicalmovable.h

SOURCES +=  $$PWD/iodevicebase.cpp \
            $$PWD/physicalbase.cpp \
            $$PWD/physicalnet.cpp \
            $$PWD/physicalrtc.cpp \
            $$PWD/physicalserial.cpp \
            $$PWD/physicalparallel.cpp \
            $$PWD/physicalnfc.cpp \
            $$PWD/physicaldisk.cpp \
            $$PWD/physicalmem.cpp \
            $$PWD/physicalled.cpp \
            $$PWD/physicallcd.cpp \
            $$PWD/physicalinput.cpp \
            $$PWD/physicalcpu.cpp \
            $$PWD/physicalcamera.cpp \
            $$PWD/physicalbluetooth.cpp \
            $$PWD/physicalaudio.cpp \
            $$PWD/physicalmovable.cpp

#contains(DEFINES, GCC)
#{
#       contains(DEFINES, WINDOWS)
#        {
#                LIBS += $$PWD/nfc/Windows/lib/libnfc.dll.a \
#                $$PWD/nfc/Windows/lib/libnfcutils.a
#        }
#        contains(DEFINES, UBUNTU)
#        {
#                LIBS += $$PWD/nfc/Ubuntu/lib/libnfc.a \
#               $$PWD/nfc/Ubuntu/lib/libnfc.so.4.0.0
#       }
#        contains(DEFINES, LINUX)
#
#                LIBS += $$PWD/nfc/Linux/lib/libnfc.a \
#                $$PWD/nfc/Linux/lib/libnfc.so.4.0.0
#        }
#}

