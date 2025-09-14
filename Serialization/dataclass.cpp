#include "dataclass.h"

DataClass::DataClass()
{
}

DataClass::~DataClass()
{
}

QList<TextButtonEditData> DataClass::getItems()
{
  return items;
}

void DataClass::setItems(const QList<TextButtonEditData> & newItems)
{
  items = newItems;
}

void DataClass::read(const QJsonObject & json)
{
  items = JsonSerializer::readObjectList<TextButtonEditData>(json, itemsKey);
}

void DataClass::write(QJsonObject & json) const
{
  JsonSerializer::writeObjectList<TextButtonEditData>(json, itemsKey, items);
}
