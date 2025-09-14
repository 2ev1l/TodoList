#include "textfilter.h"

bool TextFilter::evaluate(const IFiltered * item) const
{
  const auto * provider = dynamic_cast<const ITextDataProvider *>(item);

  if (!provider)
  {
    return true;
  }

  QString text = provider->getText();
  return text.contains(searchText, Qt::CaseInsensitive);
}

void TextFilter::setSearchText(const QString & text)
{
  searchText = text;
}
