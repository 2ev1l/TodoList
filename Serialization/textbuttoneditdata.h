#ifndef TEXTBUTTONEDITDATA_H
#define TEXTBUTTONEDITDATA_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <JsonSerializer.h>
#include <IReadWriteable.h>

class TextButtonEditData : public IReadWriteable
{
public:
  TextButtonEditData();
  TextButtonEditData(const QString & header, const QString & main, const QDateTime & dateTime = QDateTime::currentDateTime());

  QString                       headerText;
  static constexpr const char * headerTextKey = "headerText";
  QString                       mainText;
  static constexpr const char * mainTextKey = "mainText";
  QDateTime                     dateTime;
  static constexpr const char * dateTimeKey = "dateTime";

public slots:
  void read(const QJsonObject & json) override;
  void write(QJsonObject & json) const override;
};

#endif // TEXTBUTTONEDITDATA_H
