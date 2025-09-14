#include "dragdropmanager.h"

DragDropManager::DragDropManager(QWidget * target, QVBoxLayout * layout, TextFactory * factory, QObject * parent)
    : QObject(parent), targetWidget(target), mainLayout(layout), textFactory(factory)
{
  targetWidget->installEventFilter(this);
  targetWidget->setAcceptDrops(true);

  QWidget * layoutParent = mainLayout->parentWidget();
  dropPlaceholder        = new QFrame(layoutParent);
  dropPlaceholder->setStyleSheet("border-top: 2px dashed #000;");
  dropPlaceholder->hide();
}

DragDropManager::~DragDropManager()
{
  delete dropPlaceholder;
}

bool DragDropManager::eventFilter(QObject * watched, QEvent * event)
{
  if (watched != targetWidget)
  {
    return QObject::eventFilter(watched, event);
  }

  switch (event->type())
  {
    case QEvent::DragEnter:
      handleDragEnter(static_cast<QDragEnterEvent *>(event));
      return true;
    case QEvent::DragMove:
      handleDragMove(static_cast<QDragMoveEvent *>(event));
      return true;
    case QEvent::Drop:
      handleDrop(static_cast<QDropEvent *>(event));
      return true;
    case QEvent::DragLeave:
      handleDragLeave(static_cast<QDragLeaveEvent *>(event));
      return true;
    default:
      break;
  }

  return QObject::eventFilter(watched, event);
}

int DragDropManager::getDropIndex(const QPoint & localPos)
{
  int drop = textFactory->widgetCount();

  for (int i = 0; i < textFactory->widgetCount(); ++i)
  {
    QWidget * widget = mainLayout->itemAt(i)->widget();
    if (!widget || !widget->isVisible())
    {
      continue;
    }
    if (localPos.y() < widget->geometry().center().y())
    {
      drop = i;
      break;
    }
  }
  return drop;
}

void DragDropManager::handleDragEnter(QDragEnterEvent * event)
{
  TextButtonEdit * sourceWidget = qobject_cast<TextButtonEdit *>(event->source());
  if (sourceWidget && mainLayout->indexOf(sourceWidget) != -1)
  {
    event->acceptProposedAction();
    dropPlaceholder->show();
    dropPlaceholder->raise();
  }
  else
  {
    event->ignore();
  }
}

void DragDropManager::handleDragMove(QDragMoveEvent * event)
{
  if (!event->mimeData()->hasFormat(TextButtonEdit::dragEventName))
  {
    event->ignore();
    return;
  }

  QWidget * layoutParent = mainLayout->parentWidget();
  QPoint    localPos     = layoutParent->mapFrom(targetWidget, event->position().toPoint());

  int dropIndex = getDropIndex(localPos);

  int insertY;
  if (dropIndex < textFactory->widgetCount())
  {
    insertY = mainLayout->itemAt(dropIndex)->widget()->y();
  }
  else
  {
    if (textFactory->widgetCount() == 0)
    {
      insertY = 0;
    }
    else
    {
      QWidget * last = mainLayout->itemAt(textFactory->widgetCount() - 1)->widget();
      insertY        = last->y() + last->height();
    }
  }

  int leftMargin  = mainLayout->contentsMargins().left();
  int rightMargin = mainLayout->contentsMargins().right();
  int w           = layoutParent->width() - leftMargin - rightMargin;
  dropPlaceholder->setGeometry(leftMargin, insertY, w, 2);

  event->acceptProposedAction();
}

void DragDropManager::handleDrop(QDropEvent * event)
{
  dropPlaceholder->hide();

  if (!event->mimeData()->hasFormat(TextButtonEdit::dragEventName))
  {
    event->ignore();
    return;
  }

  QByteArray  itemData = event->mimeData()->data(TextButtonEdit::dragEventName);
  QDataStream dataStream(&itemData, QIODevice::ReadOnly);
  qulonglong  ptrValue;
  dataStream >> ptrValue;
  TextButtonEdit * draggedWidget = reinterpret_cast<TextButtonEdit *>(ptrValue);

  if (!draggedWidget)
  {
    event->ignore();
    return;
  }

  QWidget * layoutParent = mainLayout->parentWidget();
  QPoint    localPos     = layoutParent->mapFrom(targetWidget, event->position().toPoint());

  int dropIndex = getDropIndex(localPos);

  textFactory->moveText(draggedWidget, dropIndex);

  event->acceptProposedAction();
}

void DragDropManager::handleDragLeave(QDragLeaveEvent * event)
{
  Q_UNUSED(event);
  dropPlaceholder->hide();
}
