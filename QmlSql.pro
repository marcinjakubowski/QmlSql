include($$PWD/docs/docs.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
        $$PWD/src/sql.pro \
        $$PWD/examples

