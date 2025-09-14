#include "poolablewidget.h"

PoolableWidget::PoolableWidget(QWidget * parent) : QWidget{parent}
{
}

void PoolableWidget::hideObject()
{
  setVisible(false);
}

void PoolableWidget::showObject()
{
  setVisible(true);
}

bool PoolableWidget::isObjectUsing() const
{
  return isUsing;
}

void PoolableWidget::setUsing(const bool & state)
{
  isUsing = state;
  if (isUsing)
  {
    showObject();
  }
  else
  {
    hideObject();
  }
}
