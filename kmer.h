#ifndef KMER_H
#define KMER_H

#include <postgres.h>
#include "pg_config.h"
#include "fmgr.h"
#include "funcapi.h"

PG_MODULE_MAGIC;

/*
 * This code has only been tested with PostgreSQL 14.3
 */

#if (PG_VERSION_NUM / 100) < 14
#error "Unknown or unsupported PostgreSQL version"
#endif

// Define the kmer structure, holding the nucleotide sequence (max 32 characters)
typedef struct
{
    char ncl[32 + 1]; // +1 for null-terminator
} kmer;

// Macros for working with kmer types in PostgreSQL
#define DatumGetKMERP(X)  ((kmer *) DatumGetPointer(X))
#define KmerGetDatum(X)   PointerGetDatum(X)
#define PG_GETARG_KMER_P(n) DatumGetKMERP(PG_GETARG_DATUM(n))
#define PG_RETURN_KMER_P(x) return KmerGetDatum(x)

// Function declarations for conversion between strings and kmers
Datum kmer_in(PG_FUNCTION_ARGS);
Datum kmer_out(PG_FUNCTION_ARGS);
Datum length_kmer(PG_FUNCTION_ARGS);
Datum kmer_cast_from_text(PG_FUNCTION_ARGS);
Datum kmer_cast_to_text(PG_FUNCTION_ARGS);
Datum my_persona_sequece(PG_FUNCTION_ARGS);
Datum contains(PG_FUNCTION_ARGS);
Datum starts_with_kmer(PG_FUNCTION_ARGS);
Datum kmer_hash(PG_FUNCTION_ARGS);
uint32 djb2_hash(const char *str);

// Internal utility functions for working with kmers
static kmer* str_to_kmer( char*);
static char* kmer_to_str(const kmer*);
int length_kmer_internal(kmer *k);
static bool kmer_eq_internal(kmer *k1, kmer *k2);
static bool starts_with_2internal(kmer *k1, kmer *k2);
static bool contains_internal(char * str, kmer * k1);

#endif /* KMER_H */
