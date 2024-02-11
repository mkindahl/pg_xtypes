MODULE_big = xtypes
OBJS = bytes.o uint8.o

prog = perl -ne 'print "$$1" if /default_version = '\''(\d+\.\d+)'\''/' xtypes.control
VERSION_xtypes = $(shell $(prog))

EXTENSION = xtypes
DATA = xtypes--$(VERSION_xtypes).sql
PGFILEDESC = "xtypes - convenient extra types for PostgreSQL"
PG_CFLAGS = -std=c99
PG_CPPFLAGS = -Isrc

REGRESS = bytes_type
REGRESS_OPTS += --load-extension=xtypes

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
