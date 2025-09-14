#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <IPoolableObject.h>
#include <QList>
#include <memory>
#include <type_traits>
#include <vector>

template <typename T> class ObjectPool
{
public:
  static_assert(std::is_base_of_v<IPoolableObject, T>,
                "Template parameter T must implement the IPoolableObject interface.");
  static_assert(!std::is_abstract_v<T>, "Template parameter T cannot be an abstract class.");

  ObjectPool() = default;

  ~ObjectPool() = default;

  ObjectPool(const ObjectPool &)             = delete;
  ObjectPool & operator=(const ObjectPool &) = delete;

public:
  /**
   * @brief Устанавливает фабричный метод для новых объектов.
   */
  void setFactory(std::function<std::unique_ptr<T>()> factoryFunc)
  {
    factory = std::move(factoryFunc);
  }

  /**
   * @brief Получает свободный объект из пула или создает новый.
   * @return Не-владеющий сырой указатель на объект. Владение остается у пула.
   */
  T * getObject()
  {
    T * object = findFirstInactiveObject();
    if (object == nullptr)
    {
      object = getNewObject();
    }
    object->setUsing(true);
    return object;
  }

  /**
   * @brief Возвращает объект в пул, делая его доступным для повторного использования.
   * @param object Указатель на объект, который больше не используется.
   */
  void releaseObject(T * object)
  {
    if (object)
    {
      object->setUsing(false);
    }
  }

  /**
   * @brief Возвращает список не-владеющих указателей на все объекты в пуле.
   * @return QList<T*> со всеми объектами.
   */
  QList<T *> getAllObjects() const
  {
    QList<T *> rawPointers;
    rawPointers.reserve(objects.size());
    for (const auto & ptr : objects)
    {
      rawPointers.append(ptr.get());
    }
    return rawPointers;
  }

  int size() const
  {
    return objects.size();
  }

private:
  /**
   * @brief Создает новый объект, добавляет его в пул и возвращает указатель на него.
   *        Владение объектом передается пулу.
   */
  T * getNewObject()
  {
    if (!factory)
    {
      throw std::runtime_error("ObjectPool factory is not set");
    }
    auto newObjectPtr = factory();
    T *  rawPtr       = newObjectPtr.get();
    objects.emplace_back(std::move(newObjectPtr));
    return rawPtr;
  }

  /**
   * @brief Ищет первый неиспользуемый объект в пуле.
   * @return Не-владеющий указатель на объект или nullptr, если все заняты.
   */
  T * findFirstInactiveObject() const
  {
    for (const auto & el : objects)
    {
      if (!el->isObjectUsing())
        return el.get();
    }
    return nullptr;
  }

private:
  std::vector<std::unique_ptr<T>>     objects;
  std::function<std::unique_ptr<T>()> factory;
};

#endif // OBJECTPOOL_H
