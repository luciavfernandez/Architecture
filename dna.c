#include "dna.h"

static int dna_ncl[4] = 
{
    'A', 'C', 'G', 'T'
};

static dna* str_to_dna(char* str)
{
    dna * k;
    int i, j; 
    int n = strlen(str); 
    bool found;

    // Verify that the DNA has only values A, C, G, T
    for (i = 0; i < n; i++)
    { // Using variable size as the length of the DNA string
        found = false;

        for (j = 0; j < 4; j++) 
        {
            if (toupper(str[i]) == dna_ncl[j]) 
            { // Compare the value of the DNA string vs the dna allowed permits
                found = true;
                break;
            }
        }

        // If one value is not allowed, then show an error
        if (found == false)
        {
            elog(ERROR, "value '%c' is not a valid digit for type dna ('A','C','G','T').", str[i]);       
        }
    }

    k = (dna *)palloc(n + VARHDRSZ+1); 
    SET_VARSIZE(k, n + VARHDRSZ+1);
    memcpy(k->ncl, str, n+1);
    elog(NOTICE, "DNA converted: %s", k->ncl);
    return k;
}

static char* dna_to_str(const dna* k)
{
    char * result = psprintf("%s", k->ncl);
    return result;
}
/*****************************************************************************/
PG_FUNCTION_INFO_V1(dna_in);
Datum dna_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    elog(NOTICE, "Input string: %s", str); 
    PG_RETURN_POINTER(str_to_dna(str));
}

/* Output for the user */
PG_FUNCTION_INFO_V1(dna_out);
Datum dna_out(PG_FUNCTION_ARGS)
{
    const dna* s = PG_GETARG_DNA_P(0);
    PG_RETURN_CSTRING(dna_to_str(s));
}