#include "contextmenuactions.h"

ContextMenuActions::ContextMenuActions(QMainWindow * menuParent, IFileActionsHandler * actionHandler)
    : QObject(menuParent)
{
  createActions(menuParent, actionHandler);
}

void ContextMenuActions::createActions(QMainWindow * menuParent, IFileActionsHandler * actionHandler)
{
  auto handlerObject = dynamic_cast<QObject *>(actionHandler);
  if (!handlerObject)
  {
    return;
  }

  fileMenu = menuParent->menuBar()->addMenu(tr("&File"));

  saveAction = new QAction(tr("&Save"), menuParent);
  saveAction->setShortcut(QKeySequence::Save);

  loadAction = new QAction(tr("&Load"), menuParent);
  loadAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));

  fileMenu->addAction(saveAction);
  fileMenu->addAction(loadAction);

  connect(saveAction, &QAction::triggered, handlerObject, [actionHandler]() { actionHandler->onSaveTriggered(); });
  connect(loadAction, &QAction::triggered, handlerObject, [actionHandler]() { actionHandler->onLoadTriggered(); });
}
