#ifndef DATETIMEFILTER_H
#define DATETIMEFILTER_H

#include <IFilter.h>
#include <IFiltered.h>
#include <IDateTimeDataProvider.h>
#include <QDateTime>

class DateTimeFilter : public IFilter
{
public:
  DateTimeFilter(bool isActive = false) : IFilter(isActive) {}

  bool evaluate(const IFiltered* item) const override;

  void setDateTimeRange(const QDateTime & start, const QDateTime & end);

private:
  QDateTime start;
  QDateTime end;
};

#endif // DATETIMEFILTER_H
