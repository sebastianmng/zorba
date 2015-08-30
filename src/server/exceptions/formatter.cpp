#include "formatter.h"

namespace zorba
{
namespace server
{
namespace exceptions
{

void Formatter::formatException(const std::exception& aException, zorba::Item& aFormattedException, int& aStatusCode)
{
  utils::JSONObject lFormattedException;

  if (const zorba::ZorbaException* lZorbaException =
        dynamic_cast<const zorba::ZorbaException*>(&aException))
  {
    Formatter::formatZorbaException(*lZorbaException, lFormattedException, aStatusCode);

  }
  else if (const exceptions::ServerException* lSException =
        dynamic_cast<const exceptions::ServerException*>(&aException))
  {
    Formatter::formatServerException(*lSException, lFormattedException, aStatusCode);
  }
  else
  {
    lFormattedException.addField("source", "Internal Error");
    lFormattedException.addField("code", "INT0001");
    lFormattedException.addField("message", aException.what());
    aStatusCode = 500;
  }

  aFormattedException = lFormattedException.getAsItem();
}

void Formatter::formatZorbaException(const zorba::ZorbaException& aZorbaException, utils::JSONObject& aFormattedException, int& aStatusCode)
{
  aFormattedException.addField("source", "Zorba");

  const zorba::Diagnostic& lDiag = aZorbaException.diagnostic();

  std::string lCode = (lDiag.qname().ns() == NULL ? "" : std::string(lDiag.qname().ns()) + ":") + lDiag.qname().localname();
  aFormattedException.addField("code", lCode);

  const zorba::XQueryException* lXQueryException =
      dynamic_cast<const zorba::XQueryException*>(&aZorbaException);
  if (lXQueryException)
  {
    if (lXQueryException->has_source())
    {
      utils::JSONObject lLocation;
      lLocation.addField("module", lXQueryException->source_uri());
      lLocation.addField("lineNumber", (long)lXQueryException->source_line());
      lLocation.addField("columnNumber", (long)lXQueryException->source_column());
      lLocation.addField("lineNumberEnd", (long)lXQueryException->source_line_end());
      lLocation.addField("columnNumberEnd", (long)lXQueryException->source_column_end());
      aFormattedException.addField("location", lLocation);
    }
  }

  aFormattedException.addField("message", aZorbaException.what());

  aStatusCode = 500;
}

void Formatter::formatServerException(const exceptions::ServerException& aServerException, utils::JSONObject& aFormattedException, int& aStatusCode)
{
  aFormattedException.addField("source", "Server API");
  aFormattedException.addField("message", aServerException.what());

  aStatusCode = aServerException.getStatusCode();
}

}
}
}
