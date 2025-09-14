#include "markdownhighlighter.h"

MarkdownHighlighter::MarkdownHighlighter(QTextDocument * parent) : QSyntaxHighlighter(parent)
{
  markerFormat.setForeground(Qt::transparent);

  headerPattern.setPattern(QStringLiteral("(#)(.*?)(#)"));
  headerFormat.setForeground(QColor(100, 150, 255));
  headerFormat.setFontWeight(QFont::Bold);

  boldPattern.setPattern(QStringLiteral("(\\*\\*|__)(.*?)\\1"));
  boldFormat.setFontWeight(QFont::Bold);

  italicPattern.setPattern(QStringLiteral("(?<![*_])([*_])(?![*_])(.*?)(?<![*_])\\1(?![*_])"));
  italicFormat.setFontItalic(true);

  strikePattern.setPattern(QStringLiteral("(~~)(.*?)\\1"));
  strikeFormat.setFontStrikeOut(true);

  codePattern.setPattern(QStringLiteral("(')(.*?)(')"));
  codeFormat.setBackground(QColor(60, 60, 60));
}

void MarkdownHighlighter::highlightBlock(const QString & text)
{
  auto matchIterator = boldPattern.globalMatch(text);
  while (matchIterator.hasNext())
  {
    auto match = matchIterator.next();
    setFormat(match.capturedStart(2), match.capturedLength(2), boldFormat);
    setFormat(match.capturedStart(1), match.capturedLength(1), markerFormat);
    setFormat(match.capturedEnd(2), match.capturedLength(1), markerFormat);
  }

  matchIterator = italicPattern.globalMatch(text);
  while (matchIterator.hasNext())
  {
    auto match = matchIterator.next();
    setFormat(match.capturedStart(2), match.capturedLength(2), italicFormat);
    setFormat(match.capturedStart(1), match.capturedLength(1), markerFormat);
    setFormat(match.capturedEnd(2), match.capturedLength(1), markerFormat);
  }

  matchIterator = headerPattern.globalMatch(text);
  while (matchIterator.hasNext())
  {
    auto match = matchIterator.next();
    setFormat(match.capturedStart(1), match.capturedLength(1), markerFormat);
    setFormat(match.capturedStart(2), match.capturedLength(2), headerFormat);
    setFormat(match.capturedStart(3), match.capturedLength(3), markerFormat);
  }

  matchIterator = strikePattern.globalMatch(text);
  while (matchIterator.hasNext())
  {
    auto match = matchIterator.next();
    setFormat(match.capturedStart(2), match.capturedLength(2), strikeFormat);
    setFormat(match.capturedStart(1), match.capturedLength(1), markerFormat);
    setFormat(match.capturedEnd(2), match.capturedLength(1), markerFormat);
  }

  matchIterator = codePattern.globalMatch(text);
  while (matchIterator.hasNext())
  {
    auto match = matchIterator.next();
    setFormat(match.capturedStart(2), match.capturedLength(2), codeFormat);
    setFormat(match.capturedStart(1), match.capturedLength(1), markerFormat);
    setFormat(match.capturedStart(3), match.capturedLength(3), markerFormat);
  }
}
