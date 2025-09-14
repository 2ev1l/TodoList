#ifndef CONTEXTMENUACTIONS_H
#define CONTEXTMENUACTIONS_H

#include <QAction>
#include <QKeySequence>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <IFileActionsHandler.h>

class ContextMenuActions : public QObject
{
  Q_OBJECT
public:
  explicit ContextMenuActions(QMainWindow * menuParent, IFileActionsHandler * actionHandler);

private:
  void createActions(QMainWindow * menuParent, IFileActionsHandler * actionHandler);

  QMenu *   fileMenu;
  QAction * saveAction;
  QAction * loadAction;
};

#endif // CONTEXTMENUACTIONS_H
