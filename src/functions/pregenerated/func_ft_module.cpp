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



#include "stdafx.h"
#include "runtime/full_text/ft_module.h"
#include "functions/func_ft_module.h"


namespace zorba{


#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_current_compare_options::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new CurrentCompareOptionsIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_current_lang::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new CurrentLangIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_host_lang::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new HostLangIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_is_stem_lang_supported::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new IsStemLangSupportedIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_is_stop_word::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new IsStopWordIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_is_stop_word_lang_supported::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new IsStopWordLangSupportedIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_is_thesaurus_lang_supported::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new IsThesaurusLangSupportedIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_is_tokenizer_lang_supported::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new IsTokenizerLangSupportedIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_stem::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new StemIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_strip_diacritics::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new StripDiacriticsIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_thesaurus_lookup::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new ThesaurusLookupIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_tokenize_node::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new TokenizeNodeIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_tokenize_nodes::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new TokenizeNodesIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_tokenizer_properties::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new TokenizerPropertiesIterator(sctx, loc, argv);
}

#endif
#ifndef ZORBA_NO_FULL_TEXT
PlanIter_t full_text_tokenize_string::codegen(
  CompilerCB*,
  static_context* sctx,
  const QueryLoc& loc,
  std::vector<PlanIter_t>& argv,
  expr& ann) const
{
  return new TokenizeStringIterator(sctx, loc, argv);
}

#endif

void populate_context_ft_module(static_context* sctx)
{

#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_current_compare_options,
        (createQName("http://zorba.io/modules/full-text","","current-compare-options"), 
        GENV_TYPESYSTEM.JSON_OBJECT_TYPE_ONE),
        FunctionConsts::FULL_TEXT_CURRENT_COMPARE_OPTIONS_0);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_current_lang,
        (createQName("http://zorba.io/modules/full-text","","current-lang"), 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE),
        FunctionConsts::FULL_TEXT_CURRENT_LANG_0);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_host_lang,
        (createQName("http://zorba.io/modules/full-text","","host-lang"), 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE),
        FunctionConsts::FULL_TEXT_HOST_LANG_0);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_is_stem_lang_supported,
        (createQName("http://zorba.io/modules/full-text","","is-stem-lang-supported"), 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.BOOLEAN_TYPE_ONE),
        FunctionConsts::FULL_TEXT_IS_STEM_LANG_SUPPORTED_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_is_stop_word,
        (createQName("http://zorba.io/modules/full-text","","is-stop-word"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.BOOLEAN_TYPE_ONE),
        FunctionConsts::FULL_TEXT_IS_STOP_WORD_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_is_stop_word,
        (createQName("http://zorba.io/modules/full-text","","is-stop-word"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.BOOLEAN_TYPE_ONE),
        FunctionConsts::FULL_TEXT_IS_STOP_WORD_2);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_is_stop_word_lang_supported,
        (createQName("http://zorba.io/modules/full-text","","is-stop-word-lang-supported"), 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.BOOLEAN_TYPE_ONE),
        FunctionConsts::FULL_TEXT_IS_STOP_WORD_LANG_SUPPORTED_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_is_thesaurus_lang_supported,
        (createQName("http://zorba.io/modules/full-text","","is-thesaurus-lang-supported"), 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.BOOLEAN_TYPE_ONE),
        FunctionConsts::FULL_TEXT_IS_THESAURUS_LANG_SUPPORTED_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_is_thesaurus_lang_supported,
        (createQName("http://zorba.io/modules/full-text","","is-thesaurus-lang-supported"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.BOOLEAN_TYPE_ONE),
        FunctionConsts::FULL_TEXT_IS_THESAURUS_LANG_SUPPORTED_2);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_is_tokenizer_lang_supported,
        (createQName("http://zorba.io/modules/full-text","","is-tokenizer-lang-supported"), 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.BOOLEAN_TYPE_ONE),
        FunctionConsts::FULL_TEXT_IS_TOKENIZER_LANG_SUPPORTED_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_stem,
        (createQName("http://zorba.io/modules/full-text","","stem"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE),
        FunctionConsts::FULL_TEXT_STEM_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_stem,
        (createQName("http://zorba.io/modules/full-text","","stem"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE),
        FunctionConsts::FULL_TEXT_STEM_2);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_strip_diacritics,
        (createQName("http://zorba.io/modules/full-text","","strip-diacritics"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE),
        FunctionConsts::FULL_TEXT_STRIP_DIACRITICS_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_thesaurus_lookup,
        (createQName("http://zorba.io/modules/full-text","","thesaurus-lookup"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_STAR),
        FunctionConsts::FULL_TEXT_THESAURUS_LOOKUP_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_thesaurus_lookup,
        (createQName("http://zorba.io/modules/full-text","","thesaurus-lookup"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_STAR),
        FunctionConsts::FULL_TEXT_THESAURUS_LOOKUP_2);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_thesaurus_lookup,
        (createQName("http://zorba.io/modules/full-text","","thesaurus-lookup"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_STAR),
        FunctionConsts::FULL_TEXT_THESAURUS_LOOKUP_3);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_thesaurus_lookup,
        (createQName("http://zorba.io/modules/full-text","","thesaurus-lookup"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_STAR),
        FunctionConsts::FULL_TEXT_THESAURUS_LOOKUP_4);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_thesaurus_lookup,
        (createQName("http://zorba.io/modules/full-text","","thesaurus-lookup"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.INTEGER_TYPE_ONE, 
        GENV_TYPESYSTEM.INTEGER_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_STAR),
        FunctionConsts::FULL_TEXT_THESAURUS_LOOKUP_6);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_tokenize_node,
        (createQName("http://zorba.io/modules/full-text","","tokenize-node"), 
        GENV_TYPESYSTEM.ANY_NODE_TYPE_ONE, 
        GENV_TYPESYSTEM.JSON_OBJECT_TYPE_STAR),
        FunctionConsts::FULL_TEXT_TOKENIZE_NODE_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_tokenize_node,
        (createQName("http://zorba.io/modules/full-text","","tokenize-node"), 
        GENV_TYPESYSTEM.ANY_NODE_TYPE_ONE, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.JSON_OBJECT_TYPE_STAR),
        FunctionConsts::FULL_TEXT_TOKENIZE_NODE_2);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_tokenize_nodes,
        (createQName("http://zorba.io/modules/full-text","","tokenize-nodes"), 
        GENV_TYPESYSTEM.ANY_NODE_TYPE_PLUS, 
        GENV_TYPESYSTEM.ANY_NODE_TYPE_STAR, 
        GENV_TYPESYSTEM.JSON_OBJECT_TYPE_STAR),
        FunctionConsts::FULL_TEXT_TOKENIZE_NODES_2);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_tokenize_nodes,
        (createQName("http://zorba.io/modules/full-text","","tokenize-nodes"), 
        GENV_TYPESYSTEM.ANY_NODE_TYPE_PLUS, 
        GENV_TYPESYSTEM.ANY_NODE_TYPE_STAR, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.JSON_OBJECT_TYPE_STAR),
        FunctionConsts::FULL_TEXT_TOKENIZE_NODES_3);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_tokenizer_properties,
        (createQName("http://zorba.io/modules/full-text","","tokenizer-properties"), 
        GENV_TYPESYSTEM.JSON_OBJECT_TYPE_ONE),
        FunctionConsts::FULL_TEXT_TOKENIZER_PROPERTIES_0);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_tokenizer_properties,
        (createQName("http://zorba.io/modules/full-text","","tokenizer-properties"), 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.JSON_OBJECT_TYPE_ONE),
        FunctionConsts::FULL_TEXT_TOKENIZER_PROPERTIES_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_tokenize_string,
        (createQName("http://zorba.io/modules/full-text","","tokenize-string"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_STAR),
        FunctionConsts::FULL_TEXT_TOKENIZE_STRING_1);

  }


#endif


#ifndef ZORBA_NO_FULL_TEXT


      {
    DECL_WITH_KIND(sctx, full_text_tokenize_string,
        (createQName("http://zorba.io/modules/full-text","","tokenize-string"), 
        GENV_TYPESYSTEM.STRING_TYPE_ONE, 
        GENV_TYPESYSTEM.LANGUAGE_TYPE_ONE, 
        GENV_TYPESYSTEM.STRING_TYPE_STAR),
        FunctionConsts::FULL_TEXT_TOKENIZE_STRING_2);

  }


#endif
}


}



