#include "qkmer.h"

#define QKMER_LENGTH      32

static char qkmer_nucl[15] = 
{
    'A', 'C', 'G', 'T', 'R','Y', 'S', 'W', 'K', 'M', 'B', 'D', 'H', 'V', 'N'
};

static qkmer * 
str_to_qkmer(char* str)
{   
    qkmer *q;
    int size = strlen(str);
    int x;
    if(size == 0 || size > QKMER_LENGTH)
    {
        ereport(ERROR,
                (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                 errmsg("value \"%s\" is out of range for type Qkmer", str)));
    }
    for(int i = 0; i < size; i++)
    {
        x = 0;
        for(int j = 0; j < 15; j++)
        {
            if(toupper(str[i]) == qkmer_nucl[j])
            {
                x = 1;
                break;
            }
        }
        if(!x)
        {
            elog(ERROR, "value '%c' is not a valid digit for type qkmer", str[i]);
        }
    }
    q = (qkmer *)palloc(sizeof(qkmer));
    strncpy(q->ncl, str, QKMER_LENGTH);  
    q->ncl[QKMER_LENGTH] = '\0';  
    return q;
}

static char * 
qkmer_to_str(const qkmer* q)
{
    char * result = psprintf("%s", q->ncl);
    return result;
}

/*****************************************************************************/

PG_FUNCTION_INFO_V1(qkmer_in);
Datum 
qkmer_in(PG_FUNCTION_ARGS)
{
    char * str = PG_GETARG_CSTRING(0);
    PG_RETURN_POINTER(str_to_qkmer(str)); 
}

PG_FUNCTION_INFO_V1(qkmer_out);
Datum 
qkmer_out(PG_FUNCTION_ARGS)
{
    qkmer* q = PG_GETARG_Qkmer_P(0);
    PG_RETURN_CSTRING(qkmer_to_str(q));
}

/*****************************************************************************
 * internal functions
 ******************************************************************************/

int length_qkmer_internal(qkmer *q)
{
    int result = strlen(q->ncl);
    return result;
}

static bool
qkmer_eq_internal(qkmer *q1, qkmer *q2)
{   
    if(length_qkmer_internal(q1) != length_qkmer_internal(q2)) return false;
    return (strcmp(q1->ncl, q2->ncl) == 0);
}



/*****************************************************************************
 * functions
 ******************************************************************************/

PG_FUNCTION_INFO_V1(length_qkmer);
Datum 
length_qkmer(PG_FUNCTION_ARGS)
{
    qkmer* q = PG_GETARG_Qkmer_P(0);
    PG_RETURN_INT32(length_qkmer_internal(q));
}

PG_FUNCTION_INFO_V1(equals_qkmer);
Datum 
equals_qkmer(PG_FUNCTION_ARGS)
{
    qkmer* q = PG_GETARG_Qkmer_P(0);
    qkmer* b = PG_GETARG_Qkmer_P(1);
    bool result = qkmer_eq_internal(q, b);
    PG_FREE_IF_COPY(q, 0);
    PG_FREE_IF_COPY(b, 1);
    PG_RETURN_BOOL(result);
}







