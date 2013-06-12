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

#include "file.h"

// standard
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

// Zorba
#include <zorba/base64.h>
#include <zorba/diagnostic_list.h>
#include <zorba/empty_sequence.h>
#include <zorba/serializer.h>
#include <zorba/singleton_item_sequence.h>
#include <zorba/user_exception.h>
#include <zorba/util/fs_util.h>
#include <zorba/util/stream_util.h>
#include <zorba/util/transcode_stream.h>

// local
#include "file_module.h"

namespace zorba {
namespace filemodule {

//*****************************************************************************

CreateDirectoryFunction::CreateDirectoryFunction(const FileModule* aModule)
  : FileFunction(aModule)
{
}

ItemSequence_t
CreateDirectoryFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );

  if ( fs::get_type( path ) )
    raiseFileError( "FOFL0002", "file already exists", path );

  try {
    fs::mkdir( path );
  }
  catch ( std::exception const &e ) {
    throw raiseFileError( "FOFL9999", e.what(), path );
  }

  return ItemSequence_t( new EmptySequence() );
}

//*****************************************************************************

DeleteFileImplFunction::DeleteFileImplFunction(const FileModule* aModule) :
  FileFunction(aModule)
{
}

ItemSequence_t
DeleteFileImplFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );

  if ( !fs::get_type( path, false ) )
    raiseFileError( "FOFL0001", "file not found", path );

  try {
    fs::remove( path );
  }
  catch ( std::exception const &e ) {
    throw raiseFileError( "FOFL9999", e.what(), path );
  }

  return ItemSequence_t( new EmptySequence() );
}

//*****************************************************************************

ReadBinaryFunction::ReadBinaryFunction( FileModule const *aModule ) :
  FileFunction( aModule )
{
}

ItemSequence_t
ReadBinaryFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );

  fs::type const fs_type = fs::get_type( path );
  if ( !fs_type )
    raiseFileError( "FOFL0001", "file not found", path );
  if ( fs_type != fs::file )
    raiseFileError( "FOFL0004", "not a plain file", path );

  try {
    std::unique_ptr<std::ifstream> pin(
      new std::ifstream( path.c_str(), std::ios_base::binary )
    );
    Item item = theModule->getItemFactory()->createStreamableBase64Binary(
      *pin, &FileModule::streamReleaser, true
    );
    pin.release();
    return ItemSequence_t( new SingletonItemSequence( item ) );
  }
  catch ( std::exception const &e ) {
    throw raiseFileError( "FOFL9999", e.what(), path );
  }
}

//*****************************************************************************

ReadTextFunction::ReadTextFunction(const FileModule* aModule)
  : StreamableFileFunction(aModule)
{
}

ItemSequence_t
ReadTextFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );
  String lEncoding( "UTF-8" );

  fs::type const fs_type = fs::get_type( path );
  if ( !fs_type )
    raiseFileError( "FOFL0001", "file not found", path );
  if ( fs_type != fs::file )
    raiseFileError( "FOFL0004", "not a plain file", path );

  if ( aArgs.size() == 2 )
    lEncoding = getEncodingArg( aArgs, 1 );
  
  zorba::Item lResult;

  std::unique_ptr<std::ifstream> pin;
  if ( transcode::is_necessary( lEncoding.c_str() ) ) {
    try {
      pin.reset( new transcode::stream<std::ifstream>( lEncoding.c_str() ) );
    }
    catch ( std::exception const &e ) {
      throw raiseFileError("FOFL0006", e.what(), lEncoding.c_str() );
    }
  } else
    pin.reset( new std::ifstream() );

  pin->open( path.c_str() );
  skip_utf8_bom( *pin );
  lResult = theModule->getItemFactory()->createStreamableString(
    *pin, &FileModule::streamReleaser, path.c_str(), true
  );
  pin.release();

  return ItemSequence_t( new SingletonItemSequence( lResult ) );
}

//*****************************************************************************

ReadTextLinesFunction::ReadTextLinesFunction(const FileModule* aModule)
  : FileFunction(aModule)
{
}

ItemSequence_t
ReadTextLinesFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );
  String const lEncoding( getEncodingArg( aArgs, 1 ) );

  fs::type const fs_type = fs::get_type( path );
  if ( !fs_type )
    raiseFileError( "FOFL0001", "file not found", path );
  if ( fs_type != fs::file )
    raiseFileError( "FOFL0004", "not a plain file", path );

  return ItemSequence_t( new LinesItemSequence( path, lEncoding, this ) );
}

ReadTextLinesFunction::LinesItemSequence::LinesItemSequence(
  String const &aFile,
  String const &aEncoding,
  ReadTextLinesFunction const *aFunc
) :
  theFile( aFile ),
  theEncoding( aEncoding ),
  theFunc( aFunc )
{
}

Iterator_t
ReadTextLinesFunction::LinesItemSequence::getIterator()
{
  return new ReadTextLinesFunction::LinesItemSequence::LinesIterator(
    theFile, theEncoding, theFunc
  );
}

ReadTextLinesFunction::LinesItemSequence::LinesIterator::LinesIterator(
  String const &aFile,
  String const &aEncoding,
  ReadTextLinesFunction const *aFunc
) :
  theFile(aFile),
  theEncoding(aEncoding),
  theFunc(aFunc),
  theStream(0)
{
}

ReadTextLinesFunction::LinesItemSequence::LinesIterator::~LinesIterator()
{
  delete theStream;
}

void
ReadTextLinesFunction::LinesItemSequence::LinesIterator::open()
{
  if ( transcode::is_necessary( theEncoding.c_str() ) ) {
    try {
      theStream = new transcode::stream<std::ifstream>( theEncoding.c_str() );
    }
    catch ( std::invalid_argument const &e ) {
      theFunc->raiseFileError( "FOFL0006", e.what(), theEncoding.c_str() );
    }
  } else
    theStream = new std::ifstream();

  theStream->open( theFile.c_str() );
}

bool
ReadTextLinesFunction::LinesItemSequence::LinesIterator::next(Item& aRes)
{
  if ( !theStream || !theStream->good() )
    return false;

  std::string s;
  getline( *theStream, s );
  if ( theStream->bad() )
    return false;
  
  aRes = theFunc->theModule->getItemFactory()->createString( s );
  return true;
}

void
ReadTextLinesFunction::LinesItemSequence::LinesIterator::close()
{
  delete theStream;
  theStream = 0;
}

bool
ReadTextLinesFunction::LinesItemSequence::LinesIterator::isOpen() const
{
  return theStream != 0;
}

//*****************************************************************************

ExistsFunction::ExistsFunction(const FileModule* aModule) :
  FileFunction( aModule )
{
}

ItemSequence_t
ExistsFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path = getFilePathString( aArgs, 0 );
  bool const follow_symlink = getItem( aArgs, 1 ).getBooleanValue();
  bool const exists = !!fs::get_type( path, follow_symlink );
  return ItemSequence_t(
    new SingletonItemSequence(
      theModule->getItemFactory()->createBoolean( exists )
    )
  );
}

//*****************************************************************************

IsDirectoryFunction::IsDirectoryFunction( FileModule const *aModule ) :
  FileFunction( aModule )
{
}

ItemSequence_t
IsDirectoryFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );
  bool const is_directory = fs::get_type( path ) == fs::directory;
  return ItemSequence_t(
    new SingletonItemSequence(
      theModule->getItemFactory()->createBoolean( is_directory )
    )
  );
}

//*****************************************************************************

IsFileFunction::IsFileFunction( FileModule const *aModule ) :
  FileFunction( aModule )
{
}

ItemSequence_t
IsFileFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );
  bool const is_file = fs::get_type( path ) == fs::file;
  return ItemSequence_t(
    new SingletonItemSequence(
      theModule->getItemFactory()->createBoolean( is_file )
    )
  );
}

//*****************************************************************************

IsSymlinkFunction::IsSymlinkFunction( FileModule const *aModule ) :
  FileFunction( aModule )
{
}

ItemSequence_t
IsSymlinkFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );
  bool const is_symlink = fs::get_type( path, false ) == fs::link;
  return ItemSequence_t(
    new SingletonItemSequence(
      theModule->getItemFactory()->createBoolean( is_symlink )
    )
  );
}

//*****************************************************************************

CopyFileImplFunction::CopyFileImplFunction( FileModule const *aModule ) :
  FileFunction( aModule )
{
}

ItemSequence_t
CopyFileImplFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const src_path( getFilePathString( aArgs, 0 ) );
  String dst_path( getFilePathString( aArgs, 1 ) );

  fs::type const src_type = fs::get_type( src_path );
  if ( !src_type )
    raiseFileError( "FOFL0001", "file not found", src_path );
  if ( src_type != fs::file )
    raiseFileError( "FOFL0004", "not a plain file", src_path );

  fs::type dst_type = fs::get_type( dst_path );
  if ( dst_type == fs::directory ) {    // we are copying into a directory
    fs::append( dst_path, fs::base_name( src_path ) );
    dst_type = fs::get_type( dst_path );
    if ( dst_type == fs::directory )
      raiseFileError( "FOFL0002", "path already exists", dst_path );
  }

  if ( src_path == dst_path )
    raiseFileError( "FOFL9999", "source and destination paths must not be equal", src_path );

  try {
    std::ifstream fin( src_path.c_str(), std::ios_base::binary );
    std::ofstream fout( dst_path.c_str(), std::ios_base::binary | std::ios_base::trunc );
    char buf[ 8192 ];
    while ( !fin.eof() ) {
      fin.read( buf, sizeof buf );
      fout.write( buf, fin.gcount() );
    }  
  }
  catch ( std::exception const &e ) {
    throw raiseFileError( "FOFL9999", e.what(), src_path );
  }

  return ItemSequence_t( new EmptySequence() );
}

//*****************************************************************************

ListFunction::ListFunction( FileModule const *aModule ) :
  FileFunction( aModule )
{
}

ItemSequence_t
ListFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );

  if ( fs::get_type( path ) != fs::directory )
    raiseFileError( "FOFL0003", "path is not a directory", path );

  try {
    return ItemSequence_t(
      new IteratorBackedItemSequence( path, theModule->getItemFactory() )
    );
  }
  catch ( std::exception const &e ) {
    throw raiseFileError( "FOFL9999", e.what(), path );
  }
}

ListFunction::IteratorBackedItemSequence::IteratorBackedItemSequence(
  String const& path,
  ItemFactory* aFactory
) :
  theIterator( path ),
  theItemFactory( aFactory )
{
  is_open = false;
  open_count = 0;
}

ListFunction::IteratorBackedItemSequence::~IteratorBackedItemSequence()
{
}

Iterator_t ListFunction::IteratorBackedItemSequence::getIterator()
{
  return this;
}

void ListFunction::IteratorBackedItemSequence::open()
{
  if (open_count) {
    theIterator.reset();
  }
  open_count++;
  is_open = true;
}

void ListFunction::IteratorBackedItemSequence::close()
{
  is_open = false;
}

bool ListFunction::IteratorBackedItemSequence::isOpen() const
{
  return is_open;
}

bool
ListFunction::IteratorBackedItemSequence::next(Item& lItem)
{
  if ( !theIterator.next() )
    return false;

  String lUriStr( theIterator->name );
  lItem = theItemFactory->createString( lUriStr );
  return true;
}

//*****************************************************************************

LastModifiedFunction::LastModifiedFunction(const FileModule* aModule) :
  FileFunction(aModule)
{
}

ItemSequence_t
LastModifiedFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );

  fs::info info;
  if ( !fs::get_type( path, &info ) )
    raiseFileError( "FOFL0001", "file not found", path );

  try {
    time_t lTime = info.mtime;
    // result of localtime needs to be copied.
    // Otherwise, nasty side effecs do happen
    struct tm lT(*localtime(&lTime));
    int gmtOffset = LastModifiedFunction::getGmtOffset();

    return ItemSequence_t(
      new SingletonItemSequence(
        theModule->getItemFactory()->createDateTime(
          1900 + lT.tm_year,
          lT.tm_mon,
          lT.tm_mday,
          lT.tm_hour,
          lT.tm_min, 
          lT.tm_sec,
          gmtOffset
        )
      )
    );
  }
  catch ( std::exception const &e ) {
    throw raiseFileError( "FOFL9999", e.what(), path );
  }
}

int
LastModifiedFunction::getGmtOffset()
{
  time_t t = ::time(0);
  struct tm* data;
  data = localtime(&t);
  data->tm_isdst = 0;
  time_t a = mktime(data);
  data = gmtime(&t);
  data->tm_isdst = 0;
  time_t b = mktime(data);
  return (int)(a - b)/3600; 
}

//*****************************************************************************

SizeFunction::SizeFunction(const FileModule* aModule)
  : FileFunction(aModule)
{
}

ItemSequence_t
SizeFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path( getFilePathString( aArgs, 0 ) );

  fs::info info;
  if ( !fs::get_type( path, &info ) )
    raiseFileError( "FOFL0001", "file not found", path );
  if ( info.type != fs::file )
    raiseFileError( "FOFL0004", "not plain file", path );

  return ItemSequence_t(new SingletonItemSequence(
    theModule->getItemFactory()->createInteger(info.size)));
}

//*****************************************************************************

PathSeparator::PathSeparator(const FileModule* aModule)
  : FileFunction(aModule)
{
}

ItemSequence_t
PathSeparator::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const path_separator( 1, fs::path_separator );
  return ItemSequence_t(
    new SingletonItemSequence(
      theModule->getItemFactory()->createString( path_separator )
    )
  );
}

//*****************************************************************************

DirectorySeparator::DirectorySeparator(const FileModule* aModule)
  : FileFunction(aModule)
{
}

ItemSequence_t
DirectorySeparator::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String const dir_separator( 1, fs::dir_separator );
  return ItemSequence_t(
    new SingletonItemSequence(
      theModule->getItemFactory()->createString( dir_separator )
    )
  );
}

//*****************************************************************************

ResolvePathFunction::ResolvePathFunction(const FileModule* aModule)
  : FileFunction(aModule)
{
}

ItemSequence_t
ResolvePathFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String lPathStr = getFilePathString( aArgs, 0 );
  String lResult = pathToOSPath(lPathStr);

  return ItemSequence_t(new SingletonItemSequence(theModule->getItemFactory()->createString(lResult)));
}

//*****************************************************************************

PathToNativeFunction::PathToNativeFunction(const FileModule* aModule)
  : FileFunction(aModule)
{
}

ItemSequence_t
PathToNativeFunction::evaluate(
  ExternalFunction::Arguments_t const &args,
  StaticContext const*,
  DynamicContext const* ) const
{
  Item pathItem;
  Iterator_t arg0_iter = args[0]->getIterator();
  arg0_iter->open();
  arg0_iter->next(pathItem);
  arg0_iter->close();
  String const path( pathItem.getStringValue() );
  try {
    String const osPath( fs::normalize_path( path ) );
    return ItemSequence_t(
      new SingletonItemSequence(
        theModule->getItemFactory()->createString( osPath )
      )
    );
  }
  catch ( std::invalid_argument const &e ) {
    throw raiseFileError( "FOFL9999", e.what(), path );
  }
}

//*****************************************************************************

PathToUriFunction::PathToUriFunction(const FileModule* aModule)
  : FileFunction(aModule)
{
}

ItemSequence_t
PathToUriFunction::evaluate(
  ExternalFunction::Arguments_t const &aArgs,
  StaticContext const*,
  DynamicContext const* ) const
{
  String lPathStr = getFilePathString( aArgs, 0 );
  String lResult = pathToUriString(lPathStr);

  return ItemSequence_t(new SingletonItemSequence(theModule->getItemFactory()->createAnyURI(lResult)));
}

//*****************************************************************************

WriteTextFunction::WriteTextFunction(const FileModule* aModule)
  : WriterFileFunction(aModule)
{
}

bool
WriteTextFunction::isAppend() const {
  return false;
}

bool
WriteTextFunction::isBinary() const {
  return false;
}

//*****************************************************************************

WriteBinaryFunction::WriteBinaryFunction(const FileModule* aModule)
  : WriterFileFunction(aModule)
{
}

bool
WriteBinaryFunction::isAppend() const {
  return false;
}

bool
WriteBinaryFunction::isBinary() const {
  return true;
}

//*****************************************************************************

AppendTextFunction::AppendTextFunction(const FileModule* aModule)
  : WriterFileFunction(aModule)
{
}

bool
AppendTextFunction::isAppend() const {
  return true;
}

bool
AppendTextFunction::isBinary() const {
  return false;
}

//*****************************************************************************

AppendBinaryFunction::AppendBinaryFunction(const FileModule* aModule)
  : WriterFileFunction(aModule)
{
}

bool
AppendBinaryFunction::isAppend() const {
  return true;
}

bool
AppendBinaryFunction::isBinary() const {
  return true;
}

//*****************************************************************************


} // namespace filemodule
} // namespace zorba

#ifdef WIN32
#  define DLL_EXPORT __declspec(dllexport)
#else
#  define DLL_EXPORT __attribute__ ((visibility("default")))
#endif

extern "C" DLL_EXPORT zorba::ExternalModule* createModule() {
  return new zorba::filemodule::FileModule();
}
/* vim:set et sw=2 ts=2: */
