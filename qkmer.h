#ifndef QKMER_H
#define QKMER_H

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <postgres.h>
#include "pg_config.h"
#include "fmgr.h"
#include "libpq/pqformat.h"
#include "utils/fmgrprotos.h"

// Define the length for the qkmer sequence
#define QKMER_LENGTH 32

// Define the qkmer structure, holding the nucleotide sequence (max 32 characters)
typedef struct {
    char ncl[QKMER_LENGTH + 1];  // +1 for null-terminator
} qkmer;

/*****************************************************************************/

// Macros for working with qkmer types in PostgreSQL
#define DatumGetQkmerP(X)  ((qkmer *) DatumGetPointer(X))
#define QkmerPGetDatum(X)   PointerGetDatum(X)
#define PG_GETARG_Qkmer_P(n) DatumGetQkmerP(PG_GETARG_DATUM(n))
#define PG_RETURN_Qkmer_P(x) return QkmerPGetDatum(x)

// Function declarations for conversion between strings and qkmers
Datum qkmer_in(PG_FUNCTION_ARGS);
Datum qkmer_out(PG_FUNCTION_ARGS);
Datum length_qkmer(PG_FUNCTION_ARGS);

// Internal utility functions for working with qkmers
static qkmer* str_to_qkmer( char* str);
static char* qkmer_to_str(const qkmer* q);
int length_qkmer_internal(qkmer *q);

#endif /* QKMER_H */


