#include "ResultHighlighter.h"

Result::Highlighter::Highlighter(QTextDocument* parent)
   : QSyntaxHighlighter(parent)
{
   xmlKeywordFormat.setForeground(Qt::blue);
   xmlKeywordFormat.setFontWeight(QFont::Bold);

   xmlElementFormat.setForeground(Qt::darkMagenta);
   xmlElementFormat.setFontWeight(QFont::Bold);

   xmlAttributeFormat.setForeground(Qt::darkGreen);
   xmlAttributeFormat.setFontWeight(QFont::Bold);
   xmlAttributeFormat.setFontItalic(true);

   xmlValueFormat.setForeground(Qt::darkRed);

   xmlCommentFormat.setForeground(Qt::gray);
   xmlElementRegex.setPattern("<[?\\s]*[/]?[\\s]*([^\\n][^>]*)(?=[\\s/>])");
   xmlAttributeRegex.setPattern("\\w+(?=\\=)");
   xmlValueRegex.setPattern("\"[^\\n\"]+\"(?=[?\\s/>])");
   xmlCommentRegex.setPattern("<!--[^\\n]*-->");

   xmlKeywordRegexes << QRegularExpression("<\\?");
   xmlKeywordRegexes << QRegularExpression("/>");
   xmlKeywordRegexes << QRegularExpression(">");
   xmlKeywordRegexes << QRegularExpression("<");
   xmlKeywordRegexes << QRegularExpression("</");
   xmlKeywordRegexes << QRegularExpression("\\?>");
}

void Result::Highlighter::highlightBlock(const QString& text)
{
   highlightByRegex(xmlElementFormat, xmlElementRegex, text);

   for (QList<QRegularExpression>::const_iterator it = xmlKeywordRegexes.constBegin(); it != xmlKeywordRegexes.constEnd(); ++it)
   {
      const QRegularExpression& regex = *it;
      highlightByRegex(xmlKeywordFormat, regex, text);
   }

   highlightByRegex(xmlAttributeFormat, xmlAttributeRegex, text);
   highlightByRegex(xmlCommentFormat, xmlCommentRegex, text);
   highlightByRegex(xmlValueFormat, xmlValueRegex, text);
}

void Result::Highlighter::highlightByRegex(const QTextCharFormat& format, const QRegularExpression& regex, const QString& text)
{
   QRegularExpressionMatchIterator regexIterator = regex.globalMatch(text);
   while (regexIterator.hasNext())
   {
      QRegularExpressionMatch match = regexIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), format);
   }
}
