include($$PWD/docs/docs.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
        $$PWD/src/sql.pro \
        $$PWD/examples

##qpm
OTHER_FILES += \
        $$PWD/com_github_josephmillsatwork_qmlsql.pri \
        $$PWD/qpm.json
