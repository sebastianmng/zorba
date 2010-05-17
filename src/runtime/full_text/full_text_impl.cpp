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

#include "common/common.h"
#include "runtime/full_text/ftcontains_visitor.h"
#include "runtime/full_text/full_text.h"
#include "store/api/ft_token_iterator.h"
#include "store/api/item_factory.h"
#include "store/api/item.h"
#include "system/globalenv.h"
#include "zorbatypes/utf8.h"
#include "zorbautils/stl_helpers.h"

using namespace std;

namespace zorba {

///////////////////////////////////////////////////////////////////////////////

FTContainsIterator::FTContainsIterator(
  static_context *sctx,
  QueryLoc const &loc,
  PlanIter_t search_context,
  PlanIter_t ftignore_option,
  ftnode_t ftselection,
  sub_iter_list_t &sub_iters
) : 
  base_type( sctx, loc, search_context, ftignore_option ),
  ftselection_( ftselection )
{
  ZORBA_ASSERT( search_context );
  ZORBA_ASSERT( ftselection );
  sub_iters_.swap( sub_iters );
}

uint32_t FTContainsIterator::getStateSizeOfSubtree() const {
  uint32_t size = 0;
  FOR_EACH( sub_iter_list_t, i, sub_iters_ ) {
    size += (*i)->getStateSizeOfSubtree();
  }
  return base_type::getStateSizeOfSubtree() + size;
}

void FTContainsIterator::serialize( serialization::Archiver &ar ) {
  serialize_baseclass( ar, (base_type*)this );
  ar & ftselection_;
  ar & sub_iters_;
}

bool FTContainsIterator::nextImpl( store::Item_t &result,
                                   PlanState &plan_state ) const {
  bool ftcontains = false;
  store::Item_t item;                   // for the search context

  PlanIteratorState *state;
  DEFAULT_STACK_INIT( PlanIteratorState, state, plan_state );

  while ( !ftcontains && consumeNext( item, theChild0.getp(), plan_state ) ) {
    FTTokenIterator doc_tokens( item->getDocumentTokens() );
    if ( !doc_tokens.empty() ) {
      ftcontains_visitor v( doc_tokens, plan_state );
      ftselection_->accept( v );
      ftcontains = v.ftcontains();
    }
  }

  STACK_PUSH( GENV_ITEMFACTORY->createBoolean( result, ftcontains ), state );
  STACK_END( state );
}

void FTContainsIterator::openImpl( PlanState &state, uint32_t &offset ) {
  base_type::openImpl( state, offset );
  MUTATE_EACH( sub_iter_list_t, i, sub_iters_ ) {
    (*i)->open( state, offset );
  }
}

void FTContainsIterator::closeImpl( PlanState &state ) {
  MUTATE_EACH( sub_iter_list_t, i, sub_iters_ ) {
    (*i)->close( state );
  }
  base_type::closeImpl( state );
}

void FTContainsIterator::resetImpl( PlanState &state ) const {
  base_type::resetImpl( state );
  FOR_EACH( sub_iter_list_t, i, sub_iters_ ) {
    (*i)->reset( state );
  }
}

} // namespace zorba
/* vim:set et sw=2 ts=2: */
