/*
 * Copyright 2006-2012 The FLWOR Foundation.
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
 
// ******************************************
// *                                        *
// * THIS IS A GENERATED FILE. DO NOT EDIT! *
// * SEE .xml FILE WITH SAME NAME           *
// *                                        *
// ******************************************

#ifndef ZORBA_RUNTIME_MATHS_MATHS_H
#define ZORBA_RUNTIME_MATHS_MATHS_H


#include "common/shared_types.h"
#include "runtime/base/unarybase.h"
#include "runtime/base/binarybase.h"
#include "runtime/base/noarybase.h"

#include "zorbatypes/integer.h"
#include "zorbatypes/schema_types.h"


namespace zorba {

/**
 * math:sqrt
 * Author: Zorba Team
 */
class SqrtIterator : public UnaryBaseIterator<SqrtIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(SqrtIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(SqrtIterator,
    UnaryBaseIterator<SqrtIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  SqrtIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<SqrtIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~SqrtIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:exp
 * Author: Zorba Team
 */
class ExpIterator : public UnaryBaseIterator<ExpIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(ExpIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(ExpIterator,
    UnaryBaseIterator<ExpIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  ExpIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<ExpIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~ExpIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:exp10
 * Author: Zorba Team
 */
class Exp10Iterator : public UnaryBaseIterator<Exp10Iterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(Exp10Iterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(Exp10Iterator,
    UnaryBaseIterator<Exp10Iterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  Exp10Iterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<Exp10Iterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~Exp10Iterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:log
 * Author: Zorba Team
 */
class LogIterator : public UnaryBaseIterator<LogIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(LogIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(LogIterator,
    UnaryBaseIterator<LogIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  LogIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<LogIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~LogIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:log10
 * Author: Zorba Team
 */
class Log10Iterator : public UnaryBaseIterator<Log10Iterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(Log10Iterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(Log10Iterator,
    UnaryBaseIterator<Log10Iterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  Log10Iterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<Log10Iterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~Log10Iterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:sin
 * Author: Zorba Team
 */
class SinIterator : public UnaryBaseIterator<SinIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(SinIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(SinIterator,
    UnaryBaseIterator<SinIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  SinIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<SinIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~SinIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:cos
 * Author: Zorba Team
 */
class CosIterator : public UnaryBaseIterator<CosIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(CosIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(CosIterator,
    UnaryBaseIterator<CosIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  CosIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<CosIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~CosIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:tan
 * Author: Zorba Team
 */
class TanIterator : public UnaryBaseIterator<TanIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(TanIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(TanIterator,
    UnaryBaseIterator<TanIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  TanIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<TanIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~TanIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:asin
 * Author: Zorba Team
 */
class ArcSinIterator : public UnaryBaseIterator<ArcSinIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(ArcSinIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(ArcSinIterator,
    UnaryBaseIterator<ArcSinIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  ArcSinIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<ArcSinIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~ArcSinIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:acos
 * Author: Zorba Team
 */
class ArcCosIterator : public UnaryBaseIterator<ArcCosIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(ArcCosIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(ArcCosIterator,
    UnaryBaseIterator<ArcCosIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  ArcCosIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<ArcCosIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~ArcCosIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:atan
 * Author: Zorba Team
 */
class ArcTanIterator : public UnaryBaseIterator<ArcTanIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(ArcTanIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(ArcTanIterator,
    UnaryBaseIterator<ArcTanIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  ArcTanIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<ArcTanIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~ArcTanIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:atan2
 * Author: Zorba Team
 */
class Atan2Iterator : public BinaryBaseIterator<Atan2Iterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(Atan2Iterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(Atan2Iterator,
    BinaryBaseIterator<Atan2Iterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  Atan2Iterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child1, PlanIter_t& child2)
    : 
    BinaryBaseIterator<Atan2Iterator, PlanIteratorState>(sctx, loc, child1, child2)
  {}

  virtual ~Atan2Iterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:cosh
 * Author: Zorba Team
 */
class CoshIterator : public UnaryBaseIterator<CoshIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(CoshIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(CoshIterator,
    UnaryBaseIterator<CoshIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  CoshIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<CoshIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~CoshIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:acosh
 * Author: Zorba Team
 */
class AcoshIterator : public UnaryBaseIterator<AcoshIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(AcoshIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(AcoshIterator,
    UnaryBaseIterator<AcoshIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  AcoshIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<AcoshIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~AcoshIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:fmod
 * Author: Zorba Team
 */
class FmodIterator : public BinaryBaseIterator<FmodIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(FmodIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(FmodIterator,
    BinaryBaseIterator<FmodIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  FmodIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child1, PlanIter_t& child2)
    : 
    BinaryBaseIterator<FmodIterator, PlanIteratorState>(sctx, loc, child1, child2)
  {}

  virtual ~FmodIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:ldexp
 * Author: Zorba Team
 */
class LdexpIterator : public BinaryBaseIterator<LdexpIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(LdexpIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(LdexpIterator,
    BinaryBaseIterator<LdexpIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  LdexpIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child1, PlanIter_t& child2)
    : 
    BinaryBaseIterator<LdexpIterator, PlanIteratorState>(sctx, loc, child1, child2)
  {}

  virtual ~LdexpIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:pow
 * Author: Zorba Team
 */
class PowIterator : public BinaryBaseIterator<PowIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(PowIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(PowIterator,
    BinaryBaseIterator<PowIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  PowIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child1, PlanIter_t& child2)
    : 
    BinaryBaseIterator<PowIterator, PlanIteratorState>(sctx, loc, child1, child2)
  {}

  virtual ~PowIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:sinh
 * Author: Zorba Team
 */
class SinhIterator : public UnaryBaseIterator<SinhIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(SinhIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(SinhIterator,
    UnaryBaseIterator<SinhIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  SinhIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<SinhIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~SinhIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:asinh
 * Author: Zorba Team
 */
class AsinhIterator : public UnaryBaseIterator<AsinhIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(AsinhIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(AsinhIterator,
    UnaryBaseIterator<AsinhIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  AsinhIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<AsinhIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~AsinhIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:tanh
 * Author: Zorba Team
 */
class TanhIterator : public UnaryBaseIterator<TanhIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(TanhIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(TanhIterator,
    UnaryBaseIterator<TanhIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  TanhIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<TanhIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~TanhIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:atanh
 * Author: Zorba Team
 */
class AtanhIterator : public UnaryBaseIterator<AtanhIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(AtanhIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(AtanhIterator,
    UnaryBaseIterator<AtanhIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  AtanhIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<AtanhIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~AtanhIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * math:pi
 * Author: Zorba Team
 */
class PiNumberIterator : public NoaryBaseIterator<PiNumberIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(PiNumberIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(PiNumberIterator,
    NoaryBaseIterator<PiNumberIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  PiNumberIterator(
    static_context* sctx,
    const QueryLoc& loc)
    : 
    NoaryBaseIterator<PiNumberIterator, PlanIteratorState>(sctx, loc)
  {}

  virtual ~PiNumberIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:is_inf
 * Author: Zorba Team
 */
class IsInfIterator : public UnaryBaseIterator<IsInfIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(IsInfIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(IsInfIterator,
    UnaryBaseIterator<IsInfIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  IsInfIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<IsInfIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~IsInfIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:is_nan
 * Author: Zorba Team
 */
class IsNaNIterator : public UnaryBaseIterator<IsNaNIterator, PlanIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(IsNaNIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(IsNaNIterator,
    UnaryBaseIterator<IsNaNIterator, PlanIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  IsNaNIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<IsNaNIterator, PlanIteratorState>(sctx, loc, child)
  {}

  virtual ~IsNaNIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:modf
 * Author: Zorba Team
 */
class ModfIteratorState : public PlanIteratorState
{
public:
  xs_double theDoubInteger; //

  ModfIteratorState();

  ~ModfIteratorState();

  void init(PlanState&);
  void reset(PlanState&);
};

class ModfIterator : public UnaryBaseIterator<ModfIterator, ModfIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(ModfIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(ModfIterator,
    UnaryBaseIterator<ModfIterator, ModfIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  ModfIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<ModfIterator, ModfIteratorState>(sctx, loc, child)
  {}

  virtual ~ModfIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


/**
 * zorba-math:frexp
 * Author: Zorba Team
 */
class FrexpIteratorState : public PlanIteratorState
{
public:
  xs_integer theIntExponent; //

  FrexpIteratorState();

  ~FrexpIteratorState();

  void init(PlanState&);
  void reset(PlanState&);
};

class FrexpIterator : public UnaryBaseIterator<FrexpIterator, FrexpIteratorState>
{ 
public:
  SERIALIZABLE_CLASS(FrexpIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(FrexpIterator,
    UnaryBaseIterator<FrexpIterator, FrexpIteratorState>);

  void serialize( ::zorba::serialization::Archiver& ar);

  FrexpIterator(
    static_context* sctx,
    const QueryLoc& loc,
    PlanIter_t& child)
    : 
    UnaryBaseIterator<FrexpIterator, FrexpIteratorState>(sctx, loc, child)
  {}

  virtual ~FrexpIterator();

  zstring getNameAsString() const;

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};


}
#endif
/*
 * Local variables:
 * mode: c++
 * End:
 */ 
