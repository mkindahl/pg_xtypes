-- Basic conversions should just work
select '1 bytes'::bytes, '1 B'::bytes;
select '1 KiB'::bytes, '1 kB'::bytes;
select '1.2 KiB'::bytes, '1.2 kB'::bytes;
select '2345 MiB'::bytes, '2345 MB'::bytes;
select '1 GiB'::bytes, '1 GB'::bytes;
select '1 TiB'::bytes, '1 TB'::bytes;
select '1 EiB'::bytes, '1 EB'::bytes;

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
