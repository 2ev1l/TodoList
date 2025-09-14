#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QString>
#include <IReadWriteable.h>

namespace JsonSerializer
{
  template <typename T> static void readField(const QJsonObject & json, const QString & key, T & value)
  {
    if (json.contains(key))
    {
      QVariant variant = json.value(key).toVariant();

      if (variant.canConvert<T>())
      {
        value = variant.value<T>();
      }
      else
      {
        qWarning() << "Could not convert JSON value for key '" << key << "' to the requested type.";
      }
    }
    else
    {
      qWarning() << "Key '" << key << "' not found in JSON object.";
    }
  }

  template <typename T> static void writeField(QJsonObject & json, const QString & key, const T & value)
  {
    json[key] = QJsonValue::fromVariant(QVariant::fromValue(value));
  }

  /**
   * @brief Записывает один объект, реализующий IReadWriteable, в QJsonObject.
   * @param object - Объект для записи.
   * @return QJsonObject с данными объекта.
   */
  template <typename T> static QJsonObject writeObject(const T & object)
  {
    static_assert(std::is_base_of<IReadWriteable, T>::value, "T must inherit from IReadWriteable");

    QJsonObject jsonObj;
    object.write(jsonObj);
    return jsonObj;
  }

  /**
   * @brief Записывает список объектов в QJsonArray и помещает его в родительский QJsonObject.
   * @param json - Родительский объект JSON, в который будет произведена запись.
   * @param key - Ключ, по которому будет добавлен массив.
   * @param list - Список объектов для записи.
   */
  template <typename T> static void writeObjectList(QJsonObject & json, const QString & key, const QList<T> & list)
  {
    static_assert(std::is_base_of<IReadWriteable, T>::value, "T must inherit from IReadWriteable");

    QJsonArray jsonArray;
    for (const T & item : list)
    {
      jsonArray.append(writeObject(item));
    }
    json[key] = jsonArray;
  }

  /**
   * @brief Читает один объект из QJsonObject.
   * @param json - JSON-объект с данными.
   * @return Новый объект типа T, заполненный данными.
   */
  template <typename T> static T readObject(const QJsonObject & json)
  {
    static_assert(std::is_base_of<IReadWriteable, T>::value, "T must inherit from IReadWriteable");
    static_assert(std::is_default_constructible<T>::value, "T must be default constructible");

    T object;
    object.read(json);
    return object;
  }

  /**
   * @brief Читает список объектов из QJsonArray, находящегося в родительском QJsonObject.
   * @param json - Родительский объект JSON, из которого будет произведено чтение.
   * @param key - Ключ, по которому находится массив.
   * @return Список QList<T> с прочитанными объектами.
   */
  template <typename T> static QList<T> readObjectList(const QJsonObject & json, const QString & key)
  {
    static_assert(std::is_base_of<IReadWriteable, T>::value, "T must inherit from IReadWriteable");
    static_assert(std::is_default_constructible<T>::value, "T must be default constructible");

    QList<T> list;
    if (json.contains(key) && json[key].isArray())
    {
      QJsonArray jsonArray = json[key].toArray();
      list.reserve(jsonArray.size());
      for (const QJsonValue & value : std::as_const(jsonArray))
      {
        list.append(readObject<T>(value.toObject()));
      }
    }
    return list;
  }
}
#endif // JSONSERIALIZER_H
