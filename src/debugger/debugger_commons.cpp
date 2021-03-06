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

#include "debugger_commons.h"

#include "system/globalenv.h"

#include "compiler/api/compilercb.h"
#include "context/static_context.h"
#include "context/dynamic_context.h"
#include "diagnostics/assert.h"

#include "runtime/debug/debug_iterator.h"
#include "runtime/visitors/planiter_visitor.h"

#include "store/api/item_factory.h"

#include "zorbatypes/URI.h"

#include "types/typeimpl.h"

#include "zorba/util/uri.h"

#include "debugger_runtime.h"

namespace zorba {

// ****************************************************************************

SERIALIZABLE_CLASS_VERSIONS(Breakable)

void
Breakable::serialize(serialization::Archiver& ar)
{
  ar & theLocation;
  ar & theEnabled;
}

// ****************************************************************************

bool
QueryLocComparator::operator()(const QueryLoc& a, const QueryLoc& b) const
{
  // check the file names
  // smaller has priority
  int c;
  if ((c = a.getFilename().compare(b.getFilename())) != 0) {
    return c < 0;
  }

  int ai, bi;

  // check line numbers
  // smaller has priority
  ai = a.getLineBegin();
  bi = b.getLineBegin();
  if (ai != bi) {
    return ai < bi;
  }

  // check column begin numbers
  // smaller has priority
  ai = a.getColumnBegin();
  bi = b.getColumnBegin();
  if (ai == 0 || bi == 0) {
    return false;
  }
  if (ai != bi) {
    return ai < bi;
  }

  // check line end numbers
  // larger has priority
  ai = a.getLineEnd();
  bi = b.getLineEnd();
  if (ai != bi) {
    return ai > bi;
  }

  // check column end numbers
  // larger has priority
  ai = a.getColumnEnd();
  bi = b.getColumnEnd();
  if (ai == 0 || bi == 0) {
    return false;
  }

  return a.getColumnEnd() > b.getColumnEnd();
}

// ****************************************************************************

SERIALIZABLE_CLASS_VERSIONS(DebuggerSingletonIterator)


DebuggerSingletonIterator::DebuggerSingletonIterator(
  static_context* sctx, QueryLoc loc, DebuggerCommons* lCommons)
  : NoaryBaseIterator<DebuggerSingletonIterator, PlanIteratorState>(sctx, loc),
    theCommons(lCommons)
{
}

void
DebuggerSingletonIterator::serialize(::zorba::serialization::Archiver& ar)
{
  serialize_baseclass(ar, (NoaryBaseIterator<DebuggerSingletonIterator,PlanIteratorState>*)this);
  ar & theCommons;
}

NOARY_ACCEPT(DebuggerSingletonIterator);

bool
DebuggerSingletonIterator::nextImpl(store::Item_t& result, PlanState& planState) const
{
  PlanIteratorState* state;
  DEFAULT_STACK_INIT ( PlanIteratorState, state, planState );
  result = theCommons->getEvalItem();
  STACK_PUSH ( result != NULL, state );
  STACK_END (state);
}


// ****************************************************************************

SERIALIZABLE_CLASS_VERSIONS(DebuggerCommons)


DebuggerCommons::DebuggerCommons(static_context* sctx)
  : theBreak(false),
    theCause(0),
    theBreakCondition(0),
    theExecEval(false),
    theStepping(false)
{
  theRuntime = NULL;
  theCurrentStaticContext = NULL;
  theCurrentDynamicContext = NULL;
  thePlanState = NULL;
}


DebuggerCommons::~DebuggerCommons()
{
}


void
DebuggerCommons::serialize(::zorba::serialization::Archiver& ar)
{
  ar & theBreakables;

  if (ar.is_serializing_out())
  {
    ar.set_is_temp_field(true);
    csize s = theBreakableIDs.size();
    ar & s;
    ar.set_is_temp_field(false);

    BreakableIdMap::iterator it = theBreakableIDs.begin();
    BreakableIdMap::iterator end = theBreakableIDs.end();

    for (; it != end; ++it)
    {
      QueryLoc loc = (*it).first;
      ar & loc;
      ar & (*it).second;
    }
  }
  else
  {
    ar.set_is_temp_field(true);
    csize s;
    ar & s;
    ar.set_is_temp_field(false);

    std::pair<QueryLoc, unsigned int> p;

    for (csize i = 0; i < s; ++i)
    {
      ar & p.first;
      ar & p.second;

      theBreakableIDs.insert(p);
    }
  }

  ar & theStackTrace;
  ar & theUriFileMappingMap;

  if (ar.is_serializing_out())
    theRuntime = NULL;

  ar & theCurrentStaticContext;

  if (ar.is_serializing_out())
    theCurrentDynamicContext = NULL;

  ar & theBreak;
  ar & theCause;
  ar & theIteratorStack;
  ar & theBreakCondition;

  if (ar.is_serializing_out())
    thePlanState = NULL;

  ar & theEvalItem;
  ar & theExecEval;
  ar & theStepping;
}


void
DebuggerCommons::setRuntime(DebuggerRuntime* aRuntime)
{
  theRuntime = aRuntime;
}

DebuggerRuntime*
DebuggerCommons::getRuntime()
{
  return theRuntime;
}

void
DebuggerCommons::setCurrentStaticContext(static_context* aStaticContext)
{
  theCurrentStaticContext = aStaticContext;
}

void
DebuggerCommons::setCurrentDynamicContext(dynamic_context* aDynamicContext)
{
 theCurrentDynamicContext = aDynamicContext;
}

dynamic_context*
DebuggerCommons::getCurrentDynamicContext() const
{
  return theCurrentDynamicContext;
}

static_context*
DebuggerCommons::getCurrentStaticContext() const
{
  return theCurrentStaticContext;
}

unsigned int
DebuggerCommons::addBreakpoint(String& aFileName, unsigned int aLine, bool aEnabled)
{
  QueryLoc lLocation;
  lLocation.setLineBegin(aLine);
  lLocation.setLineEnd(aLine);
  lLocation.setFilename(aFileName.c_str());

  // we make sure that the this location file name is aligned with the internal ones
  // it must have a valid URI and a scheme (file://, http://, or https://)
  adjustLocationFilePath(lLocation);

  BreakableIdMap::iterator lIter = theBreakableIDs.find(lLocation);
  unsigned int lId;

  if (lIter == theBreakableIDs.end()) {
    String lFileName = aFileName;
    // be prepared to throw an exception
    std::stringstream lSs;
    lSs << "The breakpoint could not be set at line " << aLine
      << " in file \"" << lFileName << "\"";

    // let us then try some search before we fail, be good to the user and help him
    // 1. first we check if he sent a file URI; in this case, sorry!
    if (lFileName.find("file://") == 0) {
      throw lSs.str();
    }

    // now we have to normalize if we hope to find something
    lFileName = URIHelper::encodeFileURI(lFileName).str();
    // remove the added file schema prefix
    // TODO: maybe there is a better way to do this encoding
    lFileName = lFileName.substr(8);

    // 2. secondly we hope he gave us part of a path of a file
    lIter = theBreakableIDs.begin();
    std::vector<std::pair<QueryLoc, int> > lFoundBreakables;
    zorba::String lFirstBreakablePath;
    while (lIter != theBreakableIDs.end()) {
      // for now, only valid if on the breakable is on the same line as requested
      // TODO: this could be improved if the user wants to add a breakpoint to a line
      //       INSIDE a breakable that spans over multiple lines
      if (lIter->second != theMainModuleBreakableId && lIter->first.getLineBegin() == aLine) {
        zorba::String lBreakablePath = lIter->first.getFilename().str();

        // dies the given string matche any part in the breakable file name?
        if (lBreakablePath.find(lFileName) != String::npos) {
          // we found the fist candidate path
          if (lFirstBreakablePath == "") {
            lFirstBreakablePath = lBreakablePath;
          }
          // but stop as soon as we are reaching a second different path (report ambiguity)
          else if (lFirstBreakablePath != lBreakablePath){
            lSs.str("");
            lSs << "The file name \"" << aFileName << "\" is ambiguous. "
              << "I already found two potential files to set a breakpoint in line " << aLine
              << ":" << std::endl << "  " << lFirstBreakablePath << std::endl << "  " << lBreakablePath;
            throw lSs.str();
          }

          // Yes! We found one!
          lFoundBreakables.push_back(std::pair<QueryLoc, int>(lIter->first, lIter->second));
        }
      }
      lIter++;
    }

    // what should I say, not a very successful search :(
    if (lFoundBreakables.size() == 0) {
      throw lSs.str();
    }

    // TODO: The best solution would be for the debugger to enable all the
    // matched breakables but the protocol can send back only one ID of the
    // breakpoint set.

    // so we have multiple breakables, get the first in line
    // TODO: this does not catch multiple breakables starting in the same line
    // so only one will be picked (depending how the translator generated them)
    unsigned int lMinCol = lFoundBreakables.at(0).first.getColumnBegin();
    lId = lFoundBreakables.at(0).second;
    for (std::size_t i = 1; i < lFoundBreakables.size(); i++) {
      if (lMinCol > lFoundBreakables.at(i).first.getColumnBegin()) {
        lId = lFoundBreakables.at(i).second;
      }
    }
  }
  else {
    lId = lIter->second;
  }

  // now we have a breakable, so set it accordingly
  theBreakables[lId].setSet(true);
  theBreakables[lId].setEnabled(aEnabled);
  return lId;
}

Breakable
DebuggerCommons::getBreakpoint(unsigned int aId)
{
  checkBreakpoint(aId);
  return theBreakables[aId];
}

BreakableVector
DebuggerCommons::getBreakpoints()
{
  return theBreakables;
}

void
DebuggerCommons::checkBreakpoint(unsigned int aId)
{
  if (aId >= theBreakables.size() || !theBreakables[aId].isSet()) {
    std::stringstream lSs;
    lSs << "No such breakpoint: " << aId;
    throw lSs.str();
  }
}

void
DebuggerCommons::updateBreakpoint(
  unsigned int aId,
  bool aEnabled)
{
  checkBreakpoint(aId);
  theBreakables[aId].setEnabled(aEnabled);
}

void
DebuggerCommons::updateBreakpoint(
  unsigned int aId,
  bool aEnabled,
  std::string aCondition,
  unsigned int aHitValue)
{
  checkBreakpoint(aId);
  theBreakables[aId].setEnabled(aEnabled);
  // TODO: set condition
}

void
DebuggerCommons::removeBreakpoint(unsigned int aId)
{
  checkBreakpoint(aId);
  theBreakables[aId].setSet(false);
}

bool
DebuggerCommons::canBreak()
{
  return !theExecEval;
}

bool
DebuggerCommons::mustBreak(SuspensionCause& aCause)
{
  if (theRuntime->getAndClearInterruptBreak()) {
    aCause = CAUSE_USER;
    return true;
  }
  if (theBreak) {
    aCause = theCause;
    return true;
  } else if (theStepping) {
    std::size_t lSize = theIteratorStack.size();

    // either we meet the step condition
    if (lSize <= theBreakCondition) {
      // reset the break conditions
      theBreakCondition = 0;
      theStepping = false;
      return true;
    }
    // or we have stepped over from a function declaration breakpoint
    // we have for sure at least the following stack of debug iterators:
    // MainModule DI -> FunctionCall DI -> FunctionDecl DI -> current DI
    if (lSize > 3 && lSize == theBreakCondition + 1 && theIteratorStack.at(lSize - 2)->getDebuggerParent() == NULL) {
      // reset the break conditions
      theBreakCondition = 0;
      theStepping = false;
      return true;
    }
    // or we have stepped over and we reached another variable declaration
    // TODO: still to check and stop only at declarations on the same level
    if (lSize <= theBreakCondition && theIteratorStack.back()->isVarDeclaration()) {
      // reset the break conditions
      theBreakCondition = 0;
      theStepping = false;
      return true;
    }
  }

  return false;
}

bool
DebuggerCommons::hasToBreakAt(QueryLoc aLocation)
{
  // we make sure that this location file name is a valid URI and has a scheme (file://, http://, or https://)
  adjustLocationFilePath(aLocation);

  BreakableIdMap::const_iterator lIter = theBreakableIDs.find(aLocation);
  if (lIter == theBreakableIDs.end()) {
    return false;
  }

  Breakable lBreakable = theBreakables[lIter->second];
  return lBreakable.isSet() && lBreakable.isEnabled();
}

bool
DebuggerCommons::hasToBreakAt(const DebugIterator* aIter)
{
  return false;
}

void
DebuggerCommons::setBreak(bool lBreak, SuspensionCause aCause)
{
#ifndef NDEBUG
  //Check preconditions
  ZORBA_ASSERT(lBreak ? aCause != 0 : true);
#endif // NDEBUG
  theBreak = lBreak;
  theCause = aCause;
#ifndef NDEBUG
  // Check post conditions
  ZORBA_ASSERT(theBreak == lBreak);
  ZORBA_ASSERT(theCause == aCause);
#endif //NDEBUG
}

void
DebuggerCommons::setCurrentIterator(const DebugIterator* aIterator)
{
  // don't modify the iterator stack during expression evaluation
  if (theExecEval) {
    return;
  }

  const DebugIterator* lParent = aIterator->getDebuggerParent();

  // when the stack is empty (main module debug iterator) or
  // when the parent is NULL but not a variable declaration (function declaration iterator) or
  // when the parent is the last in the stack
  if (theIteratorStack.empty() ||
      (lParent == NULL && !aIterator->isVarDeclaration()) ||
      theIteratorStack.back() == lParent) {
    theIteratorStack.push_back((DebugIterator*)aIterator);
    return;
  }

  // avoid multiple calls to this function
  // recursive functions always insert at least 2 iterators
  if (theIteratorStack.back() == aIterator) {
    return;
  }

  // now we remove the iterators from the stack until we find the parent
  theIteratorStack.pop_back();
  while (!theIteratorStack.empty()) {
    if (theIteratorStack.back() == aIterator->getDebuggerParent()) {
      theIteratorStack.push_back((DebugIterator*)aIterator);
      return;
    }
    theIteratorStack.pop_back();
  }

  // noe the stack is empty, so push this iterator in the stack
  theIteratorStack.push_back((DebugIterator*)aIterator);
}

const DebugIterator*
DebuggerCommons::getCurrentIterator() const
{
  if (theIteratorStack.empty()) {
    return NULL;
  }
  return theIteratorStack.back();
}

void
DebuggerCommons::makeStepOver()
{
  theStepping = true;
  if (!theIteratorStack.empty()) {
    theBreakCondition = theIteratorStack.size();
  } else {
    theBreakCondition = 0;
  }
}

void DebuggerCommons::makeStepOut()
{
  theStepping = true;
  if (!theIteratorStack.empty()) {
    theBreakCondition = theIteratorStack.size() - 1;
  } else {
    theBreakCondition = 0;
  }
}

void
DebuggerCommons::setPlanState(PlanState* aPlanState)
{
  thePlanState = aPlanState;
  //Check postconditions
  ZORBA_ASSERT(thePlanState == aPlanState);
}

std::list<std::pair<zstring, zstring> >
DebuggerCommons::eval(const zstring& aExpr, Zorba_SerializerOptions& aSerOpts)
{
  theExecEval = true;
  zstring lStore = aExpr;
  GlobalEnvironment::getInstance().getItemFactory()->createString(theEvalItem,
                                                                  lStore);
  std::list<std::pair<zstring, zstring> > lRes;
  try {
    lRes = theIteratorStack.back()->eval(thePlanState, &aSerOpts);
    theExecEval = false;
  } catch (...) {
    theExecEval = false;
    throw;
  }

  return lRes;
}

store::Item_t
DebuggerCommons::getEvalItem()
{
  return theEvalItem;
}

void
DebuggerCommons::addModuleUriMapping(std::string aUri, std::string aFileUri)
{
  ZORBA_ASSERT(theUriFileMappingMap.find(aUri) == theUriFileMappingMap.end());
  theUriFileMappingMap.insert(
    std::pair<std::string, std::string>(aUri, aFileUri));
  ZORBA_ASSERT(theUriFileMappingMap.find(aUri) != theUriFileMappingMap.end());
}

std::string
DebuggerCommons::getFilepathOfURI(const std::string& aUri) const
{
  std::map<std::string, std::string>::const_iterator lIter;
  lIter = theUriFileMappingMap.find(aUri);
  zstring lString;
  if (lIter == theUriFileMappingMap.end())
  {
    lString = aUri;
  }
  else
  {
    lString = lIter->second;
  }
  zstring lRes;
  URI::decode_file_URI(lString, lRes);
  return lRes.str();
}

void
DebuggerCommons::addBreakable(
  Breakable& aBreakable,
  bool aIsMainModuleBreakable)
{
  // we make sure that this breakable file name is a valid URI and has a scheme (file://, http://, or https://)
  adjustLocationFilePath(aBreakable.getLocation());

  unsigned int lId = theBreakables.size();
  if (aIsMainModuleBreakable) {
    theMainModuleBreakableId = lId;
  }
  theBreakables.push_back(aBreakable);
  theBreakableIDs[aBreakable.getLocation()] = lId;
}

void
DebuggerCommons::pushStackFrame(QueryLoc aLocation, std::string& aFunctionName)
{
  // we make sure that the stack frame locations always have valid URIs and a scheme (file://, http://, or https://)
  adjustLocationFilePath(aLocation);

  theStackTrace.push_back(std::pair<QueryLoc, std::string>(aLocation, aFunctionName));
}

void
DebuggerCommons::popStackFrame()
{
  theStackTrace.pop_back();
}

std::vector<std::pair<QueryLoc, std::string> >
DebuggerCommons::getStackFrames() const
{
  return theStackTrace;
}

void
DebuggerCommons::adjustLocationFilePath(QueryLoc& aLocation)
{
  zstring lOldFilename(aLocation.getFilename());
  zstring lPrefix = lOldFilename.substr(0, 7);

  if (lPrefix == "file://") {
#ifdef WIN32
    // decode and encode back to solve the driver column encoding: C:, D:, etc.
    const String lNewFilename = URIHelper::decodeFileURI(lOldFilename.str());
    const String lNewURI = URIHelper::encodeFileURI(lNewFilename);
    aLocation.setFilename(lNewURI.str());
#endif
    return;
  }

  // just encode and assume file for non-URI locations
  if (lPrefix != "http://" && lPrefix != "https:/") {
    const String lNewURI = URIHelper::encodeFileURI(lOldFilename.str());
    aLocation.setFilename(lNewURI.str());
    return;
  }
}

} // namespace zorba
