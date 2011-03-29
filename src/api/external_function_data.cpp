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

#include <zorba/external_function_data.h>
#include <zorba/item.h>
#include <zorba/item_sequence.h>
#include <zorba/iterator.h>

#include "api/unmarshaller.h"
#include "store/api/item.h"
#include "store/api/item_factory.h"
#include "system/globalenv.h"
#include "zorbaerrors/assert.h"
#include "zorbaerrors/user_exception.h"
#include "zorbatypes/zstring.h"

namespace zorba {
  
store::Item_t
getErrorQName(const Item& aQName)
{
  store::Item_t lErrorQName;
  if (aQName.isNull()) {
    static const char *lErrorNamespace = "http://www.w3.org/2005/xqt-errors";
    GENV_ITEMFACTORY->createQName (lErrorQName, lErrorNamespace, "err", "FOER0000");
  } else {
    lErrorQName = Unmarshaller::getInternalItem(aQName);
  }
  ZORBA_ASSERT(lErrorQName != NULL);
  return lErrorQName;
}


void
ExternalFunctionData::error()
{
  Item lItem;
  error(lItem);
}


void
ExternalFunctionData::error(const Item& aQName)
{
  String lString;
  error(aQName, lString);
}


void
ExternalFunctionData::error(const Item& aQName, const String& aDescription)
{
  ItemSequence_t lNullSeq;
  error(aQName, aDescription, lNullSeq);
}


void
ExternalFunctionData::error(
    const Item& aQName,
    const String& aDescription,
    const ItemSequence_t& aErrorObject)
{
  UserException::error_object_type lErrorObject;

  if (aErrorObject.get() != 0) {
    Item lTmpItem;
    Iterator_t err_iter = aErrorObject->getIterator();
    if(err_iter != NULL)
    {
      err_iter->open();
      while (err_iter->next(lTmpItem)) {
        lErrorObject.push_back( lTmpItem );
      }
      err_iter->close();
    }
  }

  throw USER_EXCEPTION(
    getErrorQName( aQName ), 
    Unmarshaller::getInternalString( aDescription ),
    &lErrorObject
  );
}

} // namespace zorba
/* vim:set et sw=2 ts=2: */
