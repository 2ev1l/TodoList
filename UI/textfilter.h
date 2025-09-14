#ifndef TEXTFILTER_H
#define TEXTFILTER_H

#include <IFiltered.h>
#include <IFilter.h>
#include <ITextDataProvider.h>
#include <QString>

class TextFilter : public IFilter
{
public:
  TextFilter(bool isActive = false) : IFilter(isActive)
  {
  }

  bool evaluate(const IFiltered * item) const override;

  void setSearchText(const QString & text);

private:
  QString searchText;
};

#endif // TEXTFILTER_H
