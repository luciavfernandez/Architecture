#ifndef SPGIST_KMER_H
#define SPGIST_KMER_H

#include "postgres.h"
#include "fmgr.h"  
#include "access/spgist.h"
#include "utils/builtins.h"
#include "utils/elog.h"


// Definiciones relacionadas con el tipo kmer
typedef struct {
    char *data; // Datos del k-mer
    int length; // Longitud del k-mer
} Kmer;

// Declaración de funciones del índice SP-GiST
extern Datum spg_config(PG_FUNCTION_ARGS);
extern Datum spg_picksplit(PG_FUNCTION_ARGS);
extern Datum spg_inner_consistent(PG_FUNCTION_ARGS);
extern Datum spg_leaf_consistent(PG_FUNCTION_ARGS);

// Funciones auxiliares
extern int get_kmer_length(const char *kmer);
extern bool kmer_matches_pattern(const char *kmer, const char *pattern);

#endif /* SPGIST_KMER_H */