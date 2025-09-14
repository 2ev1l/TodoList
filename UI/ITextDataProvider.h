#ifndef ITEXTDATAPROVIDER_H
#define ITEXTDATAPROVIDER_H

#include <QString>

class ITextDataProvider
{
public:
  virtual ~ITextDataProvider() = default;
  virtual QString getText() const = 0;
};

#endif // ITEXTDATAPROVIDER_H
