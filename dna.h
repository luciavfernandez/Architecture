#ifndef DNA_H
#define DNA_H
#include <postgres.h>
#include "pg_config.h"
#include "fmgr.h"
#include <string.h>
#include "utils/geo_decls.h"
#include "utils/builtins.h"
#include "utils/varlena.h"
//#include "varatt.h"

typedef struct {
    int32 vl_len_; // Lunghezza totale (obbligatorio per tipi varlena)
    char ncl[FLEXIBLE_ARRAY_MEMBER]; // Stringa variabile
} dna;

#define DatumGetDna(X)  ((dna *) DatumGetPointer(X))
#define DnarGetDatum(X)   PointerGetDatum(X)
#define PG_GETARG_DNA_P(n) DatumGetDna(PG_GETARG_DATUM(n))
#define PG_RETURN_DNA_P(x) return DnarGetDatum(x)

Datum dna_in(PG_FUNCTION_ARGS);
Datum dna_out(PG_FUNCTION_ARGS);

static dna* str_to_dna(char* str);
static char* dna_to_str(const dna* k);

#endif