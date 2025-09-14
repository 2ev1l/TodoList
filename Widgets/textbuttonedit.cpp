#include "textbuttonedit.h"

const QString TextButtonEdit::dragEventName = "application/x-textbuttonedit-ptr";

TextButtonEdit::TextButtonEdit(QWidget * parent) : PoolableWidget{parent}
{
  headerTextEdit = new QTextEdit(this);
  textEdit       = new QTextEdit(this);
  button         = new QPushButton("Button", this);
  dateEdit       = new QDateEdit(this);
  timeEdit       = new QTimeEdit(this);
  dragButton     = new QPushButton("☰", this);

  this->setProperty("class", "card");

  headerTextEdit->setObjectName("headerTextEdit");
  textEdit->setObjectName("textEdit");
  dragButton->setObjectName("dragButton");

  button->setFocusPolicy(Qt::NoFocus);
  dragButton->setFocusPolicy(Qt::NoFocus);

  dateEdit->setDisplayFormat("dd.MM.yyyy");
  timeEdit->setDisplayFormat("hh:mm:ss");

  dragButton->setCursor(Qt::OpenHandCursor);

  QVBoxLayout * textLayout = new QVBoxLayout();
  textLayout->addWidget(headerTextEdit);
  textLayout->addWidget(textEdit);
  textLayout->setContentsMargins(0, 0, 0, 0);
  textLayout->setSpacing(5);

  QVBoxLayout * dateTimeLayout = new QVBoxLayout();
  dateTimeLayout->addWidget(dateEdit);
  dateTimeLayout->addWidget(timeEdit);
  dateTimeLayout->addStretch();

  QHBoxLayout * layout = new QHBoxLayout(this);
  layout->addWidget(dragButton);
  layout->addLayout(textLayout);
  layout->addLayout(dateTimeLayout);
  layout->addWidget(button);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(5);


  setLayout(layout);
  connect(button, &QPushButton::clicked, this, &TextButtonEdit::buttonClicked);

  dragButton->installEventFilter(this);
}
TextButtonEdit::~TextButtonEdit()
{
}

void TextButtonEdit::ApplyFilter(bool isVisible)
{
  this->setVisible(isVisible);
}

QString TextButtonEdit::getText() const
{
  return headerTextEdit->toPlainText();
}

QDateTime TextButtonEdit::getDateTime() const
{
  return QDateTime(dateEdit->date(), timeEdit->time());
}


void TextButtonEdit::applyFormatting(const QString & prefix, const QString & suffix)
{
  QTextEdit * targetEdit = nullptr;
  if (headerTextEdit->hasFocus())
    {
      targetEdit = headerTextEdit;
    }
  else if (textEdit->hasFocus())
    {
      targetEdit = textEdit;
    }

  if (targetEdit)
    {
      QTextCursor cursor         = targetEdit->textCursor();
      QString     selectedText   = cursor.selectedText();
      QString     formattedText  = prefix + selectedText + suffix;
      cursor.insertText(formattedText);
    }
}


bool TextButtonEdit::eventFilter(QObject * watched, QEvent * event)
{
  if (watched != dragButton)
    {
      return QWidget::eventFilter(watched, event);
    }

  switch (event->type())
    {
    case QEvent::MouseButtonPress:
      {
        QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() != Qt::LeftButton)
          {
            break;
          }
        dragStartPosition = mouseEvent->pos();
        return true;
      }

    case QEvent::MouseMove:
      {
        QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(event);
        if (!(mouseEvent->buttons() & Qt::LeftButton))
          {
            break;
          }
        if ((mouseEvent->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
          {
            break;
          }

        QPixmap originalPixmap = this->grab();
        QPixmap transparentPixmap(originalPixmap.size());
        transparentPixmap.fill(Qt::transparent);

        QPainter painter(&transparentPixmap);
        painter.setOpacity(0.7);
        painter.drawPixmap(0, 0, originalPixmap);
        painter.end();

        QDrag *     drag     = new QDrag(this);
        QMimeData * mimeData = new QMimeData;
        QByteArray  itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << qulonglong(this);
        mimeData->setData(dragEventName, itemData);

        drag->setMimeData(mimeData);
        drag->setPixmap(transparentPixmap);
        drag->setHotSpot(mouseEvent->pos() + dragButton->pos());

        this->hide();
        drag->exec(Qt::MoveAction);
        this->show();

        return true;
      }

    default:
      break;
    }

  return QWidget::eventFilter(watched, event);
}
