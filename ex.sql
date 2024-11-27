drop extension architecture cascade;
create extension architecture cascade;
SELECT * FROM generate_kmers('ACGTACGT', 5);


select length('ACCC'), length('ACGTACGT'::qkmer);

select starts_with('ACG' ,'ACGTACGT'::kmer );

SELECT k.kmer FROM generate_kmers'ACGTACGTTTTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTTACGTACGTTTTTTTTTTT', 2) AS k(kmer);

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


SELECT k.kmer,count(*) FROM generate_kmers('ACGTACGTACGTACGTACGTACGT', 5) AS k(kmer) WHERE kmer = 'ACGTA' GROUP BY kmer; 


SELECT * FROM generate_kmers('ACGTACGTACGTACGTACGTACGT', 5) AS k(kmer) WHERE kmerˆ@'ACG';



SELECT * FROM kmers WHERE 'ANGTA' @> kmer;

SELECT * FROM generate_kmers('ACCTACGTACGTACGTACGTACCT',5) AS k (kmer)WHERE kmer ^@ 'ACG';




SELECT k.kmer,count(*) FROM generate_kmers('ACGTACGTACGTACGTACGTACGT', 5) AS k(kmer) WHERE kmer ^@ 'ACG' GROUP BY k.kmer; 



SELECT * FROM generate_kmers('ACGTACGTACGTACGTACGTACGT', 5) AS k(kmer) WHERE 'ACGTA' @> kmer;

SELECT k.kmer, count(*) FROM generate_kmers('ACGTACGTACGTACGTACGTACGT', 3) AS k(kmer) GROUP BY k.kmer ORDER BY count(*) DESC; --WORKS

-- INDEX
SELECT COUNT(*) FROM generate_kmers('ACGTACGTACGTACGTACGTACGT', 5) AS k(kmer) WHERE kmer = 'ACGTA'; --WORKS

SELECT COUNT(*) FROM generate_kmers('ANCGTACGTACGTACGTACGTACGT', 5) AS k(kmer) WHERE kmer = 'ACGTA'; --WORKS

SELECT * FROM generate_kmers('ACGTACGTACGTACGTACGTACGT', 5) AS k(kmer) WHERE 'ACGTA' @> kmer; -- WORKS

SELECT * FROM generate_kmers('ACCTACGTACGTACGTACGTACCT',5) AS k (kmer) WHERE kmer ^@ 'ACG'; -- WORKS


SELECT k.kmer,COUNT(*) FROM generate_kmers('ACCTACGTACGTACGTACGTACCT',5) AS k (kmer) WHERE kmer ^@ 'ACG' GROUP BY 1;










