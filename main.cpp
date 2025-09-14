#include "textwindow.h"

#include <QApplication>
#include <QIcon>
#include <QTranslator>

int main(int argc, char * argv[])
{
  QApplication::setStyle("Fusion");
  QApplication a(argc, argv);

  QCoreApplication::setOrganizationName("tev1l");
  QCoreApplication::setApplicationName("todolist");

  QTranslator translator;

  QLocale locale = QLocale::system();

  QString translationsPath = QCoreApplication::applicationDirPath() + "/translations";

  if (translator.load(locale, "app", "_", translationsPath))
  {
    a.installTranslator(&translator);
  }

  a.setWindowIcon(QIcon(":/MainIcon.ico"));
  a.setApplicationDisplayName("TodoList");

  QFile styleFile(":/style.qss");
  if (styleFile.open(QFile::ReadOnly | QFile::Text))
  {
    QString styleSheet = QLatin1String(styleFile.readAll());
    a.setStyleSheet(styleSheet);
    styleFile.close();
  }
  else
  {
    qWarning("Could not find style.qss. Make sure it's in the .qrc file.");
  }

  TextWindow w;
  w.show();
  return a.exec();
}
