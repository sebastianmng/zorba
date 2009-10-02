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
#include "functions/Index.h"
#include "functions/function_impl.h"

#include "runtime/indexing/value_index_builder.h"
#include "runtime/indexing/value_index_probe.h"


namespace zorba
{

/******************************************************************************
  create-internal-index($indexName as xs:QName) as ()
********************************************************************************/
class CreateInternalIndex : public function 
{
public:
  CreateInternalIndex(const signature& sig) 
    :
    function(sig, FunctionConsts::FN_CREATE_INTERNAL_INDEX)
  {
  }

  virtual bool requires_dyn_ctx() const { return true; }

  CODEGEN_DECL();
};


CODEGEN_DEF(CreateInternalIndex)
{
  // There is no single iterator that implements this function. Instead, a
  // whole sub-plan is generated to build the index. This is done in 
  // plan_visitor.cpp because the sun-plan must be generated by the same 
  // visitor as th rest of the query plan.
  ZORBA_ASSERT(false);
}


/******************************************************************************
  create-index($indexName as xs:QName) as ()

  Creates the index container in the store and populates it according to the
  index definition. Also creates in the dynamic context an entry that maps the
  index qname and an index object that is returned by the store to act as a
  handle to the index. The specification for the index to create is taken from
  the static context, which stores a mapping from the index uri to ValueIndex
  obj (defined in indexing/value_index.h).

    for $$dot at $$pos in domainExpr
    return concatenate( $$dot, fieldExpr1, ..., fieldExprN);

********************************************************************************/
class zop_createindex : public function 
{
public:
  zop_createindex(const signature& sig) : function(sig) { }

  virtual bool requires_dyn_ctx() const { return true; }

  DEFAULT_UNARY_CODEGEN(CreateValueIndex);
};


/******************************************************************************
  drop-index($indexName as xs:QName) as ()

  Destroys the index container in the store and removes the index entry from
  the dynamic context. 
********************************************************************************/
class zop_dropindex : public function 
{
public:
  zop_dropindex(const signature& sig) : function(sig) { }

  virtual bool requires_dyn_ctx() const { return true; }

  DEFAULT_UNARY_CODEGEN(DropValueIndex);
};


/***************************************************************************//**
  probe-index-point($indexName as xs:QName,
                    $key1  as anyAtomic?,
                    ...,
                    $keyN  as anyAtomic?) as item()*
********************************************************************************/
class zop_probeindexpoint : public function 
{
public:
  zop_probeindexpoint(const signature& sig) : function(sig) { }

  virtual bool requires_dyn_ctx () const { return true; }

  DEFAULT_NARY_CODEGEN(ValueIndexPointProbe);
};


/*******************************************************************************
  item* probe-index-range($indexName               as xs:QName,
                          range1LowerBound         as anyAtomic?,
                          range1UpperBound         as anyAtomic?,
                          range1HaveLowerBound     as boolean?,
                          range1HaveupperBound     as boolean?,
                          range1LowerBoundIncluded as boolean?,
                          range1upperBoundIncluded as boolean?,
                          ....,
                          rangeNLowerBound         as anyAtomic?,
                          rangeNUpperBound         as anyAtomic?,
                          rangeNHaveLowerBound     as boolean?,
                          rangeNHaveupperBound     as boolean?,
                          rangeNLowerBoundIncluded as boolean?,
                          rangeNupperBoundIncluded as boolean?)
********************************************************************************/
class zop_probeindexrange : public function 
{
public:
  zop_probeindexrange(const signature& sig) : function(sig) { }

  virtual bool requires_dyn_ctx () const { return true; }

  DEFAULT_NARY_CODEGEN(ValueIndexRangeProbe);
};



void populateContext_Index(static_context* sctx)
{
  DECL(sctx, CreateInternalIndex,
       (createQName(ZORBA_OPEXTENSIONS_NS,"op-extensions", "create-internal-index"),
        GENV_TYPESYSTEM.QNAME_TYPE_ONE,
        GENV_TYPESYSTEM.EMPTY_TYPE));

  DECL(sctx, zop_createindex,
       (createQName(ZORBA_OPEXTENSIONS_NS,"op-extensions", "create-index"),
        GENV_TYPESYSTEM.QNAME_TYPE_ONE,
        GENV_TYPESYSTEM.EMPTY_TYPE));

  DECL(sctx, zop_dropindex,
       (createQName(ZORBA_OPEXTENSIONS_NS,"op-extensions", "drop-index"),
        GENV_TYPESYSTEM.QNAME_TYPE_ONE,
        GENV_TYPESYSTEM.EMPTY_TYPE));

  DECL(sctx, zop_probeindexpoint,
       (createQName(ZORBA_OPEXTENSIONS_NS,"op-extensions", "probe-index-point"),
        GENV_TYPESYSTEM.ANY_ATOMIC_TYPE_QUESTION,
        true,
        GENV_TYPESYSTEM.ITEM_TYPE_STAR));

  DECL(sctx, zop_probeindexrange,
       (createQName(ZORBA_OPEXTENSIONS_NS,"op-extensions", "probe-index-range"),
        GENV_TYPESYSTEM.ANY_ATOMIC_TYPE_QUESTION,
        true,
        GENV_TYPESYSTEM.ITEM_TYPE_STAR));
}


}
/* vim:set ts=2 sw=2: */
