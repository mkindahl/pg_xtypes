SELECT '1 bytes'::bytes, '1 B'::bytes;
SELECT '1 KiB'::bytes, '1 kB'::bytes;
SELECT '1.2 KiB'::bytes, '1.2 kB'::bytes;
SELECT '2345 MiB'::bytes;
SELECT '1 GiB'::bytes;
SELECT '1 TiB'::bytes;
SELECT '1 EiB'::bytes;

create table sample(time timestamptz, size1b int8, size1 bytes, size2b int8, size2 bytes);

select setseed(1.0);

insert into sample
with vals as (
  select time, random() * 1024 as one, random() * 1024 * 1024 as two
    from generate_series('2022-01-01 00:00:00'::timestamptz,
    	 	         '2022-01-02 00:00:00'::timestamptz,
			 '1 minute') as time)
select time,
       one, format('%s MiB', one)::bytes, two, format('%s kB', two)::bytes
from vals;
     
\x on
select time,
       size1, pg_size_pretty(1024 * 1024 * size1b),
       size2, pg_size_pretty(1024 * size2b),
       size1 + size2 as sum,
       size1 - size2 as difference,
       size1 / 2 as divided,
       size1 * 2 as product1,
       2 * size1 as product2
  from sample
 where size1 > size2
 limit 5;
\x off
