/*-------------------------------------------------------------------------
 *
 * uint8.c
 *	  Internal unsigned 64-bit integer operations
 *
 * More or less copied from src/backend/utils/adt/int8.c but
 * implemented for unsigned 64-bit integers.
 */

#include "uint8.h"

#include <postgres.h>

Datum uint8lt(PG_FUNCTION_ARGS) {
  uint64 val1 = PG_GETARG_UINT64(0);
  uint64 val2 = PG_GETARG_UINT64(1);

  PG_RETURN_BOOL(val1 < val2);
}

Datum uint8gt(PG_FUNCTION_ARGS) {
  uint64 val1 = PG_GETARG_UINT64(0);
  uint64 val2 = PG_GETARG_UINT64(1);

  PG_RETURN_BOOL(val1 > val2);
}

Datum uint8pl(PG_FUNCTION_ARGS) {
  uint64 arg1 = PG_GETARG_UINT64(0);
  uint64 arg2 = PG_GETARG_UINT64(1);
  uint64 result;

  if (unlikely(pg_add_u64_overflow(arg1, arg2, &result)))
    ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("bigint out of range")));
  PG_RETURN_UINT64(result);
}

Datum uint8mi(PG_FUNCTION_ARGS) {
  uint64 arg1 = PG_GETARG_UINT64(0);
  uint64 arg2 = PG_GETARG_UINT64(1);
  uint64 result;

  if (unlikely(pg_sub_u64_overflow(arg1, arg2, &result)))
    ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("bigint out of range")));
  PG_RETURN_UINT64(result);
}

Datum uint8mul(PG_FUNCTION_ARGS) {
  uint64 arg1 = PG_GETARG_UINT64(0);
  uint64 arg2 = PG_GETARG_UINT64(1);
  uint64 result;

  if (unlikely(pg_mul_u64_overflow(arg1, arg2, &result)))
    ereport(ERROR,
            (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
             errmsg("bigint out of range")));
  PG_RETURN_UINT64(result);
}

Datum uint8div(PG_FUNCTION_ARGS) {
  uint64 arg1 = PG_GETARG_UINT64(0);
  uint64 arg2 = PG_GETARG_UINT64(1);
  uint64 result;

  if (arg2 == 0) {
    ereport(ERROR,
            (errcode(ERRCODE_DIVISION_BY_ZERO), errmsg("division by zero")));
    /* ensure compiler realizes we mustn't reach the division (gcc bug) */
    PG_RETURN_NULL();
  }

  /*
   * INT64_MIN / -1 is problematic, since the result can't be represented on
   * a two's-complement machine.  Some machines produce INT64_MIN, some
   * produce zero, some throw an exception.  We can dodge the problem by
   * recognizing that division by -1 is the same as negation.
   */
  if (arg2 == -1) {
    if (unlikely(arg1 == PG_INT64_MIN))
      ereport(ERROR,
              (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
               errmsg("bigint out of range")));
    result = -arg1;
    PG_RETURN_INT64(result);
  }

  /* No overflow is possible */

  result = arg1 / arg2;

  PG_RETURN_INT64(result);
}
