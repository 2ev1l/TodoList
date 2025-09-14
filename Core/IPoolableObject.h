#ifndef IPOOLABLEOBJECT_H
#define IPOOLABLEOBJECT_H

class IPoolableObject
{
public:
  IPoolableObject() = default;
  virtual ~IPoolableObject() = default;

  virtual void hideObject() = 0;
  virtual void showObject() = 0;

  virtual bool isObjectUsing() const = 0;
  virtual void setUsing(const bool & state) = 0;

};

#endif // IPOOLABLEOBJECT_H
