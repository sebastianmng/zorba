/*
 * Copyright 2006-2008 The FLWOR Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#ifndef ZORBA_FUNCTIONS_FUNCTION_CONSTS
#define ZORBA_FUNCTIONS_FUNCTION_CONSTS

#include <zorba/config.h>


namespace zorba
{

class FunctionConsts
{

public:

typedef enum
{
  FN_UNKNOWN,

  FN_UNORDERED_1,

  FN_APPLY_1,

  FN_DATA_0,

  FN_HEAD_1,
  FN_TAIL_1,

  FN_ZORBA_MATCH_2,
  FN_ZORBA_EVAL_1,
  FN_ZORBA_EVAL_N_1,
  FN_ZORBA_EVAL_U_1,
  FN_ZORBA_EVAL_S_1,

  FN_ZORBA_INVOKE_N,
  FN_ZORBA_INVOKE_N_N,
  FN_ZORBA_INVOKE_U_N,
  FN_ZORBA_INVOKE_S_N,

  FN_ZORBA_XQDDF_PROBE_INDEX_POINT_VALUE_N,
  FN_ZORBA_XQDDF_PROBE_INDEX_POINT_VALUE_SKIP_N,
  FN_ZORBA_XQDDF_PROBE_INDEX_POINT_GENERAL_N,
  FN_ZORBA_XQDDF_PROBE_INDEX_RANGE_VALUE_N,
  FN_ZORBA_XQDDF_PROBE_INDEX_RANGE_VALUE_SKIP_N,
  FN_ZORBA_XQDDF_PROBE_INDEX_RANGE_GENERAL_N,
  OP_CREATE_INTERNAL_INDEX_2,
  FN_ZORBA_XQDDF_CREATE_INDEX_1,
  FN_ZORBA_XQDDF_DELETE_INDEX_1,
  FN_ZORBA_XQDDF_REFRESH_INDEX_1,
  OP_VALUE_INDEX_ENTRY_BUILDER_N,
  OP_GENERAL_INDEX_ENTRY_BUILDER_N,

  FN_EXACTLY_ONE_1,
  FN_MAX_1,
  FN_MAX_2,
  FN_MIN_1,
  FN_MIN_2,

  FN_BOOLEAN_1,
  FN_NOT_1,
  FN_FALSE_0,
  FN_TRUE_0,

  FN_NAME_0,
  FN_NAME_1,

  OP_EXACTLY_ONE_NORAISE_1,

  OP_UNION_2,
  OP_INTERSECT_2,
  OP_EXCEPT_2,

  OP_EQUAL_2,
  OP_NOT_EQUAL_2,
  OP_LESS_EQUAL_2,
  OP_GREATER_EQUAL_2,
  OP_LESS_2,
  OP_GREATER_2,

  OP_ATOMIC_VALUES_EQUIVALENT_2,

  OP_VALUE_EQUAL_2,
  OP_VALUE_EQUAL_DOUBLE_2,
  OP_VALUE_EQUAL_FLOAT_2,
  OP_VALUE_EQUAL_DECIMAL_2,
  OP_VALUE_EQUAL_INTEGER_2,
  OP_VALUE_EQUAL_STRING_2,
  OP_VALUE_NOT_EQUAL_2,
  OP_VALUE_NOT_EQUAL_DOUBLE_2,
  OP_VALUE_NOT_EQUAL_FLOAT_2,
  OP_VALUE_NOT_EQUAL_DECIMAL_2,
  OP_VALUE_NOT_EQUAL_INTEGER_2,
  OP_VALUE_NOT_EQUAL_STRING_2,
  OP_VALUE_LESS_EQUAL_2,
  OP_VALUE_LESS_EQUAL_DOUBLE_2,
  OP_VALUE_LESS_EQUAL_FLOAT_2,
  OP_VALUE_LESS_EQUAL_DECIMAL_2,
  OP_VALUE_LESS_EQUAL_INTEGER_2,
  OP_VALUE_LESS_EQUAL_STRING_2,
  OP_VALUE_LESS_2,
  OP_VALUE_LESS_DOUBLE_2,
  OP_VALUE_LESS_FLOAT_2,
  OP_VALUE_LESS_DECIMAL_2,
  OP_VALUE_LESS_INTEGER_2,
  OP_VALUE_LESS_STRING_2,
  OP_VALUE_GREATER_EQUAL_2,
  OP_VALUE_GREATER_EQUAL_DOUBLE_2,
  OP_VALUE_GREATER_EQUAL_FLOAT_2,
  OP_VALUE_GREATER_EQUAL_DECIMAL_2,
  OP_VALUE_GREATER_EQUAL_INTEGER_2,
  OP_VALUE_GREATER_EQUAL_STRING_2,
  OP_VALUE_GREATER_2,
  OP_VALUE_GREATER_DOUBLE_2,
  OP_VALUE_GREATER_FLOAT_2,
  OP_VALUE_GREATER_DECIMAL_2,
  OP_VALUE_GREATER_INTEGER_2,
  OP_VALUE_GREATER_STRING_2,

  OP_ADD_2,
  OP_SUBTRACT_2,
  OP_MULTIPLY_2,
  OP_DIVIDE_2,
  OP_INTEGER_DIVIDE_2,
  OP_MOD_2,

  OP_NUMERIC_ADD_2,
  OP_NUMERIC_ADD_DOUBLE_2,
  OP_NUMERIC_ADD_FLOAT_2,
  OP_NUMERIC_ADD_DECIMAL_2,
  OP_NUMERIC_ADD_INTEGER_2,
  OP_NUMERIC_SUBTRACT_2,
  OP_NUMERIC_SUBTRACT_DOUBLE_2,
  OP_NUMERIC_SUBTRACT_FLOAT_2,
  OP_NUMERIC_SUBTRACT_DECIMAL_2,
  OP_NUMERIC_SUBTRACT_INTEGER_2,
  OP_NUMERIC_MULTIPLY_2,
  OP_NUMERIC_MULTIPLY_DOUBLE_2,
  OP_NUMERIC_MULTIPLY_FLOAT_2,
  OP_NUMERIC_MULTIPLY_DECIMAL_2,
  OP_NUMERIC_MULTIPLY_INTEGER_2,
  OP_NUMERIC_DIVIDE_2,
  OP_NUMERIC_DIVIDE_DOUBLE_2,
  OP_NUMERIC_DIVIDE_FLOAT_2,
  OP_NUMERIC_DIVIDE_DECIMAL_2,
  OP_NUMERIC_DIVIDE_INTEGER_2,
  OP_NUMERIC_INTEGER_DIVIDE_2,
  OP_NUMERIC_MOD_2,

  OP_UNARY_PLUS_1,
  OP_UNARY_MINUS_1,
  OP_DOUBLE_UNARY_PLUS_1,
  OP_DOUBLE_UNARY_MINUS_1,

  FN_DATETIME_CONTRUCTOR,

  OP_ADD_YM_DURATIONS,
  OP_SUBTRACT_YM_DURATIONS,
  OP_MULTIPLY_YM_DURATION,
  OP_DIVIDE_YM_DURATION,
  OP_DIVIDE_YM_DURATIONS,
  OP_ADD_DT_DURATIONS,
  OP_SUBTRACT_DT_DURATIONS,
  OP_MULTIPLY_DT_DURATION,
  OP_DIVIDE_DT_DURATION,
  OP_DIVIDE_DT_DURATIONS,

  OP_SUBTRACT_DATETIMES,
  OP_SUBTRACT_DATES,
  OP_SUBTRACT_TIMES,

  OP_ADD_YMD_TO_DT,
  OP_ADD_DTD_TO_DT,
  OP_SUBTRACT_YMD_FROM_DT,
  OP_SUBTRACT_DTD_FROM_DT,
  OP_ADD_YMD_TO_D,
  OP_ADD_DTD_TO_D,
  OP_SUBTRACT_YMD_FROM_D,
  OP_SUBTRACT_DTD_FROM_D,
  OP_ADD_DTD_TO_T,
  OP_SUBTRACT_DTD_FROM_T,

  OP_ADJUST_DT_TO_TZ_1,
  OP_ADJUST_DT_TO_TZ_2,
  OP_ADJUST_D_TO_TZ_1,
  OP_ADJUST_D_TO_TZ_2,
  OP_ADJUST_T_TO_TZ_1,
  OP_ADJUST_T_TO_TZ_2,

  OP_FORMAT_DT_4,
  OP_FORMAT_DT_2,
  OP_FORMAT_D_4,
  OP_FORMAT_D_2,
  OP_FORMAT_T_4,
  OP_FORMAT_T_2,

  OP_AND_N,
  OP_OR_N,

  OP_VAR_GET_1,

  OP_EITHER_NODES_OR_ATOMICS_1,
  OP_DISTINCT_NODES_1,
  OP_CHECK_DISTINCT_NODES_1,
  OP_DISTINCT_NODES_OR_ATOMICS_1,
  OP_SORT_NODES_ASC_1,
  OP_SORT_NODES_ASC_OR_ATOMICS_1,
  OP_SORT_NODES_DESC_1,
  OP_SORT_NODES_DESC_OR_ATOMICS_1,
  OP_SORT_DISTINCT_NODES_ASC_1,
  OP_SORT_DISTINCT_NODES_ASC_OR_ATOMICS_1,
  OP_SORT_DISTINCT_NODES_DESC_1,
  OP_SORT_DISTINCT_NODES_DESC_OR_ATOMICS_1,

  OP_ENCLOSED_1,

  OP_HOIST_1,
  OP_UNHOIST_1,

#ifdef ZORBA_WITH_JSON
  JN_OBJECT_1,
#endif

#ifndef ZORBA_NO_FULL_TEXT
  FULL_TEXT_CURRENT_COMPARE_OPTIONS_0,
  FULL_TEXT_TOKENIZER_PROPERTIES_1,
  FULL_TEXT_TOKENIZER_PROPERTIES_0,
  FULL_TEXT_TOKENIZE_NODE_2,
  FULL_TEXT_TOKENIZE_NODE_1,
  FULL_TEXT_TOKENIZE_NODES_3,
  FULL_TEXT_TOKENIZE_NODES_2,
#endif /* ZORBA_NO_FULL_TEXT */

#include "functions/function_enum.h"

  FN_MAX_FUNC

} FunctionKind;


typedef enum
{
  NO = 0,
  YES,
  PRESERVE
} AnnotationValue;


typedef enum
{
  DoDistinct           = 1,   // Used by fn:zore-or-one and fn:exaclty-one
  SkipCodeGen          = 2,
  isDeterministic      = 4,
  isPrivate            = 8,
  isBuiltin            = 16,
  isUDF                = 32,
  propagatesInputNodes = 64,
  mustCopyInputNodes   = 128,
  AccessesDynCtx       = 256,
  ConstructsNodes      = 512,
  DereferencesNodes    = 1024,
  IsComparison         = 2048
} AnnotationFlags;

};

}

#endif

/*
 * Local variables:
 * mode: c++
 * End:
 */
/* vim:set et sw=2 ts=2: */
