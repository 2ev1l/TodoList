#ifndef IDATETIMEDATAPROVIDER_H
#define IDATETIMEDATAPROVIDER_H

#include <QDateTime>

class IDateTimeDataProvider
{
public:
  virtual ~IDateTimeDataProvider() = default;
  virtual QDateTime getDateTime() const = 0;
};

#endif // IDATETIMEDATAPROVIDER_H
