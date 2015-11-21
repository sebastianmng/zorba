/*
 * Copyright 2015 Federico Cavalieri.
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
#include <cstring>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "io/response.h"
#include "exceptions/server_exceptions.h"
#include "util/uri_util.h"

#include "request.h"

namespace zorba
{
namespace server
{
namespace io
{

Request::Request(const FCGX_Request& aRequest):
    theBodyRead(false),
    theParametersRead(false)
{
  initEnvironment(aRequest);

  const std::string* lRequestURI = getEnvironmentVariable("SCRIPT_NAME");
  if (!lRequestURI)
    throw new exceptions::ServerException("Invalid request, missing REQUEST_URI", 500);
  theURI = *lRequestURI;

  const std::string* lRequestMethod = getEnvironmentVariable("REQUEST_METHOD");
  if (!lRequestMethod)
    throw new exceptions::ServerException("Invalid request, missing REQUEST_METHOD", 500);
  theRequestMethod = *lRequestMethod;

  const std::string* lQueryString = getEnvironmentVariable("QUERY_STRING");
  if (!lQueryString)
    throw new exceptions::ServerException("Invalid request, missing QUERY_STRING", 500);
  theQueryString = *lQueryString;

  std::string lTmpRequestURI = boost::algorithm::trim_copy_if(theURI, boost::is_any_of("/"));
  boost::algorithm::split(theURISegments, lTmpRequestURI, boost::is_any_of("/"));
}

void Request::initEnvironment(const FCGX_Request& aRequest)
{
  for (char** lParam = aRequest.envp; *lParam; ++lParam)
  {
    char* lSepPos = strstr(*lParam, "=");
    if (lSepPos)
    {
      theEnvironment[std::string(*lParam, lSepPos - *lParam)] = std::string(lSepPos + 1);
    }
  }
}

void Request::initParameters() const
{
  parseParameters(theQueryString);

  if (theRequestMethod == "POST" || theRequestMethod == "PUT" || theRequestMethod == "PATCH")
  {
    const std::string* lContentType = getEnvironmentVariable("CONTENT_TYPE");
    if (lContentType && ContentTypes::isX_WWW_FORM_URLENCODED(*lContentType))
    {
      parseParameters(getBody());
    }
  }
  theParametersRead = true;
}

void Request::parseParameters(const std::string& aString) const
{
  if (aString.length() == 0)
    return;

  std::string::size_type lNextTokenStart = 0;
  std::string::size_type lSepPos = 0;
  while (lNextTokenStart <= aString.length())
  {
    if (lNextTokenStart == aString.length())
    {
      //Empty parameter
      addParameter("", "");
      break;
    }
    lSepPos = aString.find('&', lNextTokenStart);

    if (lSepPos == lNextTokenStart)
      addParameter("", "");
    else if (lSepPos < std::string::npos)
      parseParameter(aString, lNextTokenStart, lSepPos);
    else
    {
      parseParameter(aString, lNextTokenStart, aString.length());
      break;
    }
    lNextTokenStart = lSepPos + 1;
  }
}

void Request::parseParameter(const std::string& aString, std::string::size_type lStart, std::string::size_type lEnd) const
{
  std::string::size_type lEqualPos = aString.find('=', lStart);
  if (lEqualPos >= lEnd)
  {
    std::string lName(aString, lStart, lEnd - lStart);
    uri::decode(lName);
    addParameter(lName, "");
  }
  else if (lEqualPos == (lEnd - 1))
  {
    std::string lName(aString, lStart, lEqualPos - lStart);
    uri::decode(lName);
    addParameter(lName, "");
  }
  else
  {
    std::string lName(aString, lStart, lEqualPos - lStart);
    uri::decode(lName);
    std::string lValue(aString, lEqualPos + 1, lEnd - (lEqualPos +1));
    uri::decode(lValue);
    addParameter(lName, lValue);
  }
}

void Request::addParameter(const std::string& aName, const std::string& aValue) const
{
  std::map<std::string, std::vector<std::string> >::iterator lIt =
      theParameters.find(aName);

  if (lIt != theParameters.end())
    lIt->second.push_back(aValue);
  else
  {
    std::vector<std::string> lValue;
    lValue.push_back(aValue);
    theParameters[aName] = lValue;
  }
}

const std::string& Request::getBody() const
{
  if (!theBodyRead)
    readBody();

  return theBody;
}

void Request::readBody() const
{
  std::stringstream lBody;
  lBody << std::cin.rdbuf();
  theBody = lBody.str();
  theBodyRead = true;
}

const std::string* Request::getEnvironmentVariable(const std::string& aName) const
{
  std::map<std::string, std::string>::const_iterator lIt = theEnvironment.find(aName);
  if (lIt != theEnvironment.end())
    return &(lIt->second);
  return NULL;
}

const std::vector<std::string>* Request::getQueryParameter(const std::string& aName) const
{
  if (!theParametersRead)
    initParameters();

  std::map<std::string, std::vector<std::string> >::const_iterator lIt = theParameters.find(aName);
  if (lIt != theParameters.end())
    return &(lIt->second);
  return NULL;
}

bool Request::getQueryParameterAsString(const std::string& aName, std::string& aValue, bool aMandatory) const
{
  const std::vector<std::string>* lValue = getQueryParameter(aName);
  if (!lValue)
    if (aMandatory)
      throw exceptions::ServerException("Missing required parameter " + aName, 400);
    else
      return false;
  else
    if (lValue->size() == 0)
      return true;
    else if (lValue->size() > 1)
      throw exceptions::ServerException("A single value is expected for parameter " + aName, 400);
    else
    {
      aValue = (*lValue)[0];
      return true;
    }
}

bool Request::getQueryParameterAsBoolean(const std::string& aName, bool& aValue, bool aMandatory) const
{
  std::string lValue;
  bool lHaveParameter = getQueryParameterAsString(aName, lValue, aMandatory);
  if (lHaveParameter)
  {
    if (lValue.empty())
      return true;
    else
    {
      std::string lLowerCaseValue(lValue);
      std::transform(lLowerCaseValue.begin(), lLowerCaseValue.end(), lLowerCaseValue.begin(), ::tolower);
      if (lLowerCaseValue == "false")
      {
        aValue = false;
        return true;
      }
      else if (lLowerCaseValue == "true")
      {
        aValue = true;
        return true;
      }
      else
      {
        throw exceptions::ServerException("Allowed values for parameter " + aName + " are: <true>, <false>, <>.", 400);
      }
    }
  }
  else
    return false;

}

const std::string* Request::getContentType() const
{
  return getEnvironmentVariable("CONTENT_TYPE");
}

const std::string& Request::getRequestMethod() const
{
  return theRequestMethod;
}

const std::string& Request::getRequestURI() const
{
  return theURI;
}

const std::vector<std::string> Request::getRequestURISegments() const
{
  return theURISegments;
}

const std::map<std::string, std::vector<std::string> >& Request::getQueryParameters() const
{
  initParameters();
  return theParameters;
}

std::ostream & operator<<(std::ostream &aOs, const Request& aRequest)
{
  aOs << "===Environment===" << std::endl;
  for (std::map<std::string, std::string>::const_iterator lIt = aRequest.theEnvironment.begin();
       lIt != aRequest.theEnvironment.end();
       ++lIt)
  {
    aOs << lIt->first << "=" << lIt->second << std::endl;
  }

  const std::map<std::string, std::vector<std::string> >& lParameters = aRequest.getQueryParameters();
  aOs << std::endl << "===Query Parameters===" << std::endl;
  for (std::map<std::string, std::vector<std::string> >::const_iterator lIt = lParameters.begin();
      lIt != lParameters.end();
      ++lIt)
  {
    aOs << "<" << lIt->first << ">=<" << boost::algorithm::join(lIt->second, ", ") << ">" << std::endl;
  }
  aOs << std::endl << "===Body===" << std::endl;
  aOs << aRequest.getBody();
  return aOs;
}

}
}
}