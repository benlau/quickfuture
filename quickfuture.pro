TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += src tests/quickfutureunittests

quickfutureunittests.depends = src
