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
#include <sstream>

#include "common/common.h"

#include "zorbautils/strutil.h"

#include "runtime/visitors/printervisitor.h"
#include "runtime/context/ContextImpl.h"
#include "runtime/core/item_iterator.h"
#include "runtime/core/var_iterators.h"
#include "runtime/core/constructors.h"
#include "runtime/core/path_iterators.h"
//#include "runtime/core/path.h"
#include "runtime/core/fncall_iterator.h"
#include "runtime/qnames/QNamesImpl.h"
#include "runtime/core/sequencetypes.h"
#include "runtime/accessors/AccessorsImpl.h"
#include "runtime/booleans/BooleanImpl.h"
#include "runtime/validate/validate.h"
#include "runtime/core/gflwor/let_iterator.h"
#include "runtime/core/gflwor/for_iterator.h"
#include "runtime/core/gflwor/outerfor_iterator.h"
#include "runtime/core/gflwor/count_iterator.h"
#include "runtime/core/gflwor/where_iterator.h"
#include "runtime/core/gflwor/tuplesource_iterator.h"
#include "runtime/core/gflwor/tuplestream_iterator.h"
#include "runtime/core/gflwor/groupby_iterator.h"
#include "runtime/core/gflwor/window_iterator.h"
#include "runtime/core/flwor_iterator.h"
#include "runtime/core/tuple_iterators.h"
#include "runtime/core/trycatch.h"
#include "runtime/core/nodeid_iterators.h"
#include "runtime/core/internal_operators.h"
#include "runtime/indexing/value_index_builder.h"
#include "runtime/indexing/value_index_probe.h"
#include "runtime/numerics/NumericsImpl.h"
#include "runtime/core/arithmetic_impl.h"
#include "runtime/sequences/SequencesImpl.h"
#include "runtime/strings/StringsImpl.h"
#include "runtime/dateTime/DurationsDatesTimes.h"
#include "runtime/eval/FnContextImpl.h"
#include "runtime/debug/debug_iterators.h"
#include "runtime/debug/zorba_debug_iterator.h"
#include "debugger/zorba_debugger_commons.h"
#include "runtime/collections/CollectionsImpl.h"
#include "runtime/util/UtilImpl.h"
#include "runtime/convertors/ConvertorsImpl.h"
#include "context/static_context.h"
#include "context/namespace_context.h"

#ifdef ZORBA_WITH_REST
#include "runtime/rest/rest.h"
#endif
#ifdef ZORBA_WITH_FOP
#include "runtime/fop/FopImpl.h"
#endif // ZORBA_WITH_FOP
#ifdef ZORBA_WITH_EMAIL
#include "runtime/email/EmailImpl.h"
#endif

#include "runtime/visitors/iterprinter.h"
#include "runtime/update/update.h"
#include "runtime/misc/MiscImpl.h"
#include "runtime/nodes/NodesImpl.h"

#include "store/api/iterator.h"

#include "util/properties.h"

#include "types/root_typemanager.h"
#include "types/typeops.h"

#include "context/static_context.h"

#include "compiler/api/compilercb.h"


namespace zorba {

#define PRINTER_VISITOR_DEFINITION(class)                \
  void beginVisit ( const class& a )  {                  \
    thePrinter.startBeginVisit(#class, ++theId);         \
    printCommons(  &a, theId );                          \
    thePrinter.endBeginVisit( theId);                    \
  }                                                      \
  void endVisit ( const class& ) {                       \
    thePrinter.startEndVisit();                          \
    thePrinter.endEndVisit();                            \
  }


#define PRINTER_VISITOR_AXIS_DEFINITION(class)           \
  void beginVisit ( const class& a )  {                  \
    thePrinter.startBeginVisit(#class, ++theId);         \
    printCommons(&a, theId);                             \
    printNameOrKindTest(&a);                             \
    thePrinter.endBeginVisit( theId);                    \
  }                                                      \
  void endVisit ( const class& ) {                       \
    thePrinter.startEndVisit();                          \
    thePrinter.endEndVisit();                            \
  }



class PrinterVisitor : public PlanIterVisitor
{
private:
  IterPrinter  & thePrinter;
  PlanIterator * theIterator;
  int            theId;

public:

PrinterVisitor(IterPrinter& aPrinter, PlanIterator* aIter)
  :
  thePrinter(aPrinter),
  theIterator(aIter),
  theId(0)
{
}
  

void print() 
{
  thePrinter.start();
  theIterator->accept(*this);
  thePrinter.stop();
}


void printCommons(const PlanIterator* aIter, int theId) 
{
  if (! Properties::instance()->noTreeIds()) {
#if 0
      {
      std::stringstream lStream;
      lStream << aIter->loc;
      thePrinter.addAttribute("loc", lStream.str());
      }
#endif
    {
      std::stringstream lStream;
      if (Properties::instance()->stableIteratorIds())
        lStream << theId;
      else
        lStream << aIter;
      thePrinter.addAttribute("id", lStream.str());
    }
  }
}


void printNameOrKindTest(const AxisIteratorHelper* a) 
{
  thePrinter.addAttribute("test kind", toString(a->getTestKind()));
    
  if (a->getDocTestKind() != match_no_test)
    thePrinter.addAttribute("doc_test_kind", toString(a->getDocTestKind()));

  if (a->getQName() != 0)
    thePrinter.addAttribute("qname", a->getQName()->show());
  else
    thePrinter.addAttribute("qname","*");

  if (a->getType() != 0)
    thePrinter.addAttribute("typename", a->getType()->toString());
  else
    thePrinter.addAttribute("typename","*");

  {
    std::stringstream lStream;
    lStream << a->nilledAllowed();
    thePrinter.addAttribute("nill allowed", lStream.str());
  }

  if (a->getTargetPos() >= 0)
    thePrinter.addAttribute("target_position", to_string(a->getTargetPos()));
}
  

  PRINTER_VISITOR_DEFINITION(NodeReferenceIterator)

  PRINTER_VISITOR_DEFINITION(NodeByReferenceIterator)

  PRINTER_VISITOR_DEFINITION (FnDataIterator)

  PRINTER_VISITOR_DEFINITION (SequentialIterator)

  PRINTER_VISITOR_DEFINITION (FlowCtlIterator)

  PRINTER_VISITOR_DEFINITION (LoopIterator)

  PRINTER_VISITOR_DEFINITION (FnRootIterator)

  PRINTER_VISITOR_DEFINITION (FnNodeNameIterator)

  PRINTER_VISITOR_DEFINITION (FnNilledIterator)

  PRINTER_VISITOR_DEFINITION (FnBaseUriIterator)

  PRINTER_VISITOR_DEFINITION (FnDocumentUriIterator)

  PRINTER_VISITOR_DEFINITION (FnErrorIterator)

  PRINTER_VISITOR_DEFINITION (FnResolveUriIterator)

  PRINTER_VISITOR_DEFINITION (UDFunctionCallIterator)

  PRINTER_VISITOR_DEFINITION (StatelessExtFunctionCallIterator)

  PRINTER_VISITOR_DEFINITION (FnStringIterator)

  PRINTER_VISITOR_DEFINITION (FnLocalNameIterator)

  PRINTER_VISITOR_DEFINITION (FnNamespaceUriIterator)

  PRINTER_VISITOR_DEFINITION (FnLangIterator)

  PRINTER_VISITOR_DEFINITION (FnCollectionIterator)

  PRINTER_VISITOR_DEFINITION (FnBooleanIterator)

#ifndef ZORBA_NO_XMLSCHEMA
  PRINTER_VISITOR_DEFINITION (ValidateIterator)
#endif

  PRINTER_VISITOR_DEFINITION (LogicIterator)

  PRINTER_VISITOR_DEFINITION (CompareIterator)

  PRINTER_VISITOR_DEFINITION (ElementIterator)

  PRINTER_VISITOR_DEFINITION (AttributeIterator)

  PRINTER_VISITOR_DEFINITION (DocumentIterator)

  PRINTER_VISITOR_DEFINITION (DocumentContentIterator)

  PRINTER_VISITOR_DEFINITION (CommentIterator)

  PRINTER_VISITOR_DEFINITION (PiIterator)

  void beginVisit ( const SingletonIterator& a) {
    thePrinter.startBeginVisit("SingletonIterator", ++theId);
    thePrinter.addAttribute("value", a.getValue()->show());
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void endVisit ( const SingletonIterator& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

  PRINTER_VISITOR_DEFINITION (EmptyIterator)

  void beginVisit ( const EnclosedIterator& a ) {
    thePrinter.startBeginVisit("EnclosedIterator", ++theId);
    thePrinter.addAttribute("attr_cont", (a.getAttrContent() ? "true" : "false"));
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void endVisit ( const EnclosedIterator& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

  PRINTER_VISITOR_DEFINITION (IfThenElseIterator)

  PRINTER_VISITOR_DEFINITION (NodeDistinctIterator)


void beginVisit ( const NodeSortIterator& a )  
{
  thePrinter.startBeginVisit("NodeSortIterator", ++theId); 
  printCommons(&a, theId);  
  thePrinter.addAttribute("distinct", (a.getDistinct() ? "true" : "false"));
  thePrinter.addAttribute("ascending", (a.getAscending() ? "true" : "false"));
  thePrinter.endBeginVisit( theId);            
}                  
                            
void endVisit ( const NodeSortIterator& ) 
{               
  thePrinter.startEndVisit();                  
  thePrinter.endEndVisit();                    
}


#if 0
void beginVisit ( const PathIterator& a ) 
{
  thePrinter.startBeginVisit("PathIterator", ++theId);
  printCommons( &a, theId );
  thePrinter.endBeginVisit(theId);
}

void endVisit(const PathIterator& /*a*/) 
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}
#endif

void beginVisit ( const SelfAxisIterator& a ) 
{
  thePrinter.startBeginVisit("SelfAxisIteratorator", ++theId);
  printCommons( &a, theId );
  printNameOrKindTest(&a);
  thePrinter.endBeginVisit(theId);
}

void endVisit(const SelfAxisIterator& /*a*/) 
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


PRINTER_VISITOR_AXIS_DEFINITION (AttributeAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (ParentAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (AncestorAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (AncestorReverseAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (AncestorSelfAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (AncestorSelfReverseAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (RSiblingAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (LSiblingAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (LSiblingReverseAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (ChildAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (DescendantAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (DescendantSelfAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (PrecedingAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (PrecedingReverseAxisIterator)

  PRINTER_VISITOR_AXIS_DEFINITION (FollowingAxisIterator)

  PRINTER_VISITOR_DEFINITION (InstanceOfIterator)

  void beginVisit ( const TreatIterator& a ) {
    thePrinter.startBeginVisit("TreatIterator", ++theId);
    if (a.check_prime)
      thePrinter.addAttribute("type", TypeOps::toString(*a.theTreatType));
    thePrinter.addAttribute("quant", TypeOps::decode_quantifier (a.theQuantifier));
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void endVisit ( const TreatIterator& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

  PRINTER_VISITOR_DEFINITION (EitherNodesOrAtomicsIterator)

  void beginVisit ( const NumArithIterator<AddOperation>& a ) {
    thePrinter.startBeginVisit("NumArithIterator_AddOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const NumArithIterator<SubtractOperation>& a ) {
    thePrinter.startBeginVisit("NumArithIterator_SubtractOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const NumArithIterator<MultiplyOperation>& a ) {
    thePrinter.startBeginVisit("NumArithIterator_MultiplyOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const NumArithIterator<DivideOperation>& a ) {
    thePrinter.startBeginVisit("NumArithIterator_DivideOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const NumArithIterator<IntegerDivideOperation>& a ) {
    thePrinter.startBeginVisit("NumArithIterator_IntegerDivideOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const NumArithIterator<ModOperation>& a ) {
    thePrinter.startBeginVisit("NumArithIterator_ModOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void endVisit ( const NumArithIterator<AddOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const NumArithIterator<SubtractOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const NumArithIterator<MultiplyOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const NumArithIterator<DivideOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const NumArithIterator<IntegerDivideOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const NumArithIterator<ModOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

  void beginVisit ( const GenericArithIterator<AddOperation>& a ) {
    thePrinter.startBeginVisit("GenericArithIterator_AddOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const GenericArithIterator<SubtractOperation>& a ) {
    thePrinter.startBeginVisit("GenericArithIterator_SubtractOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const GenericArithIterator<MultiplyOperation>& a ) {
    thePrinter.startBeginVisit("GenericArithIterator_MultiplyOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const GenericArithIterator<DivideOperation>& a ) {
    thePrinter.startBeginVisit("GenericArithIterator_DivideOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const GenericArithIterator<IntegerDivideOperation>& a ) {
    thePrinter.startBeginVisit("GenericArithIterator_IntegerDivideOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const GenericArithIterator<ModOperation>& a ) {
    thePrinter.startBeginVisit("GenericArithIterator_ModOperation", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void endVisit ( const GenericArithIterator<AddOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const GenericArithIterator<SubtractOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const GenericArithIterator<MultiplyOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const GenericArithIterator<DivideOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const GenericArithIterator<IntegerDivideOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const GenericArithIterator<ModOperation>& /*a*/) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void beginVisit ( const SpecificNumArithIterator<AddOperation, TypeConstants::XS_DECIMAL>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_AddOperation_DECIMAL", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<AddOperation, TypeConstants::XS_INTEGER>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_AddOperation_INTEGER", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<AddOperation, TypeConstants::XS_FLOAT>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_AddOperation_FLOAT", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<AddOperation, TypeConstants::XS_DOUBLE>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_AddOperation_DOUBLE", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<SubtractOperation, TypeConstants::XS_DECIMAL>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_SubtractOperation_DECIMAL", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<SubtractOperation, TypeConstants::XS_INTEGER>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_SubtractOperation_INTEGER", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<SubtractOperation, TypeConstants::XS_FLOAT>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_SubtractOperation_FLOAT", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<SubtractOperation, TypeConstants::XS_DOUBLE>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_SubtractOperation_DOUBLE", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<MultiplyOperation, TypeConstants::XS_DECIMAL>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_MultiplyOperation_DECIMAL", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<MultiplyOperation, TypeConstants::XS_INTEGER>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_MultiplyOperation_INTEGER", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<MultiplyOperation, TypeConstants::XS_FLOAT>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_MultiplyOperation_FLOAT", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<MultiplyOperation, TypeConstants::XS_DOUBLE>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_MultiplyOperation_DOUBLE", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<DivideOperation, TypeConstants::XS_DECIMAL>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_DivideOperation_DECIMAL", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<DivideOperation, TypeConstants::XS_INTEGER>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_DivideOperation_INTEGER", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<DivideOperation, TypeConstants::XS_FLOAT>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_DivideOperation_FLOAT", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<DivideOperation, TypeConstants::XS_DOUBLE>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_DivideOperation_DOUBLE", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<ModOperation, TypeConstants::XS_DECIMAL>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_ModOperation_DECIMAL", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<ModOperation, TypeConstants::XS_INTEGER>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_ModOperation_INTEGER", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<ModOperation, TypeConstants::XS_FLOAT>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_ModOperation_FLOAT", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void beginVisit ( const SpecificNumArithIterator<ModOperation, TypeConstants::XS_DOUBLE>& a)
  {
    thePrinter.startBeginVisit("SpecificNumArithIterator_ModOperation_DOUBLE", ++theId);
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }
  void endVisit ( const SpecificNumArithIterator<AddOperation, TypeConstants::XS_DECIMAL>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<AddOperation, TypeConstants::XS_INTEGER>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<AddOperation, TypeConstants::XS_FLOAT>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<AddOperation, TypeConstants::XS_DOUBLE>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<SubtractOperation, TypeConstants::XS_DECIMAL>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<SubtractOperation, TypeConstants::XS_INTEGER>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<SubtractOperation, TypeConstants::XS_FLOAT>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<SubtractOperation, TypeConstants::XS_DOUBLE>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<MultiplyOperation, TypeConstants::XS_DECIMAL>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<MultiplyOperation, TypeConstants::XS_INTEGER>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<MultiplyOperation, TypeConstants::XS_FLOAT>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<MultiplyOperation, TypeConstants::XS_DOUBLE>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<DivideOperation, TypeConstants::XS_DECIMAL>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<DivideOperation, TypeConstants::XS_INTEGER>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<DivideOperation, TypeConstants::XS_FLOAT>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<DivideOperation, TypeConstants::XS_DOUBLE>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<ModOperation, TypeConstants::XS_DECIMAL>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<ModOperation, TypeConstants::XS_INTEGER>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<ModOperation, TypeConstants::XS_FLOAT>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }
  void endVisit ( const SpecificNumArithIterator<ModOperation, TypeConstants::XS_DOUBLE>& )
  {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

#define TYPED_VAL_CMP( xqt )                                            \
  void beginVisit ( const TypedValueCompareIterator<TypeConstants::XS_##xqt>& a) { \
    thePrinter.startBeginVisit("TypedValueCompareIterator_" #xqt, ++theId); \
    printCommons( &a, theId );                                          \
    thePrinter.endBeginVisit(theId);                                    \
  }                                                                     \
                                                                        \
  void endVisit ( const TypedValueCompareIterator<TypeConstants::XS_##xqt>& a) { \
    thePrinter.startEndVisit();                                         \
    thePrinter.endEndVisit();                                           \
  }

TYPED_VAL_CMP (DECIMAL)
TYPED_VAL_CMP (INTEGER)
TYPED_VAL_CMP (DOUBLE)
TYPED_VAL_CMP (FLOAT)
TYPED_VAL_CMP (STRING)

#undef TYPED_VAL_CMP

  PRINTER_VISITOR_DEFINITION (OpNumericUnaryIterator)

  PRINTER_VISITOR_DEFINITION (FnAbsIterator)

  PRINTER_VISITOR_DEFINITION (FnCeilingIterator)

  PRINTER_VISITOR_DEFINITION (FnFloorIterator)

  PRINTER_VISITOR_DEFINITION (FnRoundIterator)

  PRINTER_VISITOR_DEFINITION (FnSQRTIterator)

  PRINTER_VISITOR_DEFINITION (FnExpIterator)
  PRINTER_VISITOR_DEFINITION (FnLogIterator)
  PRINTER_VISITOR_DEFINITION (FnSinIterator)
  PRINTER_VISITOR_DEFINITION (FnCosIterator)
  PRINTER_VISITOR_DEFINITION (FnTanIterator)
  PRINTER_VISITOR_DEFINITION (FnArcSinIterator)
  PRINTER_VISITOR_DEFINITION (FnArcCosIterator)
  PRINTER_VISITOR_DEFINITION (FnArcTanIterator)

  PRINTER_VISITOR_DEFINITION (FnRoundHalfToEvenIterator)

  PRINTER_VISITOR_DEFINITION (FnConcatIterator)

  PRINTER_VISITOR_DEFINITION (FnEmptyIterator)

  PRINTER_VISITOR_DEFINITION (FnExistsIterator)

  PRINTER_VISITOR_DEFINITION (FnIndexOfIterator)

  PRINTER_VISITOR_DEFINITION (FnRemoveIterator)

  PRINTER_VISITOR_DEFINITION (FnReverseIterator)

  PRINTER_VISITOR_DEFINITION (OpToIterator)

  PRINTER_VISITOR_DEFINITION (FnIdIterator)

  PRINTER_VISITOR_DEFINITION (FnIdRefIterator)

  PRINTER_VISITOR_DEFINITION (FnDistinctValuesIterator)

  PRINTER_VISITOR_DEFINITION (FnInsertBeforeIterator)

  PRINTER_VISITOR_DEFINITION (FnSubsequenceIterator)

  PRINTER_VISITOR_DEFINITION (FnZeroOrOneIterator)

  PRINTER_VISITOR_DEFINITION (FnOneOrMoreIterator)

  PRINTER_VISITOR_DEFINITION (FnExactlyOneIterator)

  PRINTER_VISITOR_DEFINITION (FnCountIterator)

  PRINTER_VISITOR_DEFINITION (FnAvgIterator)

  void beginVisit ( const FnMinMaxIterator& a){
    thePrinter.startBeginVisit("FnMinMaxIterator", ++theId);
    thePrinter.addAttribute("type",
        ((a.getType() == FnMinMaxIterator::MIN) != 0 ? std::string("min") : std::string("max")));
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }

  void endVisit ( const FnMinMaxIterator& ){
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

  PRINTER_VISITOR_DEFINITION (FnSumIterator)

  PRINTER_VISITOR_DEFINITION (FnSumDoubleIterator)

  PRINTER_VISITOR_DEFINITION (FnSumFloatIterator)

  PRINTER_VISITOR_DEFINITION (FnSumDecimalIterator)

  PRINTER_VISITOR_DEFINITION (FnSumIntegerIterator)

  PRINTER_VISITOR_DEFINITION (FnDocIterator)

  PRINTER_VISITOR_DEFINITION (FnDocAvailableIterator)

  PRINTER_VISITOR_DEFINITION (FnParseIterator)

  PRINTER_VISITOR_DEFINITION (HashSemiJoinIterator)

  PRINTER_VISITOR_DEFINITION (SortSemiJoinIterator)

  PRINTER_VISITOR_DEFINITION (FnTraceIterator)

  PRINTER_VISITOR_DEFINITION (FnReadStringIterator)

  PRINTER_VISITOR_DEFINITION (FnPrintIterator)

  PRINTER_VISITOR_DEFINITION (ResolveQNameIterator)

  PRINTER_VISITOR_DEFINITION (QNameIterator)

  PRINTER_VISITOR_DEFINITION (QNameEqualIterator)

  PRINTER_VISITOR_DEFINITION (PrefixFromQNameIterator)

  PRINTER_VISITOR_DEFINITION (LocalNameFromQNameIterator)

  PRINTER_VISITOR_DEFINITION (NamespaceUriFromQNameIterator)

  PRINTER_VISITOR_DEFINITION (NamespaceUriForPrefixIterator)

  PRINTER_VISITOR_DEFINITION (InScopePrefixesIterator)

  PRINTER_VISITOR_DEFINITION (CodepointsToStringIterator)

  PRINTER_VISITOR_DEFINITION (StringToCodepointsIterator)

  PRINTER_VISITOR_DEFINITION (CompareStrIterator)

  PRINTER_VISITOR_DEFINITION (CodepointEqualIterator)

  PRINTER_VISITOR_DEFINITION (ConcatStrIterator)

  PRINTER_VISITOR_DEFINITION (StringJoinIterator)

  PRINTER_VISITOR_DEFINITION (SubstringIterator)

  PRINTER_VISITOR_DEFINITION (StringLengthIterator)

  PRINTER_VISITOR_DEFINITION (NormalizeSpaceIterator)

  PRINTER_VISITOR_DEFINITION (NormalizeUnicodeIterator)

  PRINTER_VISITOR_DEFINITION (UpperCaseIterator)

  PRINTER_VISITOR_DEFINITION (LowerCaseIterator)

  PRINTER_VISITOR_DEFINITION (TranslateIterator)

  PRINTER_VISITOR_DEFINITION (EncodeForUriIterator)

  PRINTER_VISITOR_DEFINITION (IriToUriIterator)

  PRINTER_VISITOR_DEFINITION (EscapeHtmlUriIterator)

  PRINTER_VISITOR_DEFINITION (ContainsIterator)

  PRINTER_VISITOR_DEFINITION (StartsWithIterator)

  PRINTER_VISITOR_DEFINITION (EndsWithIterator)

  PRINTER_VISITOR_DEFINITION (SubstringBeforeIterator)

  PRINTER_VISITOR_DEFINITION (SubstringAfterIterator)

  PRINTER_VISITOR_DEFINITION (FnMatchesIterator)

  PRINTER_VISITOR_DEFINITION (FnReplaceIterator)

  PRINTER_VISITOR_DEFINITION (FnTokenizeIterator)

  PRINTER_VISITOR_DEFINITION (ZorNumGen)

  PRINTER_VISITOR_DEFINITION (TextIterator)

  PRINTER_VISITOR_DEFINITION (FnDateTimeConstructorIterator)

  PRINTER_VISITOR_DEFINITION (FnYearsFromDurationIterator)

  PRINTER_VISITOR_DEFINITION (FnMonthsFromDurationIterator)

  PRINTER_VISITOR_DEFINITION (FnDaysFromDurationIterator)

  PRINTER_VISITOR_DEFINITION (FnHoursFromDurationIterator)

  PRINTER_VISITOR_DEFINITION (FnMinutesFromDurationIterator)

  PRINTER_VISITOR_DEFINITION (FnSecondsFromDurationIterator)

  PRINTER_VISITOR_DEFINITION (FnYearFromDatetimeIterator)

  PRINTER_VISITOR_DEFINITION (FnMonthFromDatetimeIterator)

  PRINTER_VISITOR_DEFINITION (FnDayFromDatetimeIterator)

  PRINTER_VISITOR_DEFINITION (FnHoursFromDatetimeIterator)

  PRINTER_VISITOR_DEFINITION (FnMinutesFromDatetimeIterator)

  PRINTER_VISITOR_DEFINITION (FnSecondsFromDatetimeIterator)

  PRINTER_VISITOR_DEFINITION (FnTimezoneFromDatetimeIterator)

  PRINTER_VISITOR_DEFINITION (FnYearFromDateIterator)

  PRINTER_VISITOR_DEFINITION (FnMonthFromDateIterator)

  PRINTER_VISITOR_DEFINITION (FnDayFromDateIterator)

  PRINTER_VISITOR_DEFINITION (FnTimezoneFromDateIterator)

  PRINTER_VISITOR_DEFINITION (FnHoursFromTimeIterator)

  PRINTER_VISITOR_DEFINITION (FnMinutesFromTimeIterator)

  PRINTER_VISITOR_DEFINITION (FnSecondsFromTimeIterator)

  PRINTER_VISITOR_DEFINITION (FnTimezoneFromTimeIterator)

  PRINTER_VISITOR_DEFINITION (CreateTupleIterator)

  PRINTER_VISITOR_DEFINITION (GetTupleFieldIterator)
      
 PRINTER_VISITOR_DEFINITION (TryCatchIterator)


void beginVisit(const ForVarIterator& a) 
{
  thePrinter.startBeginVisit("ForVarIterator", ++theId);
  thePrinter.addAttribute("varname", a.getVarName()->getStringValue()->c_str());
  printCommons( &a, theId );
  thePrinter.endBeginVisit(theId);
}

void endVisit(const ForVarIterator& /*a*/) 
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisit(const LetVarIterator& a) 
{
  thePrinter.startBeginVisit("LetVarIterator", ++theId);
  thePrinter.addAttribute("varname", a.getVarName()->getStringValue()->c_str());
  printCommons( &a, theId );
  thePrinter.endBeginVisit(theId);
}

void endVisit(const LetVarIterator& /*a*/) 
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


PRINTER_VISITOR_DEFINITION (flwor::FLWORIterator)
     
PRINTER_VISITOR_DEFINITION (flwor::TupleStreamIterator)
      
PRINTER_VISITOR_DEFINITION (flwor::TupleSourceIterator)
      
PRINTER_VISITOR_DEFINITION (flwor::ForIterator)
      
PRINTER_VISITOR_DEFINITION (flwor::OuterForIterator)

PRINTER_VISITOR_DEFINITION (flwor::LetIterator)
      
PRINTER_VISITOR_DEFINITION (flwor::WhereIterator)
      
PRINTER_VISITOR_DEFINITION (flwor::CountIterator)
      
PRINTER_VISITOR_DEFINITION (flwor::GroupByIterator)

PRINTER_VISITOR_DEFINITION (flwor::OrderByIterator)

PRINTER_VISITOR_DEFINITION (flwor::WindowIterator)


void beginVisitFlworWhereClause(const PlanIterator& a)
{
  thePrinter.startBeginVisit("WhereClause", ++theId);
  thePrinter.endBeginVisit(theId);
  a.accept(*this);
}

void endVisitFlworWhereClause(const PlanIterator& /*a*/)
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitFlworLetVariable(
    bool materialize,
    const xqpStringStore_t varName,
    const std::vector<LetVarIter_t>& varRefs)
{
  thePrinter.startBeginVisit("LetVariable", ++theId);

  thePrinter.addAttribute("name", varName->str());

  thePrinter.addAttribute("materialize", materialize ? "true" : "false");

  std::ostringstream str;

  ulong numRefs = varRefs.size();
  for (ulong i = 0; i < numRefs; i++)
  {
    str << varRefs[i].getp();
    if (i < numRefs-1)
      str << " ";
  }

  if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("referenced-by", str.str());

  thePrinter.endBeginVisit(theId);
}

void endVisitFlworLetVariable()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitFlworForVariable(
    const xqpStringStore_t varName,
    const std::vector<ForVarIter_t>& varRefs,
    const std::vector<ForVarIter_t>& posRefs)
{
  thePrinter.startBeginVisit("ForVariable", ++theId);

  thePrinter.addAttribute("name", varName->str());

  std::ostringstream str;

  ulong numRefs = varRefs.size();
  for (ulong i = 0; i < numRefs; i++)
  {
    str << varRefs[i].getp();
    if (i < numRefs-1)
      str << " ";
  }

  if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("referenced-by", str.str());

  if (!posRefs.empty())
  {
    std::ostringstream str;

    ulong numRefs = posRefs.size();
    for (ulong i = 0; i < numRefs; i++)
    {
      str << posRefs[i].getp();
      if (i < numRefs-1)
        str << " ";
    }
    
    if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("pos-referenced-by", str.str());
  }


  thePrinter.endBeginVisit(theId);
}


void endVisitFlworForVariable()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitGroupByClause()
{
  thePrinter.startBeginVisit("GroupByClause", ++theId);
  thePrinter.endBeginVisit(theId);
}
  

void endVisitGroupByClause()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitGroupBySpec()
{
  thePrinter.startBeginVisit("Spec", theId);
  thePrinter.endBeginVisit(theId);
}
  

void endVisitGroupBySpec()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}

void beginVisitGroupByOuter()
{
  thePrinter.startBeginVisit("Spec", theId);
  thePrinter.endBeginVisit(theId);
}
  

void endVisitGroupByOuter()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}

void beginVisitGroupVariable(const std::vector<ForVarIter_t>& varRefs)
{
  thePrinter.startBeginVisit("GroupVariable", ++theId);

  std::ostringstream str;

  ulong numRefs = varRefs.size();
  for (ulong i = 0; i < numRefs; i++)
  {
    str << varRefs[i].getp();
    if (i < numRefs-1)
      str << " ";
  }

  if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("referenced-by", str.str());

  thePrinter.endBeginVisit(theId);
}


void endVisitGroupVariable()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitNonGroupVariable(
      const std::vector<LetVarIter_t>& varRefs)
{
  thePrinter.startBeginVisit("NonGroupVariable", ++theId);

  std::ostringstream str;

  ulong numRefs = varRefs.size();
  for (ulong i = 0; i < numRefs; i++)
  {
    str << varRefs[i].getp();
    if (i < numRefs-1)
      str << " ";
  }

  if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("referenced-by", str.str());

  thePrinter.endBeginVisit(theId);
}


void endVisitNonGroupVariable()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}
  

void beginVisitOrderBy(const PlanIterator& a)
{
  thePrinter.startBeginVisit("OrderByClause", ++theId);
  thePrinter.endBeginVisit(theId);
  a.accept(*this);
}


void endVisitOrderBy(const PlanIterator& /*a*/)
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitOrderByForVariable(
    ForVarIter_t inputVar,
    const std::vector<ForVarIter_t>& varRefs)
{
  thePrinter.startBeginVisit("OrderByForVariable", theId);

  std::ostringstream str1;
  std::ostringstream str2;

  str1 << inputVar->getVarName()->getStringValue()->str() << " : " << inputVar.getp();

  ulong numRefs = varRefs.size();
  for (ulong i = 0; i < numRefs; i++)
  {
    str2 << varRefs[i].getp();
    if (i < numRefs-1)
      str2 << " ";
  }

  thePrinter.addAttribute("inputVar : ", str1.str());
  if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("referenced-by", str2.str());

  thePrinter.endBeginVisit(theId);
}


void endVisitOrderByForVariable()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitOrderByLetVariable(
    LetVarIter_t inputVar,
    const std::vector<LetVarIter_t>& varRefs)
{
  thePrinter.startBeginVisit("OrderByLetVariable", theId);

  std::ostringstream str1;
  std::ostringstream str2;

  str1 << inputVar->getVarName()->getStringValue()->str() << " : " << inputVar.getp();

  ulong numRefs = varRefs.size();
  for (ulong i = 0; i < numRefs; i++)
  {
    str2 << varRefs[i].getp();
    if (i < numRefs-1)
      str2 << " ";
  }

  thePrinter.addAttribute("inputVar : ", str1.str());
  if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("referenced-by", str2.str());

  thePrinter.endBeginVisit(theId);
}


void endVisitOrderByLetVariable()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitWindowVariable(
    const std::string& varName,
    const std::vector<LetVarIter_t>& varRefs)
{
  thePrinter.startBeginVisit("WindowVariable", theId);

  thePrinter.addAttribute("name", varName);

  std::ostringstream str;

  ulong numRefs = varRefs.size();
  for (ulong i = 0; i < numRefs; i++)
  {
    str << varRefs[i].getp();
    if (i < numRefs-1)
      str << " ";
  }

  if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("referenced-by", str.str());

  thePrinter.endBeginVisit(theId);
}

void endVisitWindowVariable()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitWinCondVariable(
    const std::string& varName,
    const std::vector<ForVarIter_t>& varRefs)
{
  thePrinter.startBeginVisit("WinCondVariable", theId);

  thePrinter.addAttribute("name", varName);

  std::ostringstream str;

  ulong numRefs = varRefs.size();
  for (ulong i = 0; i < numRefs; i++)
  {
    str << varRefs[i].getp();
    if (i < numRefs-1)
      str << " ";
  }

  if (! Properties::instance()->noTreeIds()) thePrinter.addAttribute("referenced-by", str.str());

  thePrinter.endBeginVisit(theId);
}

void endVisitWinCondVariable()
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


void beginVisitFlworReturn(const PlanIterator& a)
{
  thePrinter.startBeginVisit("ReturnClause", ++theId);
  thePrinter.endBeginVisit(theId);
  a.accept(*this);
}


void endVisitFlworReturn(const PlanIterator& /*a*/)
{
  thePrinter.startEndVisit();
  thePrinter.endEndVisit();
}


  void beginVisit(const CastIterator& a) {
    thePrinter.startBeginVisit("CastIterator", ++theId);
    std::ostringstream lStream;
    TypeOps::serialize(lStream, *a.theCastType);
    thePrinter.addAttribute("type", lStream.str());
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }

  void endVisit(const CastIterator&) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

  PRINTER_VISITOR_DEFINITION (NameCastIterator)

  void beginVisit(const PromoteIterator& a) {
    thePrinter.startBeginVisit("PromoteIterator", ++theId);
    std::ostringstream lStream;
    TypeOps::serialize(lStream, *a.thePromoteType);
    thePrinter.addAttribute("type", lStream.str());
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }

  void endVisit(const PromoteIterator&) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

  void beginVisit(const CastableIterator& a) {
    thePrinter.startBeginVisit("CastableIterator", ++theId);
    std::ostringstream lStream;
    TypeOps::serialize(lStream, *a.theCastType);
    thePrinter.addAttribute("type", lStream.str());
    printCommons( &a, theId );
    thePrinter.endBeginVisit(theId);
  }

  void endVisit(const CastableIterator&) {
    thePrinter.startEndVisit();
    thePrinter.endEndVisit();
  }

  PRINTER_VISITOR_DEFINITION (CtxVariableIterator)
  PRINTER_VISITOR_DEFINITION (CtxVarDeclIterator)
  PRINTER_VISITOR_DEFINITION (CtxVarAssignIterator)
  PRINTER_VISITOR_DEFINITION (CtxVarExistsIterator)

  PRINTER_VISITOR_DEFINITION (EvalIterator)

  PRINTER_VISITOR_DEFINITION (OpIsSameNodeIterator)

  PRINTER_VISITOR_DEFINITION (OpNodeBeforeIterator)

  PRINTER_VISITOR_DEFINITION (OpNodeAfterIterator)

  PRINTER_VISITOR_DEFINITION (FnCurrentDateTimeIterator)

  PRINTER_VISITOR_DEFINITION (FnCurrentDateIterator)

  PRINTER_VISITOR_DEFINITION (FnCurrentTimeIterator)

  PRINTER_VISITOR_DEFINITION (FnImplicitTimezoneIterator)

  PRINTER_VISITOR_DEFINITION (FnDefaultCollationIterator)

  PRINTER_VISITOR_DEFINITION(FnAdjustToTimeZoneIterator_1);

  PRINTER_VISITOR_DEFINITION(FnAdjustToTimeZoneIterator_2);

  PRINTER_VISITOR_DEFINITION(FnFormatDateTimeIterator);

  PRINTER_VISITOR_DEFINITION(InsertIterator);

  PRINTER_VISITOR_DEFINITION(DeleteIterator);

  PRINTER_VISITOR_DEFINITION(ReplaceIterator);

  PRINTER_VISITOR_DEFINITION(RenameIterator);

  PRINTER_VISITOR_DEFINITION(TransformIterator);

  PRINTER_VISITOR_DEFINITION(ApplyIterator);

  PRINTER_VISITOR_DEFINITION(FnDeepEqualIterator);

  PRINTER_VISITOR_DEFINITION(FnFormatNumberIterator);

  PRINTER_VISITOR_DEFINITION(ZorbaCollectionExistsIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaImportXmlIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaImportCatalogIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaImportFolderIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaListCollectionsIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaCreateCollectionIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaDeleteCollectionIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaDeleteAllCollectionsIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaInsertNodeFirstIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaInsertNodeLastIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaInsertNodeBeforeIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaInsertNodeAfterIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaInsertNodeAtIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaRemoveNodeIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaRemoveNodeAtIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaNodeCountIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaNodeAtIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaIndexOfIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaExportXmlIterator);

  PRINTER_VISITOR_DEFINITION(CreateValueIndex);
  PRINTER_VISITOR_DEFINITION(CreateInternalIndexIterator);
  PRINTER_VISITOR_DEFINITION(DropValueIndex);
  PRINTER_VISITOR_DEFINITION(ValueIndexPointProbe);
  PRINTER_VISITOR_DEFINITION(ValueIndexRangeProbe);

  PRINTER_VISITOR_DEFINITION(ZorbaSchemaTypeIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaBase64EncodeIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaBase64DecodeIterator);
  PRINTER_VISITOR_DEFINITION(XQDocIterator);
#ifdef ZORBA_WITH_TIDY
  PRINTER_VISITOR_DEFINITION(ZorbaTidyIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaTDocIterator);
#endif  /* ZORBA_WITH_TIDY */
  PRINTER_VISITOR_DEFINITION(ZorbaRandomIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaUUIDIterator);
#ifdef ZORBA_WITH_EMAIL
  PRINTER_VISITOR_DEFINITION(ZorbaMailIterator);
#endif  /* ZORBA_WITH_EMAIL */

  PRINTER_VISITOR_DEFINITION(ZorbaTimestampIterator);

  PRINTER_VISITOR_DEFINITION(ZorbaDebugIterator);
  PRINTER_VISITOR_DEFINITION(DebuggerSingletonIterator);

#ifdef ZORBA_WITH_REST
  PRINTER_VISITOR_DEFINITION(ZorbaRestGetIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaRestPostIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaRestPutIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaRestDeleteIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaRestHeadIterator);
#endif

#ifdef ZORBA_WITH_FOP
  PRINTER_VISITOR_DEFINITION(ZorbaFopIterator);
#endif /* ZORBA_WITH_FOP */

  PRINTER_VISITOR_DEFINITION(HoistIterator);
  PRINTER_VISITOR_DEFINITION(UnhoistIterator);

  PRINTER_VISITOR_DEFINITION(ZorbaJsonParseIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaJsonSerializeIterator);

  PRINTER_VISITOR_DEFINITION(ZorbaJsonMLParseIterator);
  PRINTER_VISITOR_DEFINITION(ZorbaJsonMLSerializeIterator);
};

void print_iter_plan (IterPrinter& aPrinter, PlanIterator* aIter) {
  PrinterVisitor v (aPrinter, aIter);
  v.print ();
}

} /* namespace zorba */
/* vim:set ts=2 sw=2: */

