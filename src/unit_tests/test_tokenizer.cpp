/*
 * Copyright 2006-2010 The FLWOR Foundation.
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
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>

#include <zorba/diagnostic_list.h>
#include <zorba/properties.h>
#include <zorba/store_consts.h>
#include <zorba/store_manager.h>
#include <zorba/tokenizer.h>
#include <zorba/user_exception.h>
#include <zorba/zorba.h>
#include <zorba/zorba_exception.h>

using namespace std;
using namespace zorba;
using namespace zorba::locale;

static bool destroy_called;

///////////////////////////////////////////////////////////////////////////////

namespace std {

template<> struct less<char const*> :
  std::binary_function<char const*,char const*,bool>
{
  less() { }

  result_type
  operator()( first_argument_type a, second_argument_type b ) const {
    return std::strcmp( a, b ) < 0;
  }
};

} // namespace std

///////////////////////////////////////////////////////////////////////////////

class TestTokenizer : public Tokenizer {
public:
  TestTokenizer( State &state ) : Tokenizer( state ) { }
  ~TestTokenizer();

  // inherited
  void destroy() const;
  void properties( Properties* ) const;
  void tokenize_string( char const*, size_type, iso639_1::type, bool,
                        Callback&, Item const* );

protected:
  // inherited
  void item( Item const&, bool );

private:
  typedef std::string token_t;

  enum token_type {
    t_generic,
    t_integer,
    t_floating_point,
    t_time
  };

  token_type t_type_;

  static bool is_word_begin_char( char );
  bool is_word_char( char );
  static char peek( char const *s, char const *end );
  bool send_token( token_t const &token, iso639_1::type, Callback&,
                   Item const* );
};

TestTokenizer::~TestTokenizer() {
  // out-of-line since it's virtual
}

void TestTokenizer::destroy() const {
  destroy_called = true;
  delete this;
}

void TestTokenizer::item( Item const &item, bool entering ) {
  static char const *const block_elements[] = {
    "address",
    "blockquote",
    "dd",
    "div",
    "dt",
    "h1", "h2", "h3", "h4", "h5", "h6",
    "legend",
    "li",
    "p",
    "pre",
    "td",
    "th",
  };
  static char const *const *const end =
    block_elements + sizeof( block_elements ) / sizeof( char* );

  if ( entering && item.isNode() &&
       item.getNodeKind() == store::StoreConsts::elementNode ) {
    Item qname;
    item.getNodeName( qname );
    if ( ::binary_search( block_elements, end, qname.getLocalName().c_str(),
                          less<char const*>() ) ) {
      ++state().para;
    }
  }
}

inline bool TestTokenizer::is_word_begin_char( char c ) {
  return !!isalnum( c );
}

bool TestTokenizer::is_word_char( char c ) {
  if ( t_type_ == t_integer ) {
    switch ( c ) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        break;
      case ',':
      case '.':
        t_type_ = t_floating_point;
        break;
      case ':':
        t_type_ = t_time;
        break;
      default:
        t_type_ = t_generic;
    }
  }
  switch ( t_type_ ) {
    case t_floating_point:
      return isdigit( c ) || c == '.' || c == ',';
    case t_time:
      return isdigit( c ) || c == ':';
    default:
      return is_word_begin_char( c ) 
          || c == '&'                   // AT&T
          || c == '\''                  // men's
          || c == '-'                   // set-top
          || c == '_';                  // language_identifier
  }
}

inline char TestTokenizer::peek( char const *s, char const *end ) {
  return ++s < end ? *s : '\0';
}

void TestTokenizer::properties( Properties *p ) const {
  p->comments_separate_tokens = true;
  p->elements_separate_tokens = true;
  p->processing_instructions_separate_tokens = true;
  p->languages.clear();
  p->languages.push_back( iso639_1::en );
  p->uri = "http://www.zorba-xquery.com/full-text/tokenizer/unit-test";
}

#define HANDLE_BACKSLASH()            \
  if ( !got_backslash ) ; else {      \
    got_backslash = in_wild = false;  \
    break;                            \
  }

void TestTokenizer::tokenize_string( char const *s, size_type s_len,
                                     iso639_1::type lang, bool wildcards,
                                     Callback &callback, Item const *item ) {
  bool got_backslash = false;
  bool in_wild = false;
  token_t token;

  t_type_ = t_generic;

  for ( char const *const end = s + s_len; s < end; ++s ) {
    if ( wildcards ) {
      switch ( *s ) {
        case '.':
          HANDLE_BACKSLASH();
          in_wild = true;
          break;
        case '\\':
          HANDLE_BACKSLASH();
          got_backslash = true;
          break;
        case '*':
        case '+':
        case '?':
          HANDLE_BACKSLASH();
          break;
        case '{':
          HANDLE_BACKSLASH();
          if ( in_wild ) {
            //
            // Validate that the regex "{[0-9]+,[0-9]+}" is matched.
            //
            bool got_comma = false;
            bool got_digit = false;
            while ( ++s < end ) {
              if ( got_digit && *s == ',' ) {
                got_comma = true;
                break;
              }
              if ( !isdigit( *s ) )
                throw USER_EXCEPTION( err::FTDY0020, "digit expected" );
              got_digit = true;
            }
            if ( !got_comma )
              throw USER_EXCEPTION( err::FTDY0020, "',' expected" );
            got_digit = false;
            while ( ++s < end ) {
              if ( *s == '}' ) {
                if ( !got_digit )
                  throw USER_EXCEPTION( err::FTDY0020, "digit expected" );
                break;
              }
              if ( !isdigit( *s ) )
                throw USER_EXCEPTION( err::FTDY0020, "digit expected" );
              got_digit = true;
            }
          }
          break;
        default:
          in_wild = false;
      } // switch
    } // if ( wildcards )

    if ( token.empty() ) {
      if ( is_word_begin_char( *s ) ) {
        token += *s;
        if ( isdigit( *s ) )
          t_type_ = t_integer;
      }
    } else {
      if ( is_word_char( *s ) )
        token += *s;
      else if ( send_token( token, lang, callback, item ) ) {
        token.clear();
        t_type_ = t_generic;
      }
    }

    if ( !wildcards )
      switch ( *s ) {
        case '.':
          switch ( peek( s, end ) ) {
            case '"':
            case '\'':
            case ')':
            case ']':
            case '}':
            case ' ':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
            case '\v':
              break;
            default:
              continue;
          }
          // no break;
        case '!':
        case '?':
          ++state().sent;
      }
  } // for

  send_token( token, lang, callback, item );
}

static char const *const tokens[] = {
  "It", "was", "the", "best", "of", "times",
  "It", "was", "the", "worst", "of", "times",
  "1.2", "10,000.00",
  "1,2", "10.000,00",
  "12:00", "1:42", "02:12:37",
};
static unsigned num_tokens = sizeof( tokens ) / sizeof( tokens[0] );

static void check_token( char const *token, Tokenizer::size_type t_no ) {
  static bool did_t0 = false;
  if ( !t_no ) {
    if ( did_t0 )
      return;
    did_t0 = true;
  }
  if ( t_no < num_tokens && ::strcmp( token, tokens[ t_no ] ) != 0 )
    ::exit( 3 );
}

#define PRINT_TOKENS 0

bool TestTokenizer::send_token( token_t const &token, iso639_1::type lang,
                                Callback &callback, Item const *item ) {
  if ( !token.empty() ) {
#if PRINT_TOKENS
    cout <<   "t=" << setw(2) << state().token
         << ", s=" << setw(2) << state().sent
         << ", p=" << setw(2) << state().para
         << ": \"" << token << "\"\n";
#endif /* PRINT_TOKENS */

    check_token( token.c_str(), state().token );

    callback.token(
      token.data(), token.size(), lang,
      state().token, state().sent, state().para, item
    );
    ++state().token;
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

class TestTokenizerProvider : public TokenizerProvider {
public:
  // inherited
  bool getTokenizer( iso639_1::type, Tokenizer::State* = 0,
                     Tokenizer::ptr* = 0 ) const;
};

bool TestTokenizerProvider::getTokenizer( iso639_1::type lang,
                                          Tokenizer::State *state,
                                          Tokenizer::ptr *t ) const {
  if ( state && t )
    t->reset( new TestTokenizer( *state ) );
  return true;
}

///////////////////////////////////////////////////////////////////////////////

namespace zorba {
namespace UnitTests {

int test_tokenizer( int argc, char *argv[] ) {
  int result = 0;

  void *const zstore = StoreManager::getStore();
  Zorba *const zorba = Zorba::getInstance( zstore );

  try {
    char const *const query_src =
      "let $x :="
      "  <content>"
      "    <quote>"
      "      <p>It was the best of times.</p>"
      "      <p>It was the worst of times</p>."
      "    </quote>"
      "    <numbers-us>1.2 10,000.00</numbers-us>"
      "    <numbers-eu>1,2 10.000,00</numbers-eu>"
      "    <times>12:00 1:42 02:12:37</times>"
      "  </content>"
      "return $x contains text \"times\"";

    TestTokenizerProvider provider;
    XmlDataManager_t xmlDataMgr = zorba->getXmlDataManager();
    xmlDataMgr->registerTokenizerProvider( &provider );

    StaticContext_t sctx = zorba->createStaticContext();
    XQuery_t xquery = zorba->compileQuery( query_src, sctx );

    Zorba_SerializerOptions ser_options;
    ser_options.omit_xml_declaration = ZORBA_OMIT_XML_DECLARATION_YES;
    std::ostringstream sout;
    xquery->execute( sout, &ser_options );

    if ( !destroy_called )
      result = 1;

    if ( sout.str() != "true" )
      result = 2;
  }
  catch ( ZorbaException const &e ) {
    cerr << e << endl;
    result = 2;
  }

  zorba->shutdown();
  StoreManager::shutdownStore( zstore );
  return result;
}

} // namespace UnitTests
} // namespace zorba

/* vim:set et sw=2 ts=2: */
