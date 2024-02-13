-- Basic conversions should just work
select '1 bytes'::bytes, '1 B'::bytes;
select '1 KiB'::bytes, '1 KB'::bytes;
select '1.2 KiB'::bytes, '1.2 KB'::bytes;
select '2345 MiB'::bytes, '2345 MB'::bytes;
select '1 GiB'::bytes, '1 GB'::bytes;
select '1 TiB'::bytes, '1 TB'::bytes;
select '1 EiB'::bytes, '1 EB'::bytes;

-- Boundary checks and syntax checks. 
\set VERBOSITY terse
\set ON_ERROR_STOP 0
SELECT '543'::bytes;
SELECT '1 AiB'::bytes;
select '1234567890 TiB'::bytes;
select '16383 PiB'::bytes;
select '16384 PiB'::bytes;
select '16 EiB'::bytes;
select '17 EiB'::bytes;
\set ON_ERROR_STOP 1

create table sample(
       size1n numeric,
       size1b bytes,
       size2n numeric,
       size2b bytes
);

select setseed(1.0);

insert into sample(size1n, size1b, size2n, size2b)
select 1024.0 * 1024.0 * one,
       format('%s MiB', one)::bytes,
       1024.0 * two,
       format('%s KB', two)::bytes
from (select random() * 512 as one,
             random() * 512 * 1024 as two
        from generate_series(1,1000)) vals;
     
\x on
select round(size1b), pg_size_pretty(size1n),
       round(size2b), pg_size_pretty(size2n),
       round(size1b + size2b), pg_size_pretty(size1n + size2n),
       round(size1b - size2b), pg_size_pretty(size1n - size2n),
       round(size1b / 2), pg_size_pretty(size1n / 2),
       round(size1b * 2), pg_size_pretty(size1n * 2),
       round(2 * size1b), pg_size_pretty(2 * size1n)
  from sample
 where size1b > size2b
 limit 5;
\x off
