#include "HelpDescriptionHighlighter.h"

#include "HelpPatchParser.h"

Help::DescriptionHighlighter::DescriptionHighlighter(QTextDocument* document)
   : QSyntaxHighlighter(document)
   , tagFormat()
   , contentFormat()
   , errorFormat()
   , tagStartExpression()
   , tagEndExpression()
{
   tagFormat.setForeground(Qt::blue);
   tagFormat.setFontWeight(QFont::Bold);

   contentFormat.setFontItalic(true);

   errorFormat.setBackground(Qt::lightGray);
   errorFormat.setForeground(Qt::red);

   QString patternStart;
   QString patternEnd;
   for (const QByteArray& tag : PatchParser::descriptionMaxTags)
   {
      if (!patternStart.isEmpty())
         patternStart += "|";
      patternStart += "<" + QString::fromLatin1(tag) + ">";

      patternEnd += "|";
      patternEnd += "</" + QString::fromLatin1(tag) + ">";
   }

   tagStartExpression.setPattern(patternStart);
   tagEndExpression.setPattern(patternEnd);
}

void Help::DescriptionHighlighter::highlightBlock(const QString& text)
{
   using ContentMap = QMap<int, bool>;
   ContentMap contentMap;

   QRegularExpressionMatchIterator matchStartIterator = tagStartExpression.globalMatch(text);
   while (matchStartIterator.hasNext())
   {
      QRegularExpressionMatch match = matchStartIterator.next();
      if (0 == match.capturedLength())
         continue;

      setFormat(match.capturedStart(), match.capturedLength(), tagFormat);
      contentMap[match.capturedStart() + match.capturedLength()] = true;
   }

   QRegularExpressionMatchIterator matchEndIterator = tagEndExpression.globalMatch(text);
   while (matchEndIterator.hasNext())
   {
      QRegularExpressionMatch match = matchEndIterator.next();
      if (0 == match.capturedLength())
         continue;

      setFormat(match.capturedStart(), match.capturedLength(), tagFormat);
      contentMap[match.capturedStart()] = false;
   }

   if (contentMap.empty())
      return;

   int startIndex = -1;
   for (ContentMap::ConstIterator it = contentMap.constBegin(); it != contentMap.constEnd(); it++)
   {
      if (it.value())
      {
         startIndex = it.key();
         continue;
      }
      else
      {
         const int length = it.key() - startIndex;
         setFormat(startIndex, length, contentFormat);
         startIndex = -1;
      }
   }

   if (-1 != startIndex)
   {
      setFormat(startIndex, text.length() - startIndex, errorFormat);
   }
}
