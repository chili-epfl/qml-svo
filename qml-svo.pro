TEMPLATE = lib
TARGET = svoplugin

CONFIG += qt plugin c++11 nostrip
CONFIG -= android_install

QT += qml quick

QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS += -O3
QMAKE_CXXFLAGS_RELEASE += -O3

TARGET = $$qtLibraryTarget($$TARGET)
uri = SVO

HEADERS += \
    src/SVO.h \
    src/SVOPlugin.h \
    src/SVOThread.h

SOURCES += \
    src/SVO.cpp \
    src/SVOPlugin.cpp \
    src/SVOThread.cpp

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
LIBS += -lfast -lSophus -lvikit_common -lsvo

android {

    #Enable automatic NEON vectorization
    QMAKE_CXXFLAGS -= -mfpu=vfp
    QMAKE_CXXFLAGS_RELEASE -= -mfpu=vfp
    QMAKE_CXXFLAGS += -mfpu=neon -ftree-vectorize -ftree-vectorizer-verbose=1 -mfloat-abi=softfp
    QMAKE_CXXFLAGS_RELEASE += -mfpu=neon -ftree-vectorize -ftree-vectorizer-verbose=1 -mfloat-abi=softfp

    INCLUDEPATH += $(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/include
    INCLUDEPATH += $(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/jni/include
    INCLUDEPATH += $(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/include/eigen3

    LIBS += -L$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/lib
    LIBS += -L$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/

    LIBS += -lboost_system-gcc-mt-1_53 -lboost_thread-gcc-mt-1_53
}

OTHER_FILES += qmldir

#Install plugin library, qmldir
qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir #types
}

