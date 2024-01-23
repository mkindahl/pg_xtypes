SELECT '1 KiB'::bytes, '1 kB'::bytes;
SELECT '1.2 KiB'::bytes, '1.2 kB'::bytes;
SELECT '2345 MiB'::bytes;
SELECT '1 GiB'::bytes;
SELECT '1 TiB'::bytes;
SELECT '1 EiB'::bytes;

create table sample(time timestamptz, size1 bytes, size2 bytes);

select setseed(1.0);

insert into sample
select time,
       format('%s MiB', random() * 1024)::bytes,
       format('%s kB', random() * 1024 * 1024)::bytes
from generate_series('2022-01-01 00:00:00'::timestamptz, '2022-01-02 00:00:00'::timestamptz, '1 minute') as time;
     
select time, size1, size2, size1 + size2 from sample limit 5;
select size1, size2, size1 - size2 from sample where size1 > size2 limit 5;
select size1, size1 / 2 from sample limit 5;
select size1, size1 * 2 from sample limit 5;
select size1, 2 * size1 from sample limit 5;
