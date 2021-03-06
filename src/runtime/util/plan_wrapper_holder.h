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
#ifndef ZORBA_RUNTIME_UTIL_PLAN_WRAPPER_HOLDER_H
#define ZORBA_RUNTIME_UTIL_PLAN_WRAPPER_HOLDER_H

#include "runtime/api/plan_wrapper.h"
#include <zorba/internal/unique_ptr.h>

namespace zorba {

// RAII-style class
class PlanWrapperHolder 
{
  std::unique_ptr<PlanWrapper> pw;
  
public:
  PlanWrapperHolder (PlanWrapper *pw_)
    :
    pw(pw_)
  {
    pw->open();
  }

  PlanWrapperHolder() {}

  ~PlanWrapperHolder() 
  {
    if (pw.get() != NULL)
      pw->close();
  }
  
  void reset(PlanWrapper* pw_) 
  {
    if (pw.get() != NULL)
      pw->close();

    pw.reset(pw_);

    if (pw_ != NULL)
      pw->open();
  }

  PlanWrapper* get() { return pw.get(); }

  PlanWrapper* operator->() { return pw.get(); }
};

}

#endif
/*
 * Local variables:
 * mode: c++
 * End:
 */
/* vim:set et sw=2 ts=2: */
