-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION architecure" to load this file. \quit
DROP TYPE IF EXISTS dna CASCADE;
CREATE OR REPLACE FUNCTION kmer_in(cstring)
RETURNS kmer
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION kmer_out(kmer)
RETURNS cstring
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION qkmer_in(cstring)
RETURNS qkmer
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION qkmer_out(qkmer)
RETURNS cstring
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION dna_in(cstring)
RETURNS dna
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION dna_out(dna)
RETURNS cstring
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE kmer (
	INPUT          = kmer_in,
	OUTPUT         = kmer_out,
  INTERNALLENGTH = 32
);
CREATE TYPE qkmer (
	INPUT          = qkmer_in,
	OUTPUT         = qkmer_out,
  INTERNALLENGTH = 32
);


CREATE TYPE dna (
    INTERNALLENGTH = VARIABLE,
    INPUT = dna_in,
    OUTPUT = dna_out
);

CREATE OR REPLACE FUNCTION kmer(text)
  RETURNS kmer
  AS 'MODULE_PATHNAME', 'kmer_cast_from_text'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION text(kmer)
  RETURNS text
  AS 'MODULE_PATHNAME', 'kmer_cast_to_text'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE CAST (text as kmer) WITH FUNCTION kmer(text) AS IMPLICIT;
CREATE CAST (kmer as text) WITH FUNCTION text(kmer);

/******************************************************************************
* Functions 
******************************************************************************/
CREATE FUNCTION length(qkmer) 
RETURNS integer 
AS 'MODULE_PATHNAME', 'length_qkmer' 
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION length(kmer) 
RETURNS integer 
AS 'MODULE_PATHNAME', 'length_kmer' 
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION equals(kmer, kmer) 
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'equals_kmer'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION equals(qkmer, qkmer) 
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'equals_qkmer'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
  
CREATE FUNCTION starts_with(kmer, kmer)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'starts_with_kmer'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION starts_with(cstring, kmer) 
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'starts_with_kmer_string'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

 CREATE FUNCTION contains(cstring, kmer) 
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'contains'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

 CREATE FUNCTION generate_kmers(cstring,INTEGER)
 RETURNS SETOF kmer
  AS 'MODULE_PATHNAME', 'my_persona_sequece'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION kmer_hash(kmer)
RETURNS integer
AS 'MODULE_PATHNAME', 'kmer_hash'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION spg_config() 
RETURNS void 
AS 'MODULE_PATHNAME', 'spg_config'
LANGUAGE C IMMUTABLE;

CREATE FUNCTION spg_picksplit() 
RETURNS void
AS 'MODULE_PATHNAME', 'spg_picksplit'
LANGUAGE C IMMUTABLE ;

CREATE FUNCTION spg_inner_consistent() 
RETURNS void 
AS 'MODULE_PATHNAME', 'spg_inner_consistent'
LANGUAGE C IMMUTABLE;

CREATE FUNCTION spg_leaf_consistent() 
RETURNS void 
AS 'MODULE_PATHNAME', 'spg_leaf_consistent'
LANGUAGE C IMMUTABLE;


  /***************************************************************/

CREATE OPERATOR = (
  LEFTARG = kmer, RIGHTARG = kmer,
  PROCEDURE = equals,
  COMMUTATOR = =
);
CREATE OPERATOR = (
  LEFTARG = qkmer, RIGHTARG = qkmer,
  PROCEDURE = equals,
  COMMUTATOR = =
);
CREATE OPERATOR ^@ (
  LEFTARG = kmer , RIGHTARG = kmer,
  PROCEDURE = starts_with,
  COMMUTATOR = ^@
);


CREATE OPERATOR @> (
  RIGHTARG = kmer,LEFTARG = cstring,
  PROCEDURE = contains,
  COMMUTATOR = @>
);

CREATE OPERATOR CLASS kmer_hash_ops
DEFAULT FOR TYPE kmer USING hash AS
    OPERATOR 1 =,
    FUNCTION 1 kmer_hash(kmer);  

COMMENT ON TYPE kmer IS 'qkmer and kmer';

--- Index

CREATE OPERATOR CLASS kmer_spgist_ops
DEFAULT FOR TYPE kmer USING spgist AS
    OPERATOR 1 = (kmer, kmer),
    FUNCTION 1 spg_config(),
    FUNCTION 2 spg_picksplit(),
    FUNCTION 3 spg_inner_consistent(),
    FUNCTION 4 spg_leaf_consistent();



