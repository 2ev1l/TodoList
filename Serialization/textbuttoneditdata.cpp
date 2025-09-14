#include "textbuttoneditdata.h"

TextButtonEditData::TextButtonEditData()
{
}

TextButtonEditData::TextButtonEditData(const QString & header, const QString & main, const QDateTime & dateTime)
    : headerText(header), mainText(main), dateTime(dateTime)
{
}

void TextButtonEditData::read(const QJsonObject & json)
{
  JsonSerializer::readField(json, headerTextKey, headerText);
  JsonSerializer::readField(json, mainTextKey, mainText);
  JsonSerializer::readField(json, dateTimeKey, dateTime);
}

void TextButtonEditData::write(QJsonObject & json) const
{
  JsonSerializer::writeField(json, headerTextKey, headerText);
  JsonSerializer::writeField(json, mainTextKey, mainText);
  JsonSerializer::writeField(json, dateTimeKey, dateTime);
}
