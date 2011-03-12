# /**
# * Copyright (c) 2010-2010 Andrey AndryBlack Kunitsyn
# * email:support.andryblack@gmail.com
# *
# * Report bugs and download new versions at http://code.google.com/p/fontbuilder
# *
# * This software is distributed under the MIT License.
# *
# * Permission is hereby granted, free of charge, to any person
# * obtaining a copy of this software and associated documentation
# * files (the "Software"), to deal in the Software without
# * restriction, including without limitation the rights to use,
# * copy, modify, merge, publish, distribute, sublicense, and/or sell
# * copies of the Software, and to permit persons to whom the
# * Software is furnished to do so, subject to the following
# * conditions:
# *
# * The above copyright notice and this permission notice shall be
# * included in all copies or substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# * OTHER DEALINGS IN THE SOFTWARE.
# */
# -------------------------------------------------
# Project created by QtCreator 2010-03-15T19:36:33
# -------------------------------------------------
TARGET = FontBuilder
TEMPLATE = app
SOURCES += main.cpp \
    fontbuilder.cpp \
    colorbutton.cpp \
    fontselectframe.cpp \
    fontoptionsframe.cpp \
    fontrenderer.cpp \
    charactersframe.cpp \
    fontconfig.cpp \
    abstractlayouter.cpp \
    layoutconfig.cpp \
    layoutdata.cpp \
    layouters/linelayouter.cpp \
    layouterfactory.cpp \
    layouters/boxlayouter.cpp \
    layoutconfigframe.cpp \
    outputconfig.cpp \
    outputframe.cpp \
    abstractexporter.cpp \
    exporters/ghlexporter.cpp \
    exporterfactory.cpp \
    abstractimagewriter.cpp \
    imagewriterfactory.cpp \
    image/builtinimagewriter.cpp \
    exporters/zfiexporter.cpp \
    image/targawriter.cpp \
    fonttestframe.cpp \
    fonttestwidget.cpp \
    exporters/divoexporter.cpp \
    charmapdialog.cpp \
    charsselectwidget.cpp \
    exporters/nglexporter.cpp \
    exporters/luaexporter.cpp
HEADERS += fontbuilder.h \
    colorbutton.h \
    fontselectframe.h \
    fontoptionsframe.h \
    fontrenderer.h \
    charactersframe.h \
    fontconfig.h \
    rendererdata.h \
    abstractlayouter.h \
    layoutconfig.h \
    layoutdata.h \
    layouters/linelayouter.h \
    layouterfactory.h \
    layoutchar.h \
    layouters/boxlayouter.h \
    layoutconfigframe.h \
    outputconfig.h \
    outputframe.h \
    abstractexporter.h \
    exporters/ghlexporter.h \
    exporterfactory.h \
    abstractimagewriter.h \
    imagewriterfactory.h \
    image/builtinimagewriter.h \
    exporters/zfiexporter.h \
    image/targawriter.h \
    fonttestframe.h \
    fonttestwidget.h \
    exporters/divoexporter.h \
    charmapdialog.h \
    charsselectwidget.h \
    exporters/nglexporter.h \
    exporters/luaexporter.h
FORMS += fontbuilder.ui \
    fontselectframe.ui \
    fontoptionsframe.ui \
    charactersframe.ui \
    layoutconfigframe.ui \
    outputframe.ui \
    fonttestframe.ui \
    charmapdialog.ui
TRANSLATIONS = fontbuilder_en.ts \
    fontbuilder_ru.ts
QT += xml

DESTDIR = bin
OBJECTS_DIR = .obj
MOC_DIR = .obj
UI_DIR = .obj
TARGET = FontBuilder

INCLUDEPATH+=./
mac { 
    INCLUDEPATH += ../include
    INCLUDEPATH += ../include/freetype2
    LIBS += -L../lib -lfreetype -lz
}
win32 { 
    INCLUDEPATH += ../include
    INCLUDEPATH += ../include/freetype2
    LIBS += -L../lib \
        -lfreetype
}
linux*|freebsd* { 
    CONFIG += link_pkgconfig
    PKGCONFIG += freetype2
}
OTHER_FILES += fontbuilder_ru.ts \
    fontbuilder_en.ts
