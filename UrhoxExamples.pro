QMAKE_CLEAN += $$DESTDIR/$$TARGET

TEMPLATE = subdirs

CONFIG = ordered

SUBDIRS += \
    Samples/ImguiDemo/ImguiDemo.pro \
    Samples/SparkDemo/SparkDemo.pro
