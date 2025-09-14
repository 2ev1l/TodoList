#ifndef IFILTER_H
#define IFILTER_H

class IFiltered;

class IFilter
{
public:
  virtual ~IFilter() = default;

  virtual bool evaluate(const IFiltered* item) const = 0;

  void setActive(bool active) { m_isActive = active; }
  bool isActive() const { return m_isActive; }

protected:
  explicit IFilter(bool isActive = false) : m_isActive(isActive) {}
  bool m_isActive;
};

#endif // IFILTER_H
