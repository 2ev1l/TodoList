#ifndef IREADWRITEABLE_H
#define IREADWRITEABLE_H

#include <QJsonObject>

class IReadWriteable
{
public:
  virtual ~IReadWriteable() = default;

  virtual void read(const QJsonObject & json)  = 0;
  virtual void write(QJsonObject & json) const = 0;
};

#endif // IREADWRITEABLE_H
