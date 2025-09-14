#include "textfactory.h"
#include "markdownhighlighter.h"
#include <QDateEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTimeEdit>
#include <QTimer>

TextFactory::TextFactory(QVBoxLayout * layout)
{
  this->layout = layout;

  QWidget * parentWidget = layout->parentWidget();
  if (!parentWidget)
  {
    throw std::runtime_error("Layout has no parent widget");
  }

  textPool.setFactory([parentWidget]() { return std::make_unique<TextButtonEdit>(parentWidget); });
}

TextFactory::~TextFactory()
{
}

int TextFactory::widgetCount()
{
  return activeWidgets.size();
}

const QList<TextButtonEdit *> & TextFactory::getActiveWidgets() const
{
  return activeWidgets;
}

void TextFactory::createTextList(const QList<TextButtonEditData> & dataList)
{
  for (const TextButtonEditData & data : dataList)
  {
    createText(data);
  }
}

void TextFactory::createText(const TextButtonEditData & data)
{
  TextButtonEdit * textButtonEdit      = textPool.getObject();
  const char *     initializedProperty = "isInitialized";

  if (!textButtonEdit->property(initializedProperty).toBool())
  {
    firstCreateText(initializedProperty, textButtonEdit);
  }

  textButtonEdit->headerTextEdit->setMarkdown(data.headerText);
  textButtonEdit->textEdit->setMarkdown(data.mainText);
  textButtonEdit->dateEdit->setDate(data.dateTime.date());
  textButtonEdit->timeEdit->setTime(data.dateTime.time());

  layout->insertWidget(layout->count() - 1, textButtonEdit);
  activeWidgets.append(textButtonEdit);

  QTimer::singleShot(0, this, [=]() { updateTextHeight(textButtonEdit); });
}

void TextFactory::firstCreateText(const char * initializedProperty, TextButtonEdit * textButtonEdit)
{
  int           size           = 100;
  QPushButton * button         = textButtonEdit->button;
  QPushButton * dragButton     = textButtonEdit->dragButton;
  QTextEdit *   headerTextEdit = textButtonEdit->headerTextEdit;
  QTextEdit *   textEdit       = textButtonEdit->textEdit;

  button->setFixedSize(size / 2, size / 2);
  button->setText("X");
  dragButton->setFixedSize(size / 2, size / 2);

  headerTextEdit->setPlaceholderText(tr("Enter header..."));
  headerTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  headerTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  headerTextEdit->setFixedHeight(size / 2.5);

  textEdit->setPlaceholderText(tr("Enter text..."));
  textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  new MarkdownHighlighter(textEdit->document());
  new MarkdownHighlighter(headerTextEdit->document());

  connect(textEdit, &QTextEdit::textChanged, this, [=]() { updateTextHeight(textButtonEdit); });
  connect(textButtonEdit->button, &QPushButton::clicked, this,
          [this, textButtonEdit]() { removeText(textButtonEdit); });

  textButtonEdit->setProperty(initializedProperty, true);
}

void TextFactory::updateTextHeight(TextButtonEdit * textButtonEdit)
{
  if (!textButtonEdit)
    return;

  int    headerHeight  = textButtonEdit->headerTextEdit->height();
  int    minBodyHeight = 30;
  QSizeF documentSize  = textButtonEdit->textEdit->document()->size();
  int    newBodyHeight = static_cast<int>(documentSize.height()) + 15;
  if (newBodyHeight < minBodyHeight)
  {
    newBodyHeight = minBodyHeight;
  }
  textButtonEdit->textEdit->setFixedHeight(newBodyHeight);

  int spacing        = 5;
  int totalNewHeight = headerHeight + newBodyHeight + spacing;

  textButtonEdit->setFixedHeight(totalNewHeight);
  textButtonEdit->button->setFixedHeight(totalNewHeight);
  textButtonEdit->dragButton->setFixedHeight(totalNewHeight);
}

void TextFactory::removeText(int id)
{
  if (id < 0 || id >= activeWidgets.size())
    return;
  removeText(activeWidgets.at(id));
}

void TextFactory::removeText()
{
  if (!activeWidgets.isEmpty())
  {
    removeText(activeWidgets.last());
  }
}

void TextFactory::removeText(TextButtonEdit * widget)
{
  if (!widget || !activeWidgets.contains(widget))
    return;

  activeWidgets.removeOne(widget);
  layout->removeWidget(widget);
  textPool.releaseObject(widget);
}

void TextFactory::removeAllText()
{
  QList<TextButtonEdit *> widgetsToRemove = activeWidgets;
  for (TextButtonEdit * widget : widgetsToRemove)
  {
    removeText(widget);
  }
}

void TextFactory::moveText(TextButtonEdit * widget, int toIndex)
{
  int fromIndex = activeWidgets.indexOf(widget);
  if (fromIndex == -1 || fromIndex == toIndex)
  {
    return;
  }

  layout->removeWidget(widget);
  activeWidgets.removeAt(fromIndex);

  int correctedToIndex = toIndex;
  if (fromIndex < correctedToIndex)
  {
    correctedToIndex--;
  }

  if (correctedToIndex > activeWidgets.count())
  {
    correctedToIndex = activeWidgets.count();
  }

  layout->insertWidget(correctedToIndex, widget);
  activeWidgets.insert(correctedToIndex, widget);
}


QList<TextButtonEditData> TextFactory::getText()
{
  QList<TextButtonEditData> result;
  for (TextButtonEdit * el : std::as_const(activeWidgets))
  {
    QString   headerText = el->headerTextEdit->toMarkdown();
    QString   bodyText   = el->textEdit->toMarkdown();
    QDate     date       = el->dateEdit->date();
    QTime     time       = el->timeEdit->time();
    QDateTime dateTime(date, time);
    result.append(TextButtonEditData(headerText, bodyText, dateTime));
  }
  return result;
}
