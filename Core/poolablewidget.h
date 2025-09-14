#ifndef POOLABLEWIDGET_H
#define POOLABLEWIDGET_H

#include <QWidget>
#include <IPoolableObject.h>

class PoolableWidget : public QWidget, IPoolableObject
{
  Q_OBJECT
public:
  explicit PoolableWidget (QWidget *parent = nullptr);

  void hideObject() override;
  void showObject() override;

  bool isObjectUsing() const override;
  void setUsing(const bool & state) override;

private:
  bool isUsing = false;
};

#endif // POOLABLEWIDGET_H
