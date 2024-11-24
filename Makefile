MODULE_big	= architecture
OBJS = dna.o qkmer.o kmer.o 
EXTENSION   = architecture
DATA        = architecture--1.0.sql architecture.control

CFLAGS = -std=c11 -Wall  
LDFLAGS += -v
PG_CONFIG ?= pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)