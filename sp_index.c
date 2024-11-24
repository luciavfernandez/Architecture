#include "sp_index.h"

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

    char first_char = in->datum[0]; // Usa el primer carácter como criterio.
    out->resultType = spgMatchNode;
    out->result.matchNode.nodeN = first_char; // Envía el nodo correspondiente.
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

    // Configura etiquetas para los nodos (uno por cada nucleótido)
    for (int i = 0; i < numNodes; i++) {
        out->nodeLabels[i] = CharGetDatum(labels[i]);
    }

    out->mapTuplesToNodes = palloc(in->nTuples * sizeof(int));

    // Asigna cada k-mer a un nodo según el primer carácter
    for (int i = 0; i < in->nTuples; i++) {
        text *kmer = DatumGetTextP(in->datums[i]);
        char firstChar = VARDATA(kmer)[0]; // Obtén el primer carácter del k-mer
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

    out->nNodes = 0; // Número inicial de nodos coincidentes
    out->nodeNumbers = palloc(sizeof(int) * in->nNodes);
    out->recheck = palloc0(sizeof(bool) * in->nNodes);

    char targetChar = DatumGetChar(in->query);

    for (int i = 0; i < in->nNodes; i++) {
        char nodeLabel = DatumGetChar(in->nodeLabels[i]);

        // Coincide con la rama que tiene el carácter buscado
        if (targetChar == nodeLabel || targetChar == 'N') { // 'N' coincide con cualquier carácter
            out->nodeNumbers[out->nNodes++] = i;
            out->recheck[i] = false; // No necesita recheck porque es una coincidencia exacta
        }
    }

    PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(spg_leaf_consistent);
Datum spg_leaf_consistent(PG_FUNCTION_ARGS) {
    spgLeafConsistentIn *in = (spgLeafConsistentIn *) PG_GETARG_POINTER(0);
    spgLeafConsistentOut *out = (spgLeafConsistentOut *) PG_GETARG_POINTER(1);

    text *kmer = DatumGetTextP(in->datum);
    text *query = DatumGetTextP(in->query);
    int kmerLen = VARSIZE(kmer) - VARHDRSZ;
    int queryLen = VARSIZE(query) - VARHDRSZ;

    // Coincidencia exacta
    if (in->strategy == 1) { // Equality search
        out->leafMatches = (kmerLen == queryLen && memcmp(VARDATA(kmer), VARDATA(query), kmerLen) == 0);
    }
    // Coincidencia de prefijo
    else if (in->strategy == 2) { // Prefix search
        int prefixLen = queryLen;
        out->leafMatches = (kmerLen >= prefixLen && memcmp(VARDATA(kmer), VARDATA(query), prefixLen) == 0);
    }
    // Coincidencia por patrones
    else if (in->strategy == 3) { // Pattern matching
        out->leafMatches = true; // Inicializa como verdadero

        for (int i = 0; i < queryLen && out->leafMatches; i++) {
            char queryChar = VARDATA(query)[i];
            char kmerChar = VARDATA(kmer)[i];

            if (queryChar != 'N' && queryChar != kmerChar) { // 'N' coincide con cualquier carácter
                out->leafMatches = false;
            }
        }
    } else {
        ereport(ERROR, (errmsg("Unknown search strategy: %d", in->strategy)));
    }

    PG_RETURN_VOID();
}

