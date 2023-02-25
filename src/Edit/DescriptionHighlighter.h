#ifndef HelpDescriptionHighlighterH
#define HelpDescriptionHighlighterH

#include <QRegularExpression>
#include <QSyntaxHighlighter>

namespace Help
{
   class DescriptionHighlighter : public QSyntaxHighlighter
   {
      Q_OBJECT

   public:
      DescriptionHighlighter(QTextDocument* document);

   protected:
      void highlightBlock(const QString& text) override;

   private:
      QTextCharFormat tagFormat;
      QTextCharFormat contentFormat;
      QTextCharFormat errorFormat;
      QRegularExpression tagStartExpression;
      QRegularExpression tagEndExpression;
   };
} // namespace Help

#endif // NOT HelpDescriptionHighlighterH
