# Convenience types for PostgreSQL

This repository contains a bunch of convenience types for PostgreSQL.

## The `bytes` type

Byte size type, printing in the same way as `pg_size_pretty`, but also
supporting arithmetics. It accepts a number followed by any of the
following suffixes, which will scale it accordingly.

| Name     | Symbol | Value          |
|----------|--------|----------------|
| kibibyte | KiB    | 2<sup>10</sup> |
| mibibyte | MiB    | 2<sup>20</sup> |
| gibibyte | GiB    | 2<sup>30</sup> |
| tebibyte | TiB    | 2<sup>40</sup> |
| pebibyte | PiB    | 2<sup>50</sup> |
| exbibyte | EiB    | 2<sup>60</sup> |

The size is stored internally as a 64-bit unsigned integer, meaning
that the maximum size that can be stored is 16 EiB (exbibyte).
