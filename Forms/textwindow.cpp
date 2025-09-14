#include "textwindow.h"
#include "Forms/ui_textwindow.h"

TextWindow::TextWindow(QWidget * parent) : QMainWindow(parent), ui(new Ui::TextWindow)
{
  ui->setupUi(this);
  textFactory = new TextFactory(ui->mainTextLayout);
  setAcceptDrops(true);
  dndManager  = new DragDropManager(this, ui->mainTextLayout, textFactory, this);
  menuActions = new ContextMenuActions(this, this);
  setupFormattingControls();

  textFilter     = new TextFilter();
  dateTimeFilter = new DateTimeFilter();
  filterChain    = new FilterChain({textFilter, dateTimeFilter});

  connect(ui->timeFilterStart, &QDateTimeEdit::dateTimeChanged, this, &TextWindow::applyFilters);
  connect(ui->timeFilterEnd, &QDateTimeEdit::dateTimeChanged, this, &TextWindow::applyFilters);
}

TextWindow::~TextWindow()
{
  delete ui;
  delete textFactory;
  delete dndManager;
  delete boldAction;
  delete italicAction;
  delete strikeAction;
  delete codeAction;
  delete headerAction;
  delete filterChain;
  delete textFilter;
  delete dateTimeFilter;
}

void TextWindow::on_createButton_clicked()
{
  TextButtonEditData data = TextButtonEditData();
  data.dateTime           = QDateTime::currentDateTime();
  textFactory->createText(data);
}

void TextWindow::onSaveTriggered()
{
  DataClass *               data      = JsonDataSerializer::data;
  QList<TextButtonEditData> itemsList = textFactory->getText();
  data->setItems(itemsList);
  if (JsonDataSerializer::save())
    return;
  QMessageBox::information(this, tr("An error occured"), tr("Can't save file"));
}

void TextWindow::onLoadTriggered()
{
  if (!JsonDataSerializer::load())
  {
    QMessageBox::information(this, tr("An error occured"), tr("Can't load file"));
    return;
  }
  DataClass *               data      = JsonDataSerializer::data;
  QList<TextButtonEditData> itemsList = data->getItems();
  textFactory->removeAllText();
  textFactory->createTextList(itemsList);
}

void TextWindow::on_headerSearch_textEdited(const QString & arg1)
{
  textFilter->setSearchText(arg1);
  textFilter->setActive(!arg1.isEmpty());
  applyFilters();
}

void TextWindow::on_timeFilterCheckBox_checkStateChanged(const Qt::CheckState & arg1)
{
  dateTimeFilter->setActive(arg1 == Qt::CheckState::Checked);
  applyFilters();
}

void TextWindow::applyFilters()
{
  if (dateTimeFilter->isActive())
  {
    dateTimeFilter->setDateTimeRange(ui->timeFilterStart->dateTime(), ui->timeFilterEnd->dateTime());
  }

  filterChain->apply(textFactory->getActiveWidgets());
}

void TextWindow::setupFormattingControls()
{
  boldAction = new QAction(this);
  boldAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));
  italicAction = new QAction(this);
  italicAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_I));
  strikeAction = new QAction(this);
  strikeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
  codeAction = new QAction(this);
  codeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_K));
  headerAction = new QAction(this);
  headerAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));

  this->addActions({boldAction, italicAction, strikeAction, codeAction, headerAction});

  connect(boldAction, &QAction::triggered, this, &TextWindow::onBoldFormatTriggered);
  connect(italicAction, &QAction::triggered, this, &TextWindow::onItalicFormatTriggered);
  connect(strikeAction, &QAction::triggered, this, &TextWindow::onStrikeFormatTriggered);
  connect(codeAction, &QAction::triggered, this, &TextWindow::onCodeFormatTriggered);
  connect(headerAction, &QAction::triggered, this, &TextWindow::onHeaderFormatTriggered);
}

TextButtonEdit * TextWindow::findFocusedTextButtonEdit() const
{
  QWidget * focusWidget = QApplication::focusWidget();
  if (!focusWidget)
  {
    return nullptr;
  }

  QWidget * parent = focusWidget;
  while (parent)
  {
    if (auto textButtonEdit = qobject_cast<TextButtonEdit *>(parent))
    {
      return textButtonEdit;
    }
    parent = parent->parentWidget();
  }

  return nullptr;
}

void TextWindow::onBoldFormatTriggered()
{
  onFormatTriggered("**");
}

void TextWindow::onItalicFormatTriggered()
{
  onFormatTriggered("*");
}

void TextWindow::onStrikeFormatTriggered()
{
  onFormatTriggered("~~");
}

void TextWindow::onCodeFormatTriggered()
{
  onFormatTriggered("'");
}

void TextWindow::onHeaderFormatTriggered()
{
  onFormatTriggered("#");
}

void TextWindow::onFormatTriggered(const QString & format)
{
  TextButtonEdit * target = findFocusedTextButtonEdit();
  if (target)
  {
    target->applyFormatting(format, format);
  }
}

void TextWindow::on_boldFormatterButton_clicked()
{
  onBoldFormatTriggered();
}

void TextWindow::on_italicFormatterButton_clicked()
{
  onItalicFormatTriggered();
}

void TextWindow::on_strikeFormatterButton_clicked()
{
  onStrikeFormatTriggered();
}

void TextWindow::on_codeFormatterButton_clicked()
{
  onCodeFormatTriggered();
}

void TextWindow::on_headerFormatterButton_clicked()
{
  onHeaderFormatTriggered();
}
