#include "kmer.h"
// Kmer structure maximum size is 32 bytes
#define KMER_LENGTH 32

// An array with all the possible four standard nucleotides
static int kmer_ncl[4] = 
{
    'A', 'C', 'G', 'T'
};

// Insert into table values ("ACGTA")
static kmer* str_to_kmer(char* str)
{
    kmer * k;
    int i, j, n = strlen(str);
    int size = n; 
    bool found;

    // Verify that the DNA has only values A, C, G, T
    for (i = 0; i < size; i++)
    { // Using variable size as the length of the DNA string
        found = false;

        for (j = 0; j < 4; j++) 
        {
            if (toupper(str[i]) == kmer_ncl[j]) 
            { // Compare the value of the DNA string vs the kmer allowed permits
                found = true;
                break;
            }
        }

        // If one value is not allowed, then show an error
        if (found == false)
        {
            elog(ERROR, "value '%c' is not a valid digit for type kmer ('A','C','G','T').", str[i]);       
        }
    }

    k = (kmer *)palloc(sizeof(kmer)); 
    strncpy(k->ncl, str, 32);  
    k->ncl[32] = '\0';  
    return k;
}

// From structure KMER to Str
static char* kmer_to_str(const kmer* k)
{
    char * result = psprintf("%s", k->ncl);
    return result;
}


// Input from the user 
PG_FUNCTION_INFO_V1(kmer_in);
Datum kmer_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    PG_RETURN_POINTER(str_to_kmer(str));
}

/* Output for the user */
PG_FUNCTION_INFO_V1(kmer_out);
Datum kmer_out(PG_FUNCTION_ARGS)
{
    const kmer* s = PG_GETARG_KMER_P(0);
    PG_RETURN_CSTRING(kmer_to_str(s));
}

PG_FUNCTION_INFO_V1(kmer_cast_from_text);
Datum
kmer_cast_from_text(PG_FUNCTION_ARGS)
{
  text *txt = PG_GETARG_TEXT_P(0);
  char *str = DatumGetCString(DirectFunctionCall1(kmer_out,PointerGetDatum(txt)));
  PG_RETURN_KMER_P(str_to_kmer(str));
}

PG_FUNCTION_INFO_V1(kmer_cast_to_text);
Datum
kmer_cast_to_text(PG_FUNCTION_ARGS)
{
  kmer *k  = PG_GETARG_KMER_P(0);
  text *out = (text *)DirectFunctionCall1(kmer_in,PointerGetDatum(kmer_to_str(k)));
  PG_FREE_IF_COPY(k, 0);
  PG_RETURN_TEXT_P(out);
}

/*****************************************************************************
 * Internal functions
 ******************************************************************************/
int length_kmer_internal(kmer *k) {
    return strlen(k->ncl);
}

static bool kmer_eq_internal(kmer *k1, kmer *k2)
{    
    if (length_kmer_internal(k1) != length_kmer_internal(k2)) return false;
    return (strcmp(k1->ncl, k2->ncl) == 0);
}

static bool starts_with_2internal(kmer *k1, kmer *k2)
{   const char* path1;
    const char* path2;
    if (length_kmer_internal(k1) > length_kmer_internal(k2)) {
        return false;
    }
    path1 = k1->ncl;
    path2 = k2->ncl;
    elog(NOTICE, "Comparison two letter: %s, %s",path1,path2); 
    for (int i = 0; i < length_kmer_internal(k1); i++) {
        elog(NOTICE, "Comparison two letter: %c, %c",path1[i],path2[i]); 
        if (path1[i] != path2[i]) {
            return false;
        }
    }
    return true;
}
static bool
starts_with_internal(char *str, kmer *k)
{   
	char* path;
    if(length_kmer_internal(k) < strlen(str)) {
        return false;
    }
    path = k->ncl;  
    for(int i = 0; i < strlen(str); i++) {
        if(path[i] != str[i]) {
            return false;
        }
    }
    return true;
}
/*****************************************************************************/
static bool find_equal(char c, char f){
	
	switch (f) {
		case 'R':
		if(c == 'A'|| c=='G'){
			return true;
		}
		return false;
			break;
		case 'Y':
		if(c == 'C'|| c=='T'){
			return true;
		}
		return false;
			break;
		case 'S':
		if(c == 'C'|| c=='G'){
			return true;
		}
		return false;
			break;
		case 'W':
		if(c == 'A'|| c=='T'){
			return true;
		}
		return false;
			break;
		case 'K':
		if(c == 'T'|| c=='G'){
			return true;
		}
		return false;
			break;
		case 'M':
		if(c == 'A'|| c=='C'){
			return true;
		}
		return false;
			break;
		case 'B':
		if(c == 'C'|| c=='G'||c=='T'){
			return true;
		}
		return false;
			break;
		case 'D':
		if(c == 'A'|| c=='G'||c=='T'){
			return true;
		}
		return false;
			break;
		case 'H':
		if(c == 'A'|| c=='C'||c=='T'){
			return true;
		}
		return false;
			break;
		case 'V':
		if(c == 'A'|| c=='C'||c=='G'){
			return true;
		}
		return false;
			break;
		case 'N':
		return true;
		break;
		return false;
			break;
		default:
		return false;	
		
	}
	
	return false; 
	
}
static bool
contains_internal(char * str, kmer * k1)
{	
  char* path;
	if(length_kmer_internal(k1)< strlen(str)){
		return false;
	}
	path = k1->ncl;
	for(int i = 0;i<strlen(path); i++){
		
		if (path[i]!=str[i]){
			bool result = find_equal(path[i], toupper(str[i]));
			if(result == false){
				return result; 
			}
		}
		
	}
	return true;
}
/*****************************************************************************/
PG_FUNCTION_INFO_V1(length_kmer);
Datum 
length_kmer(PG_FUNCTION_ARGS)
{
    kmer* k = PG_GETARG_KMER_P(0);
    PG_RETURN_INT32(length_kmer_internal(k));
}

PG_FUNCTION_INFO_V1(equals_kmer);
Datum 
equals_kmer(PG_FUNCTION_ARGS)
{
    kmer* q = PG_GETARG_KMER_P(0);
    kmer* b = PG_GETARG_KMER_P(1);
    bool result = kmer_eq_internal(q, b);
    PG_FREE_IF_COPY(q, 0);
    PG_FREE_IF_COPY(b, 1);
    PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(starts_with_kmer);
Datum 
starts_with_kmer(PG_FUNCTION_ARGS)
{
    kmer* q = PG_GETARG_KMER_P(0);  
    kmer* b = PG_GETARG_KMER_P(1);
    bool result = starts_with_2internal(q, b);
    PG_FREE_IF_COPY(q, 0);
    PG_FREE_IF_COPY(b, 1);
    PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(contains);
Datum 
contains(PG_FUNCTION_ARGS)
{
	kmer* q = PG_GETARG_KMER_P(1);
	char * str = (char *)PG_GETARG_POINTER(0);
	bool result = contains_internal(str, q);
    PG_FREE_IF_COPY(q, 1);
    PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(my_persona_sequece);
Datum
my_persona_sequece(PG_FUNCTION_ARGS)
{

    FuncCallContext *funcctx;
    kmer * k;
    int j = 0;
	int count=0;
    int len, call=0;
    char *str;
	str = PG_GETARG_CSTRING(0);
    len = PG_GETARG_INT32(1);
    if(SRF_IS_FIRSTCALL())
    {
        MemoryContext oldcontext;
        /* In FuncCallContext */
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        
        
        call = strlen(str) - len +1;
        k= (kmer *) palloc(call*sizeof(kmer));
        k->ncl[len] = '\0';
        funcctx->max_calls = call;
        funcctx->call_cntr = j;
        funcctx->user_fctx = k;
        MemoryContextSwitchTo(oldcontext); 
		elog(NOTICE, "init: %d", count);
		count++;
    }
    funcctx = SRF_PERCALL_SETUP();
    call =  funcctx->max_calls;
    j = funcctx->call_cntr;
    k = (kmer *) funcctx->user_fctx;
    if(j<call)
    {
		elog(NOTICE, "call____: %d", j);
		for( int i = 0; i<len;i++){
			elog(NOTICE, "letter: %c", k->ncl[i]);
			k->ncl[i]= str[i+j];
		}
		Datum result;
		j++;
		
		result = KmerGetDatum(k);
        SRF_RETURN_NEXT(funcctx, result);
    }
    else
    {
        SRF_RETURN_DONE(funcctx);

    }
}
PG_FUNCTION_INFO_V1(starts_with_kmer_string);
Datum 
starts_with_kmer_string(PG_FUNCTION_ARGS)
{
    kmer* k = PG_GETARG_KMER_P(1);
    char *str = PG_GETARG_CSTRING(0);  
    bool result = starts_with_internal(str, k);  
    PG_FREE_IF_COPY(k, 1);
    PG_RETURN_BOOL(result);
}