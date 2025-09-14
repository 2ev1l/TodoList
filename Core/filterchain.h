// filterchain.h

#ifndef FILTERCHAIN_H
#define FILTERCHAIN_H

#include <IFilter.h>
#include <IFiltered.h>
#include <QList>

class FilterChain
{
public:
  explicit FilterChain(std::initializer_list<IFilter *> filters);

  template <typename Container> void apply(const Container & items)
  {
    for (auto * item : items)
    {
      IFiltered * filteredItem = static_cast<IFiltered *>(item);

      bool isVisible = true;
      for (const IFilter * filter : std::as_const(m_filters))
      {
        if (filter->isActive())
        {
          if (!filter->evaluate(filteredItem))
          {
            isVisible = false;
            break;
          }
        }
      }
      filteredItem->ApplyFilter(isVisible);
    }
  }

private:
  QList<IFilter *> m_filters;
};

#endif // FILTERCHAIN_H
