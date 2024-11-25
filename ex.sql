drop extension architecture cascade;
create extension architecture cascade;
SELECT * FROM generate_kmer('ACGTACGT', 5);


select length('ACCC'), length('ACGTACGT'::qkmer);

select starts_with('ACG' ,'ACGTACGT'::kmer );

SELECT k.kmer FROM generate_kmer('ACGTACGTTTTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTT', 2) AS k(kmer);

select 'ACG'::kmer ^@ 'ACGTACGT' as contain1, 'ACG'::kmer ^@ 'ACGTACGT'::kmer as contain2;

drop table arch cascade ;
create table arch (d dna);
INSERT INTO arch(d) VALUES (dna_in('GAGGGGGGGGGGGGGGGGGGGGGGGGGGG'));
insert into arch(d)
values ( dna_in('GAGGGGGG'));
insert into arch(d)
values ( dna_in('GAGGGGGG'));
insert into arch(d)
values ( dna_in('AG'));
select d from arch;

select dna_in('GAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'),dna_out(dna_in('GAAAAAA'));

SELECT version();

SELECT k.kmer, count(*) FROM generate_kmers('ACGTACGT', 5) AS k(kmer) GROUP BY k.kmer;

SELECT k.kmer FROM generate_kmers('ACGTACGT', 5) AS k(kmer);

WITH kmers AS (
    SELECT k.kmer, count(*) FROM generate_kmers('ATCGATCAC', 3) AS k(kmer) GROUP BY k.kmer)
SELECT k.kmer,sum(count) AS total_count, count(*) AS distinct_count, count(*) FILTER (WHERE count = 1) AS unique_count FROM kmers GROUP BY 1;



SELECT k.kmer, count(*), FROM generate_kmers('ATCGATCAC', 3) AS k(kmer) GROUP BY k.kmer ORDER BY count(*) DESC;