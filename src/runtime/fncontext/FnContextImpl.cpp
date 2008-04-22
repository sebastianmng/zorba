/*
 *  Copyright 2006-2007 FLWOR Foundation.
 *  Authors: David Graf, Tim Kraska, Markos Zaharioudakis
 */

#include "compiler/api/compiler_api.h"

#include "runtime/fncontext/FnContextImpl.h"
#include "runtime/core/item_iterator.h"
#include "runtime/api/runtimecb.h"
#include "runtime/api/plan_iterator_wrapper.h"

#include "context/dynamic_context.h"
#include "context/static_context.h"

#include "store/api/store.h"
#include "store/api/iterator.h"
#include "store/api/temp_seq.h"

#include "system/globalenv.h"

using namespace std;

namespace zorba
{

store::Item_t CtxVariableIterator::nextImpl(PlanState& planState) const
{
  store::Item_t item, varName;
  xqpStringStore dot (".");
  
  CtxVariableIteratorState* state;
  DEFAULT_STACK_INIT(CtxVariableIteratorState, state, planState);

  varName = consumeNext(theChildren[0].getp(), planState);

  if (varName == NULL)
    return NULL;

	if(varName->getStringValue ()->equals (&dot)) {  // looking for context item?
    item = planState.theRuntimeCB->theDynamicContext->context_item();
		if(item == NULL)
			ZORBA_ERROR_LOC_PARAM( ZorbaError::XPDY0002, loc, "context item", "");
		STACK_PUSH( item, state);
	} else {
    state->theIter = planState.theRuntimeCB->theDynamicContext->
                     get_variable(varName);

    if (state->theIter == NULL)
			ZORBA_ERROR_LOC_PARAM( ZorbaError::XPDY0002, loc, varName->getStringValue (), "");

    state->theIter->open();

    while ( (item = state->theIter->next()) != NULL )
			STACK_PUSH (item, state);

    state->theIter->close();
	}

  STACK_END (state);
}

  static PlanIter_t compile (XQueryCompiler &compiler, xqp_string query) {
    istringstream os (query);
    return compiler.compile (os);
  }

store::Item_t EvalIterator::nextImpl(PlanState& planState) const {
  store::Item_t item;
  EvalIteratorState* state;
  CompilerCB *ccb = new CompilerCB (*planState.theCompilerCB);
  XQueryCompiler compiler (ccb);
  auto_ptr<dynamic_context> dctx (new dynamic_context (planState.dctx ()));

  DEFAULT_STACK_INIT(EvalIteratorState, state, planState);

#if 0
#endif

  state->ccb.reset (ccb);
  ccb->m_sctx = ccb->m_sctx->create_child_context ();
  item = consumeNext (theChildren [0].getp (), planState);
  state->eval_plan.reset (new PlanWrapper (compile (compiler, &*item->getStringValue ()), ccb, dctx.get ()));

  for (unsigned i = 0; i < theChildren.size () - 1; i++) {
    store::Iterator_t lIter = new PlanIteratorWrapper (theChildren [i + 1], planState);
    dctx->add_variable (ccb->m_sctx->qname_internal_key (varnames [i]),
                        GENV_STORE.createTempSeq (lIter)->getIterator ());
  }

  while (NULL != (item = state->eval_plan->next ())) {
    STACK_PUSH (item, state);
  }

  STACK_END (state);
}


}
