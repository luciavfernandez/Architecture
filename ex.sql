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
