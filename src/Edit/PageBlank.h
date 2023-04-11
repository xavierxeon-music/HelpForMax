#ifndef PageBlankH
#define PageBlankH

#include "PageAbstract.h"
#include "ui_PageBlank.h"

namespace Page
{
   class Blank : public Abstract,
                 private Ui::Blank
   {
      Q_OBJECT

   public:
      Blank(QWidget* parent, Central* central, const Block::Item::Marker& marker);

   private:
      void update(const QVariant& data) override;
   };

} // namespace Page

#endif // HelpPageBlankH
