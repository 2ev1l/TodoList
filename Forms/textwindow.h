#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

#include <QApplication>
#include <QDateTime>
#include <QFrame>
#include <QMainWindow>
#include <QMessageBox>
#include <contextmenuactions.h>
#include <dragdropmanager.h>
#include <IFileActionsHandler.h>
#include <jsondataserializer.h>
#include <textfactory.h>
#include <datetimefilter.h>
#include <filterchain.h>
#include <textfilter.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
  class TextWindow;
}
QT_END_NAMESPACE

class TextWindow : public QMainWindow, public IFileActionsHandler
{
  Q_OBJECT

public:
  TextWindow(QWidget * parent = nullptr);
  ~TextWindow();

  void onSaveTriggered() override;
  void onLoadTriggered() override;

private slots:
  void on_createButton_clicked();
  void on_headerSearch_textEdited(const QString & arg1);

  void on_timeFilterCheckBox_checkStateChanged(const Qt::CheckState & arg1);
  void applyFilters();

  void onBoldFormatTriggered();
  void onItalicFormatTriggered();
  void onStrikeFormatTriggered();
  void onCodeFormatTriggered();
  void onHeaderFormatTriggered();
  void onFormatTriggered(const QString & format);

  void             setupFormattingControls();
  TextButtonEdit * findFocusedTextButtonEdit() const;

  void on_boldFormatterButton_clicked();
  void on_italicFormatterButton_clicked();
  void on_strikeFormatterButton_clicked();
  void on_codeFormatterButton_clicked();
  void on_headerFormatterButton_clicked();

private:
  Ui::TextWindow *     ui;
  TextFactory *        textFactory = nullptr;
  DragDropManager *    dndManager  = nullptr;
  ContextMenuActions * menuActions = nullptr;

  FilterChain *    filterChain    = nullptr;
  TextFilter *     textFilter     = nullptr;
  DateTimeFilter * dateTimeFilter = nullptr;

  QAction * boldAction;
  QAction * italicAction;
  QAction * strikeAction;
  QAction * codeAction;
  QAction * headerAction;
};
#endif // TEXTWINDOW_H
