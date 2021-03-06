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
#ifndef ZORBA_RUNTIME_UTIL_TIMEOUT_H
#define ZORBA_RUNTIME_UTIL_TIMEOUT_H

#include "zorbautils/runnable.h"

namespace zorba 
{

class PlanState;


class StateWrapper 
{
private:
  PlanState& theState;

public:
  StateWrapper(PlanState& aState);

  void doBreak();
};


class Timeout : public Runnable 
{
private:
  unsigned long  theTimeout;
  StateWrapper   theWrapper;

public:
  Timeout(unsigned long aTimeout, const StateWrapper& aWrapper);

  virtual void run();
  // Note: this method is not allowd to throw an exception!
  virtual void finish();
};


} //namespace zorba

#endif //TIMEOUT_H
/* vim:set et sw=2 ts=2: */
