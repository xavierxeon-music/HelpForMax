#ifndef ResultHighlighterH
#define ResultHighlighterH

#include <QSyntaxHighlighter>

#include <QRegularExpression>

namespace Result
{
   class Highlighter : public QSyntaxHighlighter
   {
      Q_OBJECT

   public:
      Highlighter(QTextDocument* parent = nullptr);

   protected:
      void highlightBlock(const QString& text) override;

   private:
      void highlightByRegex(const QTextCharFormat& format, const QRegularExpression& regex, const QString& text);

   private:
      QTextCharFormat xmlKeywordFormat;
      QTextCharFormat xmlElementFormat;
      QTextCharFormat xmlAttributeFormat;
      QTextCharFormat xmlValueFormat;
      QTextCharFormat xmlCommentFormat;

      QList<QRegularExpression> xmlKeywordRegexes;
      QRegularExpression xmlElementRegex;
      QRegularExpression xmlAttributeRegex;
      QRegularExpression xmlValueRegex;
      QRegularExpression xmlCommentRegex;
   };
} // namespace Result

#endif // NOT ResultHighlighterH
