#ifndef DATACLASS_H
#define DATACLASS_H

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <JsonSerializer.h>
#include <textbuttoneditdata.h>

class DataClass
{
public:
  DataClass();
  ~DataClass();

public slots:
  QList<TextButtonEditData> getItems();
  void                      setItems(const QList<TextButtonEditData> & newItems);

  void read(const QJsonObject & json);
  void write(QJsonObject & json) const;

private:
  QList<TextButtonEditData>     items;
  static constexpr const char * itemsKey = "items";

private slots:
};

#endif // DATACLASS_H
