#include "jsondataserializer.h"

DataClass * JsonDataSerializer::data = new DataClass();

JsonDataSerializer::JsonDataSerializer()
{
}

JsonDataSerializer::~JsonDataSerializer()
{
}

QString JsonDataSerializer::GetFilename()
{
  QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

  QDir dir(dataPath);
  if (!dir.exists())
  {
    dir.mkpath(".");
  }

  return dataPath + QDir::separator() + "data.json";
}

bool JsonDataSerializer::save()
{
  QString filename = GetFilename();
  QFile   saveFile(filename);

  if (!saveFile.open(QIODevice::WriteOnly))
  {
    qWarning() << QObject::tr("Couldn't open save file:") << filename;
    return false;
  }

  QJsonObject dataObject;
  data->write(dataObject);

  QJsonDocument saveDoc(dataObject);
  saveFile.write(saveDoc.toJson());
  saveFile.close();

  return true;
}

bool JsonDataSerializer::load()
{
  QString filename = GetFilename();
  QFile   loadFile(filename);

  if (!loadFile.exists())
  {
    qWarning() << "Save file does not exist:" << filename;
    return false;
  }

  if (!loadFile.open(QIODevice::ReadOnly))
  {
    qWarning() << "Couldn't open save file:" << filename;
    return false;
  }

  QByteArray saveData = loadFile.readAll();
  loadFile.close();

  QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
  data->read(loadDoc.object());

  return true;
}
