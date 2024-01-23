-- Copyright 2023 Mats Kindahl
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

create type bytes;

create function bytes_in(cstring) returns bytes as 'MODULE_PATHNAME' language c immutable strict;
create function bytes_out(bytes) returns cstring as 'MODULE_PATHNAME' language c immutable strict;

create type bytes (input = bytes_in, output = bytes_out, like = int8);

create function bytes_pl(bytes, bytes) returns bytes as 'MODULE_PATHNAME' language c immutable strict;
create function bytes_mi(bytes, bytes) returns bytes as 'MODULE_PATHNAME' language c immutable strict;
create function bytes_lt(bytes, bytes) returns bool as 'MODULE_PATHNAME' language c immutable strict;
create function bytes_gt(bytes, bytes) returns bool as 'MODULE_PATHNAME' language c immutable strict;
create function bytes_div(bytes, int8) returns bytes as 'MODULE_PATHNAME' language c immutable strict;
create function bytes_mul(bytes, int8) returns bytes as 'MODULE_PATHNAME' language c immutable strict;
create function bytes_mul(int8, bytes) returns bytes as 'MODULE_PATHNAME' language c immutable strict;

create operator +(leftarg = bytes, rightarg = bytes, function = bytes_pl);
create operator -(leftarg = bytes, rightarg = bytes, function = bytes_mi);
create operator <(leftarg = bytes, rightarg = bytes, function = bytes_lt, commutator = >, negator = >=);
create operator >(leftarg = bytes, rightarg = bytes, function = bytes_gt, commutator = <, negator = <=);
create operator /(leftarg = bytes, rightarg = int8, function = bytes_div);
create operator *(leftarg = bytes, rightarg = int8, function = bytes_mul);
create operator *(leftarg = int8, rightarg = bytes, function = bytes_mul);
