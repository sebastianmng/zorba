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
#include "stdafx.h"

#include <algorithm>

#include "types/typeops.h"

#include "functions/function.h"
#include "functions/library.h"

#include "compiler/xqddf/value_index.h"
#include "compiler/api/compilercb.h"
#include "compiler/expression/flwor_expr.h"
#include "compiler/expression/fo_expr.h"
#include "compiler/expression/script_exprs.h"
#include "compiler/expression/expr.h"
#include "compiler/expression/expr_iter.h"
#include "compiler/expression/expr_manager.h"
#include "compiler/codegen/plan_visitor.h"
#include "compiler/rewriter/framework/rewriter_context.h"
#include "compiler/rewriter/rules/fold_rules.h"

#include "runtime/base/plan_iterator.h"
#include "runtime/indexing/doc_indexer.h"

#include "store/api/item_factory.h"

#include "diagnostics/util_macros.h"


namespace zorba
{

SERIALIZABLE_CLASS_VERSIONS(OrderModifier)

SERIALIZABLE_CLASS_VERSIONS(IndexDecl)


/*******************************************************************************

********************************************************************************/
IndexDecl::IndexDecl(
    static_context* sctx,
    CompilerCB* ccb,
    const QueryLoc& loc,
    const store::Item_t& name)
  :
  theCCB(ccb),
  theLocation(loc),
  theSctx(sctx),
  theName(name),
  theIsGeneral(false),
  theIsUnique(false),
  theIsTemp(false),
  theMaintenanceMode(MANUAL),
  theContainerKind(HASH),
  theDomainClause(NULL),
  theDomainExpr(NULL),
  theDomainVar(NULL),
  theDomainPosVar(NULL),
  theViewExpr(NULL),
  theBuildExpr(NULL),
  theDocIndexerExpr(NULL)
{
}


/*******************************************************************************

********************************************************************************/
IndexDecl::IndexDecl(::zorba::serialization::Archiver& ar)
  :
  SimpleRCObject(ar),
  theDomainClause(NULL),
  theDomainExpr(NULL),
  theDomainVar(NULL),
  theDomainPosVar(NULL),
  theViewExpr(NULL),
  theBuildExpr(NULL),
  theDocIndexerExpr(NULL)
{
}


/*******************************************************************************

********************************************************************************/
void IndexDecl::serialize(::zorba::serialization::Archiver& ar)
{
  ar & theSctx;
  ar & theName;
  ar & theIsGeneral;
  ar & theIsUnique;
  ar & theIsTemp;
  SERIALIZE_ENUM(MaintenanceMode, theMaintenanceMode);
  SERIALIZE_ENUM(ContainerKind, theContainerKind);
  //ar & theDomainClause;
  //ar & theKeyExprs;
  ar & theNumKeyExprs;
  ar & theKeyTypes;
  ar & theOrderModifiers;

  ar & theSourceNames;
  //ar & theDomainSourceExprs;

  ar & theBuildPlan;
  ar & theDocIndexerPlan;
}


/*******************************************************************************

********************************************************************************/
IndexDecl::~IndexDecl()
{
}


/*******************************************************************************

********************************************************************************/
store::Item* IndexDecl::getName() const
{
  return theName.getp();
}


/*******************************************************************************

********************************************************************************/
expr* IndexDecl::getDomainExpr() const
{
  return theDomainExpr;
}


/*******************************************************************************

********************************************************************************/
void IndexDecl::setDomainExpr(expr* domainExpr)
{
  theDomainExpr = domainExpr;

  if (theDomainClause == NULL)
    theDomainClause = theCCB->theEM->create_for_clause(domainExpr->get_sctx(),
                                     domainExpr->get_loc(),
                                     NULL,
                                     NULL);

  theDomainClause->set_expr(domainExpr);
}


/*******************************************************************************

********************************************************************************/
var_expr* IndexDecl::getDomainVariable() const
{
  return theDomainVar;
}


/*******************************************************************************

********************************************************************************/
void IndexDecl::setDomainVariable(var_expr* domainVar)
{
  theDomainVar = domainVar;

  if (theDomainClause == NULL)
    theDomainClause = theCCB->theEM->create_for_clause(domainVar->get_sctx(),
                                     domainVar->get_loc(),
                                     NULL,
                                     NULL);

  theDomainClause->set_var(domainVar);
}


/*******************************************************************************

********************************************************************************/
var_expr* IndexDecl::getDomainPositionVariable() const
{
  return theDomainPosVar;
}


/*******************************************************************************

********************************************************************************/
void IndexDecl::setDomainPositionVariable(var_expr* domainPosVar)
{
  theDomainPosVar = domainPosVar;

  theDomainClause->set_pos_var(domainPosVar);
}


/*******************************************************************************

********************************************************************************/
void IndexDecl::setKeyExpressions(const std::vector<expr*>& keyExprs)
{
  theKeyExprs = keyExprs;
  theNumKeyExprs = theKeyExprs.size();
}


/*******************************************************************************

********************************************************************************/
const std::vector<xqtref_t>& IndexDecl::getKeyTypes() const
{
  return theKeyTypes;
}


/*******************************************************************************

********************************************************************************/
void IndexDecl::setKeyTypes(const std::vector<xqtref_t>& keyTypes)
{
  assert(!keyTypes.empty());

  theKeyTypes = keyTypes;

  if (theIsGeneral && theIsUnique)
  {
    TypeManager* tm = theSctx->get_typemanager();

    xqtref_t type = theKeyTypes[0];
    xqtref_t ptype;

    if (type != NULL)
      ptype = TypeOps::prime_type(tm, *type);

    if (ptype == NULL ||
        TypeOps::is_equal(tm, *ptype, *GENV_TYPESYSTEM.ANY_ATOMIC_TYPE_ONE) ||
        TypeOps::is_subtype(tm, *ptype, *GENV_TYPESYSTEM.UNTYPED_ATOMIC_TYPE_ONE))
    {
      RAISE_ERROR(zerr::ZDST0025_INDEX_BAD_UNIQUE_PROPERTY, theLocation,
      ERROR_PARAMS(theName->getStringValue()));
    }
  }
}


/*******************************************************************************

********************************************************************************/
const std::vector<OrderModifier>& IndexDecl::getOrderModifiers() const
{
  return theOrderModifiers;
}


/*******************************************************************************

********************************************************************************/
void IndexDecl::setOrderModifiers(const std::vector<OrderModifier>& modifiers)
{
  theOrderModifiers = modifiers;
}


/*******************************************************************************

********************************************************************************/
const std::string& IndexDecl::getCollation(csize i) const
{
  return theOrderModifiers[i].theCollation;
}


/******************************************************************************
  Check that the domain and key exprs satisfy the constraints specified by the
  XQDDF spec. This method is called from the translator, after the domain and
  key exprs have been translated and optimized.
*******************************************************************************/
void IndexDecl::analyze()
{
  store::Item_t dotQName;
  GENV_ITEMFACTORY->createQName(dotQName, "", "", static_context::DOT_VAR_NAME);
  expr* dotVar = NULL;

  // Get the var_expr representing the context item, if it is defined
  VarInfo* var = theSctx->lookup_var(dotQName);

  if (var)
    dotVar = var->getVar();

  std::vector<var_expr*> varExprs;

  // Check constraints on the domain expr
  analyzeExprInternal(getDomainExpr(),
                      theSourceNames,
                      theDomainSourceExprs,
                      varExprs,
                      dotVar);

  std::vector<expr*> keySources;

  csize numKeys = theKeyExprs.size();

  if (theIsGeneral && numKeys > 1)
  {
    RAISE_ERROR(zerr::ZDST0035_INDEX_GENERAL_MULTIKEY, theKeyExprs[1]->get_loc(),
    ERROR_PARAMS(theName->getStringValue()));
  }

  // Check constraints on the key exprs
  for (csize i = 0; i < numKeys; ++i)
  {
    varExprs.clear();

    analyzeExprInternal(theKeyExprs[i],
                        theSourceNames,
                        keySources,
                        varExprs,
                        dotVar);
  }

  // If the index is declared as "automatically maintained", check whether
  // automatic maintence can be done efficiently, and if so, set the appropriate
  // maintenance mode.
  if (keySources.empty() &&
      theDomainSourceExprs.size() == 1 &&
      theMaintenanceMode != MANUAL)
  {
    if (getDomainExpr()->is_map(theDomainSourceExprs[0], theSctx))
      theMaintenanceMode = DOC_MAP;
  }

  if (theMaintenanceMode == REBUILD)
  {
    // If the index is declared as "automatically maintained", then
    // theMaintenanceMode is initially set to REBUILD. If theMaintenanceMode
    // is not changed above (to DOC_MAP), then we throw an error because we
    // don't want to automatically rebuild the full index with every update.
    RAISE_ERROR(zerr::ZDST0034_INDEX_CANNOT_DO_AUTOMATIC_MAINTENANCE,
    getDomainExpr()->get_loc(),
    ERROR_PARAMS(theName->getStringValue()));
  }
  else if (theMaintenanceMode == DOC_MAP)
  {
    // Have to do this here (rather than during runtime) so that we don't have to
    // serialize the index exprs.
    (void)getDocIndexer(theLocation);
  }

  // Have to do this here (rather than during runtime) so that we don't have to
  // serialize the index exprs.
  (void)getBuildPlan(theLocation);
}


/*******************************************************************************
  Check that the given expr
  (a) is deterministic,
  (b) does not reference any variables other than those in varExprs
  (c) does not reference the given dotVar
  (c) does not reference any input functions other than xqddf:collection(qname)
  (d) the arg to each xqddf:collection is a const qname

  If the above conditions are met, the method will return the qnames of all the
  accessed collections and the fo exprs representing the xqddf:collection
  invocations.
********************************************************************************/
void IndexDecl::analyzeExprInternal(
    expr* e,
    std::vector<store::Item*>& sourceNames,
    std::vector<expr*>& sourceExprs,
    std::vector<var_expr*>& varExprs,
    expr* dotVar)
{
  switch (e->get_expr_kind())
  {
  case fo_expr_kind:
  {
    fo_expr* foExpr = static_cast<fo_expr*>(e);
    const function* func = foExpr->get_func();

    if (!func->isDeterministic())
    {
      RAISE_ERROR(zerr::ZDST0028_INDEX_NOT_DETERMINISTIC, e->get_loc(),
      ERROR_PARAMS(theName->getStringValue()));
    }

    if (func->isSource())
    {
      if (func->getKind() == FunctionConsts::ZORBA_STORE_STATIC_COLLECTIONS_DML_COLLECTION_1)
      {
        const expr* argExpr = foExpr->get_arg(0);

        const store::Item* qname = argExpr->getQName();

        if (qname != NULL)
        {
          sourceNames.push_back((store::Item*)qname);
          sourceExprs.push_back(foExpr);
        }
        else
        {
          RAISE_ERROR(zerr::ZDST0030_INDEX_NON_CONST_DATA_SOURCE, e->get_loc(),
          ERROR_PARAMS(theName->getStringValue()));
        }
      }
      else
      {
        RAISE_ERROR(zerr::ZDST0029_INDEX_INVALID_DATA_SOURCE, e->get_loc(),
        ERROR_PARAMS(theName->getStringValue()));
      }
    }

    break;
  }
  case var_decl_expr_kind:
  {
    var_expr* varExpr = static_cast<var_decl_expr*>(e)->get_var_expr();

    ZORBA_ASSERT(varExpr->get_kind() == var_expr::local_var);

    varExprs.push_back(varExpr);

    break;
  }
  case flwor_expr_kind:
  {
    static_cast<const flwor_expr*>(e)->get_vars(varExprs);

    break;
  }
  case var_expr_kind:
  {
    if (e == dotVar)
    {
      RAISE_ERROR(zerr::ZDST0032_INDEX_REFERENCES_CTX_ITEM, e->get_loc(),
      ERROR_PARAMS(theName->getStringValue()));
    }

    var_expr* var = static_cast<var_expr*>(e);

    if (e != getDomainVariable() &&
        std::find(varExprs.begin(), varExprs.end(), var) == varExprs.end())
    {
      RAISE_ERROR(zerr::ZDST0031_INDEX_HAS_FREE_VARS,  e->get_loc(),
      ERROR_PARAMS(theName->getStringValue()));
    }

    break;
  }
  default:
  {
    break;
  }
  }

  ExprIterator iter(e);
  while (!iter.done())
  {
    analyzeExprInternal((**iter), sourceNames, sourceExprs, varExprs, dotVar);
    iter.next();
  }
}


/******************************************************************************
  Create the expression that represents the index as a view.
 
  For now, this is done for value indexes only

  for $newdot at $newpos in cloned_domain_expr
  let  $key_1 := new_key_expr_1
  .....
  let $key_N := new_key_expr_N
  return $newdot
*******************************************************************************/
flwor_expr* IndexDecl::getViewExpr(std::vector<let_clause*>*& keyClauses)
{
  if (theViewExpr != NULL)
  {
    keyClauses = &theKeyClauses;
    return theViewExpr;
  }

  theDomainClause = NULL;

  expr* domainExpr = getDomainExpr();
  var_expr* dot = getDomainVariable();
  var_expr* pos = getDomainPositionVariable();
  static_context* sctx = domainExpr->get_sctx();
  user_function* udf = domainExpr->get_udf();

  assert(theIsTemp || udf == NULL);

  const QueryLoc& domloc = domainExpr->get_loc();

  // Clone the domain expr, the domain variable, and the domain pos variable.
  // These 2 vars are referenced by the key exprs..
  expr::substitution_t subst;
  expr* newdom = domainExpr->clone(udf, subst);

  var_expr* newdot = theCCB->theEM->
  create_var_expr(sctx, udf, domloc, dot->get_kind(), dot->get_name());

  var_expr* newpos = theCCB->theEM->
  create_var_expr(sctx, udf, domloc, pos->get_kind(), pos->get_name());

  //
  // Create for clause (this has to be done here so that the cloned dot var gets
  // associated with the cloned domain expr; this is needed before cloning the
  // key expr) :
  //
  // for $newdot at $newpos in new_domain_expr
  //
  for_clause* fc = theCCB->theEM->
  create_for_clause(sctx, domloc, newdot, newdom, newpos);

  //
  // Create flwor expr:
  //
  // for $newdot at $newpos in new_domain_expr
  // return $newdot
  //

  expr* returnExpr = theCCB->theEM->create_wrapper_expr(sctx, udf, domloc, newdot);

  flwor_expr* flworExpr = theCCB->theEM->create_flwor_expr(sctx, udf, domloc);
  flworExpr->set_return_expr(returnExpr);
  flworExpr->add_clause(fc);

  //
  // Handle the key exprs
  //
  // for $newdot at $newpos in new_domain_expr
  // let $key_1 := new_key_expr_1
  // .....
  // let $key_N := new_key_expr_N
  // where $key_1 eq $arg_1 and ... and $key_N eq $arg_N
  // return $newdot
  //
  //function* compFunc = BUILTIN_FUNC(OP_EQUAL_2);
  //std::vector<expr*> predExprs;
  csize numKeys = theKeyExprs.size();

  theKeyClauses.reserve(numKeys);

  for (csize i = 0; i < numKeys; ++i)
  {
    // clone the key expr
    subst.clear();
    subst[dot] = newdot;
    subst[pos] = newpos;

    expr* keyClone = theKeyExprs[i]->clone(udf, subst);

    keyClone->set_scripting_detail(SEQUENTIAL_FUNC_EXPR);

    const QueryLoc& keyloc = keyClone->get_loc();

    // create the LET clause
    std::string localName = "$$key_" + ztd::to_string(i);
    store::Item_t keyVarName;
    GENV_ITEMFACTORY->createQName(keyVarName, "", "", localName);

    var_expr* keyVar = theCCB->theEM->
    create_var_expr(sctx, udf, keyloc, var_expr::let_var, keyVarName);

    let_clause* lc = theCCB->theEM->create_let_clause(sctx, keyloc, keyVar, keyClone);

    flworExpr->add_clause(lc);
    theKeyClauses.push_back(lc);
  }

  theViewExpr = flworExpr;

  // We apply the fold rules on the view expr in order to flatten a
  std::ostringstream msg;
  msg << "normalization of candidate index: " << getName()->getStringValue();

  RewriterContext rCtx(theViewExpr->get_ccb(),
                       theViewExpr,
                       theViewExpr->get_udf(),
                       msg.str(),
                       true);
  FoldRules foldRules;
  foldRules.rewrite(rCtx);

  ZORBA_ASSERT(theViewExpr == rCtx.getRoot());

  keyClauses = &theKeyClauses;
  return theViewExpr;
}


/******************************************************************************
  Create the expression that "builds" the index, if not done already. The expr
  to build is the following, for value and general indexes, respectively:

  for $newdot at $newpos in cloned_domain_expr
  return value-index-entry-builder($$newdot, cloned_key1_expr, ..., cloned_keyN_expr)

  for $$newdot at $$newpos in cloned_domain_expr
  return general-index-entry-builder($$newdot, cloned_key_expr);

  The runtime plan corresponding to this expr is given to the store, which
  then populates the index by creating entries out of the items returned by
  this expr.
*******************************************************************************/
expr* IndexDecl::getBuildExpr(const QueryLoc& loc)
{
  if (theBuildExpr != NULL)
    return theBuildExpr;

  theDomainClause = NULL;

  expr* domainExpr = getDomainExpr();
  var_expr* dot = getDomainVariable();
  var_expr* pos = getDomainPositionVariable();
  static_context* sctx = domainExpr->get_sctx();
  user_function* udf = domainExpr->get_udf();

  assert(theIsTemp || udf == NULL);

  const QueryLoc& dotloc = dot->get_loc();

  csize numKeys = theKeyExprs.size();
  std::vector<expr*> clonedExprs(numKeys + 1);

  //
  // Clone the domain expr.
  //
  expr::substitution_t subst;
  expr* newdom = domainExpr->clone(udf, subst);

  //
  // Clone the domain variable and the domain pos variable. These vars are
  // referenced by the key exprs.
  //
  var_expr* newdot = theCCB->theEM->
  create_var_expr(sctx, udf, dotloc, dot->get_kind(), dot->get_name());

  var_expr* newpos = theCCB->theEM->
  create_var_expr(sctx, udf, dotloc, pos->get_kind(), pos->get_name());

  //
  // Create for clause (this has to be done here so that the cloned dot var gets
  // associated with the cloned domain expr; this is needed before cloning the
  // key expr) :
  //
  // for $newdot at $newpos in new_domain_expr
  //
  for_clause* fc = theCCB->theEM->
  create_for_clause(sctx, dotloc, newdot, newdom, newpos);

  //
  // Clone the key exprs, replacing their references to the 2 domain variables
  // with the clones of these variables.
  //
  for (csize i = 0; i < numKeys; ++i)
  {
    subst.clear();
    subst[dot] = newdot;
    subst[pos] = newpos;

    clonedExprs[i+1] = theKeyExprs[i]->clone(udf, subst);
  }

  //
  // Create flwor expr:
  //
  // for $newdot at $newpos in new_domain_expr
  // return index-entry-builder($$newdot, new_key1_expr, ..., new_keyN_expr)
  //

  expr* domainVarExpr(theCCB->theEM->create_wrapper_expr(sctx, udf, loc, newdot));

  clonedExprs[0] = domainVarExpr;

  function* f = NULL;

  if (theIsGeneral)
    f = BUILTIN_FUNC(OP_GENERAL_INDEX_ENTRY_BUILDER_N);
  else
    f = BUILTIN_FUNC(OP_VALUE_INDEX_ENTRY_BUILDER_N);

  ZORBA_ASSERT(f != NULL);

  fo_expr* returnExpr =  theCCB->theEM->create_fo_expr(sctx, udf, loc, f, clonedExprs);

  flwor_expr* flworExpr = theCCB->theEM->create_flwor_expr(sctx, udf, loc);
  flworExpr->set_return_expr(returnExpr);
  flworExpr->add_clause(fc);

  theBuildExpr = flworExpr;

  if (theCCB->theConfig.optimize_cb != NULL)
  {
    std::string msg = "build expr for index " + theName->getStringValue().str();

    theCCB->theConfig.optimize_cb(theBuildExpr, msg);
  }

  return theBuildExpr;
}


/*******************************************************************************

********************************************************************************/
PlanIterator* IndexDecl::getBuildPlan(const QueryLoc& loc)
{
  if (theBuildPlan != NULL)
    return theBuildPlan.getp();

  expr* buildExpr = getBuildExpr(loc);

  ulong nextVarId = 1;
  theBuildPlan = codegen("index", buildExpr, theCCB, nextVarId);

  return theBuildPlan.getp();
}


/*******************************************************************************
  Called from ApplyIterator::nextImpl before it actually starts applying the
  updates.
********************************************************************************/
DocIndexer* IndexDecl::getDocIndexer(const QueryLoc& loc)
{
  if (theDocIndexer != NULL)
    return theDocIndexer.getp();

  if (theMaintenanceMode != DOC_MAP)
    return NULL;

  std::string varname = "$$idx_doc_var";
  store::Item_t docVarName;
  GENV_ITEMFACTORY->createQName(docVarName, "", "", varname.c_str());

  csize numKeys = theNumKeyExprs;

  if (theDocIndexerPlan != NULL)
  {
    theDocIndexer = new DocIndexer(isGeneral(), numKeys, theDocIndexerPlan, docVarName);

    return theDocIndexer.getp();
  }

  expr* domainExpr = getDomainExpr();
  var_expr* dot = getDomainVariable();
  var_expr* pos = getDomainPositionVariable();

  static_context* sctx = domainExpr->get_sctx();
  user_function* udf = domainExpr->get_udf();

  assert(udf == NULL);

  const QueryLoc& dotloc = dot->get_loc();

  std::vector<expr*> clonedExprs(numKeys + 1);

  //
  // Clone the domain expr and replace the reference to the collection with a
  // reference to a new prolog variable that will be bound to a set of docs
  // during the apply-updates.
  //

  var_expr* docVar = theCCB->theEM->
  create_var_expr(sctx, udf, dot->get_loc(), var_expr::prolog_var, docVarName);

  docVar->set_unique_id(1);
  ulong nextVarId = 2;

  expr* wrapperExpr = theCCB->theEM->
  create_wrapper_expr(sctx, udf, dot->get_loc(), docVar);

  docVar->set_type(domainExpr->get_return_type());

  expr::substitution_t subst;

  subst[theDomainSourceExprs[0]] = wrapperExpr;

  expr* newdom = domainExpr->clone(udf, subst);

  //
  // Clone the domain variable and the domain pos variable. These vars are
  // referenced by the key exprs.
  //
  var_expr* newdot = theCCB->theEM->
  create_var_expr(sctx, udf, dotloc, dot->get_kind(), dot->get_name());

  var_expr* newpos = theCCB->theEM->
  create_var_expr(sctx, udf, dotloc, pos->get_kind(), pos->get_name());

  //
  // Create for clause (this has to be done here so that the cloned dot var gets
  // associated with the cloned domain expr; this is needed before cloning the
  // key expr) :
  //
  // for $newdot at $newpos in new_domain_expr
  //
  for_clause* fc = theCCB->theEM->
  create_for_clause(sctx, dotloc, newdot, newdom, newpos);

  //
  // Clone the key exprs, replacing their references to the 2 domain variables
  // with the clones of these variables.
  //
  for (csize i = 0; i < numKeys; ++i)
  {
    subst.clear();
    subst[dot] = newdot;
    subst[pos] = newpos;

    clonedExprs[i+1] = theKeyExprs[i]->clone(udf, subst);
  }

  //
  // Create flwor expr:
  //
  // for $newdot at $newpos in new_domain_expr
  // return index-entry-builder($$newdot, new_key1_expr, ..., new_keyN_expr)
  //

  expr* domainVarExpr = theCCB->theEM->create_wrapper_expr(sctx, udf, loc, newdot);

  clonedExprs[0] = domainVarExpr;

  function* f = NULL;

  if (theIsGeneral)
    f = BUILTIN_FUNC(OP_GENERAL_INDEX_ENTRY_BUILDER_N);
  else
    f = BUILTIN_FUNC(OP_VALUE_INDEX_ENTRY_BUILDER_N);

  ZORBA_ASSERT(f != NULL);

  fo_expr* returnExpr =  theCCB->theEM->create_fo_expr(sctx, udf, loc, f, clonedExprs);

  flwor_expr* flworExpr = theCCB->theEM->create_flwor_expr(sctx, udf, loc);
  flworExpr->set_return_expr(returnExpr);
  flworExpr->add_clause(fc);

  if (theCCB->theConfig.optimize_cb != NULL)
  {
    std::string msg = "entry-creator expr for index " + theName->getStringValue().str();

    theCCB->theConfig.optimize_cb(flworExpr, msg);
  }

  theDocIndexerExpr = flworExpr;

  //
  // Generate the runtime plan for theDocIndexerExpr
  //
  theDocIndexerPlan = codegen("doc indexer", flworExpr, theCCB, nextVarId);

  //
  // Create theDocIndexer obj
  //
  theDocIndexer = new DocIndexer(isGeneral(), numKeys, theDocIndexerPlan, docVarName);

  return theDocIndexer.getp();
}


/*******************************************************************************

********************************************************************************/
std::string IndexDecl::toString()
{
  std::ostringstream os;

  os << "Index : " << theName->getStringValue() << std::endl;

  os << "Domain Expr : " << std::endl;

  getDomainExpr()->put(os) << std::endl;

  os << "Domain Variable : ";
  getDomainVariable()->put(os);

  csize numColumns = theKeyExprs.size();

  for (csize i = 0; i < numColumns; ++i)
  {
    os << std::endl << "Key Expr " << i << " : " << std::endl;
    theKeyExprs[i]->put(os);
  }

  return os.str();
}


} // namespace zorba
/* vim:set et sw=2 ts=2: */
