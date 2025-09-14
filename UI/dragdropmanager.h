#ifndef DRAGDROPMANAGER_H
#define DRAGDROPMANAGER_H

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFrame>
#include <QObject>
#include <QPoint>
#include <QVBoxLayout>
#include <QWidget>
#include <textbuttonedit.h>
#include <textfactory.h>

class DragDropManager : public QObject
{
  Q_OBJECT
public:
  explicit DragDropManager(QWidget * target, QVBoxLayout * layout, TextFactory * factory, QObject * parent = nullptr);
  ~DragDropManager();

protected slots:
  bool eventFilter(QObject * watched, QEvent * event) override;
  void handleDragEnter(QDragEnterEvent * event);
  void handleDragMove(QDragMoveEvent * event);
  void handleDrop(QDropEvent * event);
  void handleDragLeave(QDragLeaveEvent * event);

private slots:
  int getDropIndex(const QPoint & localPos);

private:
  QWidget *     targetWidget;
  QVBoxLayout * mainLayout;
  TextFactory * textFactory;
  QFrame *      dropPlaceholder;
};

#endif // DRAGDROPMANAGER_H
