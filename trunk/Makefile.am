SUBDIRS = src backends frontends
bin_PROGRAMS = picdb_test
picdb_test_SOURCES = main.cc
#picdb_test_LDADD = -Lsrc/.libs -Lbackends/src/.libs -lpicdb
picdb_test_LDADD = -L`pwd`/src/ -lpicdb
