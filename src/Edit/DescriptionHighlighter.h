#ifndef DescriptionHighlighterH
#define DescriptionHighlighterH

#include <QRegularExpression>
#include <QSyntaxHighlighter>

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

#endif // NOT DescriptionHighlighterH
