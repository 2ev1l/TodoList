#include "datetimefilter.h"

bool DateTimeFilter::evaluate(const IFiltered* item) const
{
  const auto * provider = dynamic_cast<const IDateTimeDataProvider*>(item);
  if (!provider)
    {
      return true;
    }

  QDateTime widgetDateTime = provider->getDateTime();
  return (widgetDateTime >= start && widgetDateTime <= end);
}

void DateTimeFilter::setDateTimeRange(const QDateTime & start, const QDateTime & end)
{
  this->start = start;
  this->end = end;
}
