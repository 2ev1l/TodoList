#ifndef IFILTERED_H
#define IFILTERED_H

class IFiltered
{
public:
  virtual ~IFiltered() = default;
  virtual void ApplyFilter(bool isVisible) = 0;
};

#endif // IFILTERED_H
