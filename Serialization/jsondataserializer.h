#ifndef JSONDATASERIALIZER_H
#define JSONDATASERIALIZER_H

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QString>
#include <dataclass.h>
#include <QStandardPaths>
#include <QObject>

class JsonDataSerializer
{
public:
  JsonDataSerializer();
  ~JsonDataSerializer();

public slots:
  static bool        save();
  static bool        load();
  static DataClass * data;

private slots:
  static QString GetFilename();
};

#endif // JSONDATASERIALIZER_H
