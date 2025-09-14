#ifndef MARKDOWNHIGHLIGHTER_H
#define MARKDOWNHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QApplication>

class MarkdownHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT

public:
  MarkdownHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;

private:
  QTextCharFormat markerFormat;

  QRegularExpression headerPattern;
  QTextCharFormat    headerFormat;

  QRegularExpression boldPattern;
  QTextCharFormat    boldFormat;

  QRegularExpression italicPattern;
  QTextCharFormat    italicFormat;

  QRegularExpression strikePattern;
  QTextCharFormat    strikeFormat;

  QRegularExpression codePattern;
  QTextCharFormat    codeFormat;

};
#endif // MARKDOWNHIGHLIGHTER_H
