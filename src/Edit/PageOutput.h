#ifndef PageOutputH
#define PageOutputH

#include "PageAbstract.h"
#include "ui_PageOutput.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class Output : public Abstract,
                  private Ui::Output
   {
      Q_OBJECT

   public:
      Output(QWidget* parent, Central* central, const Block::Item::Marker& marker);

   private:
      void update(const QVariant& data) override;

   private:
      DescriptionHighlighter* highlighter;
      int outputIndex;
   };
} // namespace Page

#endif // NOT PageOutputH
