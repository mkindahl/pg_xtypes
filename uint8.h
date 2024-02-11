#ifndef UINT8_H_
#define UINT8_H_

#include <postgres.h>
#include <fmgr.h>

#include <common/int.h>

#define PG_GETARG_UINT64(n) DatumGetUInt64(PG_GETARG_DATUM(n))

Datum uint8pl(PG_FUNCTION_ARGS);
Datum uint8mi(PG_FUNCTION_ARGS);
Datum uint8mul(PG_FUNCTION_ARGS);
Datum uint8div(PG_FUNCTION_ARGS);
Datum uint8lt(PG_FUNCTION_ARGS);
Datum uint8gt(PG_FUNCTION_ARGS);

#endif /* UINT8_H_ */
