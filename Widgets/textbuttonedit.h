#ifndef TEXTBUTTONEDIT_H
#define TEXTBUTTONEDIT_H

#include <QApplication>
#include <QDateEdit>
#include <QDrag>
#include <QEvent>
#include <QHBoxLayout>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QString>
#include <QTextCursor>
#include <QTextEdit>
#include <QTimeEdit>
#include <QWidget>
#include <poolablewidget.h>
#include <IDateTimeDataProvider.h>
#include <IFiltered.h>
#include <ITextDataProvider.h>

class TextButtonEdit : public PoolableWidget, public IFiltered, public ITextDataProvider, public IDateTimeDataProvider
{
  Q_OBJECT
public:
  explicit TextButtonEdit(QWidget * parent = nullptr);
  ~TextButtonEdit();

  QPushButton * button;
  QTextEdit *   textEdit;
  QTextEdit *   headerTextEdit;
  QPushButton * dragButton;
  QDateEdit *   dateEdit;
  QTimeEdit *   timeEdit;

  static const QString dragEventName;

  void      ApplyFilter(bool isVisible) override;
  QString   getText() const override;
  QDateTime getDateTime() const override;

public slots:
  void applyFormatting(const QString & prefix, const QString & suffix);

signals:
  void buttonClicked();

protected:
  bool eventFilter(QObject * watched, QEvent * event) override;

private:
  QPoint dragStartPosition;
};

#endif // TEXTBUTTONEDIT_H
