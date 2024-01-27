/*
 * Copyright 2023 Mats Kindahl
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <postgres.h>
#include <fmgr.h>

#include <stdio.h>

#include <utils/fmgrprotos.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(bytes_in);
PG_FUNCTION_INFO_V1(bytes_out);

/* Unit enum with value also being the power-of-two for the
   value. Note that the "UNIT_" prefix is used below. */
enum Unit {
  UNIT_BASE,
  UNIT_KILO,
  UNIT_MEGA,
  UNIT_GIGA,
  UNIT_TERA,
  UNIT_PETA,
  UNIT_EXA,
  MAX_UNIT
};

#define UNIT_SUFFIX(S, U) \
  { .str = #S, .unit = UNIT_##U, .len = sizeof(#S), }

/* Structure containing unit name, length, and enum. */
struct {
  const char* str;
  size_t len;
  enum Unit unit;
} units[] = {
    UNIT_SUFFIX(B, BASE),
    UNIT_SUFFIX(bytes, BASE),
    UNIT_SUFFIX(EiB, EXA),
    UNIT_SUFFIX(GB, GIGA),
    UNIT_SUFFIX(GiB, GIGA),
    UNIT_SUFFIX(KB, KILO),
    UNIT_SUFFIX(KiB, KILO),
    UNIT_SUFFIX(MB, MEGA),
    UNIT_SUFFIX(MiB, MEGA),
    UNIT_SUFFIX(PB, PETA),
    UNIT_SUFFIX(PiB, PETA),
    UNIT_SUFFIX(TB, TERA),
    UNIT_SUFFIX(TiB, TERA),
    UNIT_SUFFIX(kB, KILO),
};

/*
 *  We use the same suffixes as pg_size_pretty() uses for the
 *  printout.
 *
 * This is not entirely correct since most disk vendors use kB to
 * denote 1000 bytes rather than 1024 bytes (and in a similar manner
 * for the other suffixes).
 */
const char* names[] = {
    [UNIT_BASE] = "B",
    [UNIT_KILO] = "kB",
    [UNIT_MEGA] = "MB",
    [UNIT_GIGA] = "GB",
    [UNIT_TERA] = "TB",
    [UNIT_PETA] = "PB",
    [UNIT_EXA] = "EB",
};

Datum bytes_in(PG_FUNCTION_ARGS) {
  char* str = PG_GETARG_CSTRING(0);
  double num;
  char unit[16];

  if (sscanf(str, "%lf %15s", &num, unit) != 2)
    goto error;

  for (int i = 0; i < sizeof(units) / sizeof(*units); ++i)
    if (strncmp(units[i].str, unit, units[i].len) == 0)
      PG_RETURN_UINT64(num * (1ULL << (10 * units[i].unit)));

error:
  ereport(ERROR,
          (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
           errmsg("invalid input syntax for size type: \"%s\"", str)));
}

Datum bytes_out(PG_FUNCTION_ARGS) {
  const uint64 bytes = PG_GETARG_INT64(0);
  enum Unit unit = 0;
  double repr = bytes;

  /* Reduce the precision until reducing it gives fractions on the
   * base type */
  while (repr >= 1024.0) {
    repr /= 1024.0;
    ++unit;
  }
  PG_RETURN_CSTRING(psprintf("%.2lf %s", repr, names[unit]));
}

/*
 * Arithmetic function maker that will generate a function that
 * forwards the call to another type.
 */
#define MAKE_FORWARD_FUNC(NAME, FWD, SFX) \
  PG_FUNCTION_INFO_V1(NAME);              \
  Datum NAME(PG_FUNCTION_ARGS) {          \
    PG_RETURN_##SFX((FWD)(fcinfo));       \
  }                                       \
  extern int no_such_variable

/*
 * Arithmetic functions for bytes type.
 */
MAKE_FORWARD_FUNC(bytes_pl, int8pl, INT64);
MAKE_FORWARD_FUNC(bytes_mi, int8mi, INT64);
MAKE_FORWARD_FUNC(bytes_lt, int8lt, INT64);
MAKE_FORWARD_FUNC(bytes_gt, int8gt, INT64);
MAKE_FORWARD_FUNC(bytes_div, int8div, INT64);
MAKE_FORWARD_FUNC(bytes_mul, int8mul, INT64);
