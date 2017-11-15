#-------------------------------------------------
#
# Project created by JOAO VICTOR 2017-10-16T10:01:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServidorRemoto_16_10_2017
TEMPLATE = app

# O seguinte define faz o compilador emitir avisos se você usar
# qualquer característica do Qt que tenha sido marcada como obsoleta (as advertências exatas
# depende do seu compilador). Consulte a documentação do
# API obsoleta, a fim de saber como manter seu código longe dele.
DEFINES += QT_DEPRECATED_WARNINGS

# Você também pode fazer seu código falhar para compilar se você usar APIs obsoletas.
# Para fazer isso, descomente a seguinte linha.
# Você também pode selecionar desativar as APIs obsoletas apenas até uma certa versão do Qt.
#DEFINES + = QT_DISABLE_DEPRECATED_BEFORE = 0x060000 # desabilita todas as APIs obsoletas antes do Qt 6.0.0

RC_ICONS = tomcat.ico

SOURCES += main.cpp\
        server.cpp \
    client.cpp

HEADERS  += server.h \
    client.h

FORMS    += server.ui \

QMAKE_CXXFLAGS += -std=gnu++11
#padrao defino em 2011


