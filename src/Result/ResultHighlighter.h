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
      struct HighlightingRule
      {
         QRegularExpression pattern;
         QTextCharFormat format;
      };
      QList<HighlightingRule> highlightingRules;

      QRegularExpression commentStartExpression;
      QRegularExpression commentEndExpression;

      QTextCharFormat keywordFormat;
      QTextCharFormat classFormat;
      QTextCharFormat singleLineCommentFormat;
      QTextCharFormat multiLineCommentFormat;
      QTextCharFormat quotationFormat;
      QTextCharFormat functionFormat;
   };
} // namespace Result

#endif // NOT ResultHighlighterH
