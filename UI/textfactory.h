#ifndef TEXTFACTORY_H
#define TEXTFACTORY_H

#include <QDateTime>
#include <QFrame>
#include <QObject>
#include <QStack>
#include <QTextCursor>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <markdownhighlighter.h>
#include <objectpool.h>
#include <textbuttonedit.h>
#include <textbuttoneditdata.h>

class TextFactory : public QObject
{
  Q_OBJECT;

public:
  explicit TextFactory(QVBoxLayout * layout);
  ~TextFactory();

  int widgetCount();
  const QList<TextButtonEdit *> & getActiveWidgets() const;

public slots:
  void createText(const TextButtonEditData & data);
  void createTextList(const QList<TextButtonEditData> & dataList);

  void removeText();
  void removeText(int id);
  void removeText(TextButtonEdit * widget);
  void removeAllText();
  void moveText(TextButtonEdit * widget, int toIndex);

  QList<TextButtonEditData> getText();

private slots:
  void updateTextHeight(TextButtonEdit * textButtonEdit);
  void firstCreateText(const char * initializedProperty, TextButtonEdit * textButtonEdit);

private:
  QVBoxLayout *           layout;
  ObjectPool<TextButtonEdit> textPool;
  QList<TextButtonEdit *> activeWidgets;

signals:
};

#endif // TEXTFACTORY_H
