#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T16:43:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeneticAlgorithm
TEMPLATE = app


SOURCES += main.cpp\
    individual.cpp \
    utility_fun.cpp \
    genetic_algorithm.cpp \
    ga_plot.cpp \
    mainwindow.cpp

HEADERS  += \
    individual.h \
    utility_fun.h \
    genetic_algorithm.h \
    ga_plot.h \
    mainwindow.h

# in order to use the qwt framework, you should add these statements
LIBS+= -lqwt
INCLUDEPATH+=F:\Qt\install\5.1.1\mingw48_32\include\QWT
# in order to use the qwt framework, you should add these statements

#FORMS    += mainwindow.ui





