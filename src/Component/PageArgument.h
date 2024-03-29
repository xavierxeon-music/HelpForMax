#ifndef PageArgumentH
#define PageArgumentH

#include "PageAbstract.h"
#include "ui_PageArgument.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class Argument : public Abstract,
                    private Ui::Argument
   {
      Q_OBJECT

   public:
      Argument(QWidget* parent, Central* central, const Block::Marker& marker);

   private:
      void update(const QVariant& data) override;

   private:
      DescriptionHighlighter* highlighter;
      int argumentIndex;
   };
} // namespace Page

#endif // NOT PageArgumentH
