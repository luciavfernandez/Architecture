#include "sp_index.h"
#include "postgres.h"
#include "access/spgist.h"
#include "utils/builtins.h"

PG_FUNCTION_INFO_V1(spg_config);
Datum spg_config(PG_FUNCTION_ARGS) {
    spgConfigOut *cfg = (spgConfigOut *) PG_GETARG_POINTER(0);
    cfg->prefixType = TEXTOID; // Tipo de datos que actúa como prefijo.
    cfg->labelType = TEXTOID;  // Tipo de datos que se usa para nodos internos.
    cfg->leafType = TEXTOID;   // Tipo de datos en las hojas (k-mers completos).
    PG_RETURN_VOID();
}


PG_FUNCTION_INFO_V1(spg_choose);
Datum spg_choose(PG_FUNCTION_ARGS) {
    spgChooseIn *in = (spgChooseIn *) PG_GETARG_POINTER(0);
    spgChooseOut *out = (spgChooseOut *) PG_GETARG_POINTER(1);

    text *kmer = DatumGetTextP(in->datum); // Cambia query por datum
    char first_char = VARDATA(kmer)[0];   // Obtén el primer carácter

    out->resultType = spgMatchNode;
    out->result.matchNode.nodeN = first_char; // Envía el nodo correspondiente
    PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(spg_picksplit);
Datum spg_picksplit(PG_FUNCTION_ARGS) {
    spgPickSplitIn *in = (spgPickSplitIn *) PG_GETARG_POINTER(0);
    spgPickSplitOut *out = (spgPickSplitOut *) PG_GETARG_POINTER(1);

    int numNodes = 4; // A, C, G, T
    char labels[] = {'A', 'C', 'G', 'T'}; // Nucleótidos válidos

    out->nNodes = numNodes;
    out->nodeLabels = palloc(numNodes * sizeof(Datum));

    for (int i = 0; i < numNodes; i++) {
        out->nodeLabels[i] = CharGetDatum(labels[i]);
    }

    out->mapTuplesToNodes = palloc(in->nTuples * sizeof(int));

    for (int i = 0; i < in->nTuples; i++) {
        text *kmer = DatumGetTextP(in->datums[i]); // Cambia query por datums
        char firstChar = VARDATA(kmer)[0];
        int nodeIndex = -1;

        for (int j = 0; j < numNodes; j++) {
            if (firstChar == labels[j]) {
                nodeIndex = j;
                break;
            }
        }

        if (nodeIndex == -1) {
            ereport(ERROR, (errmsg("Invalid kmer character: %c", firstChar)));
        }

        out->mapTuplesToNodes[i] = nodeIndex;
    }

    PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(spg_inner_consistent);
Datum spg_inner_consistent(PG_FUNCTION_ARGS) {
    spgInnerConsistentIn *in = (spgInnerConsistentIn *) PG_GETARG_POINTER(0);
    spgInnerConsistentOut *out = (spgInnerConsistentOut *) PG_GETARG_POINTER(1);

    char targetChar = DatumGetChar(in->reconstructedValue); // Usa query en lugar de key

    for (int i = 0; i < in->nNodes; i++) {
        char nodeLabel = DatumGetChar(in->nodeLabels[i]);

        if (targetChar == nodeLabel || targetChar == 'N') { // Coincidencia exacta o comodín 'N'
            out->nodeNumbers[out->nNodes++] = i;
        }
    }

    PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(spg_leaf_consistent);
Datum spg_leaf_consistent(PG_FUNCTION_ARGS) {
    spgLeafConsistentIn *in = (spgLeafConsistentIn *) PG_GETARG_POINTER(0);
    spgLeafConsistentOut *out = (spgLeafConsistentOut *) PG_GETARG_POINTER(1);

    text *kmer = DatumGetTextP(in->leafDatum); // Accede al dato de la hoja
    text *query = DatumGetTextP(in->reconstructedValue);   // Usa query en lugar de key
    int kmerLen = VARSIZE(kmer) - VARHDRSZ;
    int queryLen = VARSIZE(query) - VARHDRSZ;

    // Verifica coincidencia exacta
    out->recheck = (kmerLen == queryLen && memcmp(VARDATA(kmer), VARDATA(query), kmerLen) == 0);
    PG_RETURN_VOID();
}