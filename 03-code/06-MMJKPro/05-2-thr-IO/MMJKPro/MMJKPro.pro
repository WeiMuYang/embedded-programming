#-------------------------------------------------
#
# Project created by QtCreator 2020-11-06T11:24:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MMJKPro
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    io_ctrl.cpp \
    di_thr.cpp \
    bit_operator.cpp \
    udp_socket.cpp \
    process_config.cpp

HEADERS  += mainwindow.h \
    bdaqctrl.h \
    compatibility.h \
    io_ctrl.h \
    di_thr.h \
    bit_operator.h \
    udp_socket.h \
    process_config.h

FORMS    += mainwindow.ui

unix: LIBS += -lbiodaq
