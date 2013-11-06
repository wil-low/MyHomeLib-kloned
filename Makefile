KLONE_VERSION ?= 3.1.0

# webapp content is in webapp/ the current dir
WEBAPP_DIR = $(CURDIR)/webapp
WEBAPP_LDFLAGS = -lsqlite3
KLONE_CONF_ARGS += --enable_hooks

include klapp.mk

klapp.mk: ; wget -O $@ -c http://koanlogic.com/klone/klapp.mk

