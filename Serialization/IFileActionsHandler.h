#ifndef IFILEACTIONSHANDLER_H
#define IFILEACTIONSHANDLER_H

class IFileActionsHandler
{
public:
  virtual ~IFileActionsHandler() = default;

  virtual void onSaveTriggered() = 0;
  virtual void onLoadTriggered() = 0;
};

#endif // IFILEACTIONSHANDLER_H
