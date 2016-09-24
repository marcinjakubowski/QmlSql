TEMPLATE = app
TARGET = query
QT += qml quick sql
CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

target.path = $$[QT_INSTALL_EXAMPLES]/qmlsql/query/
INSTALLS += target
