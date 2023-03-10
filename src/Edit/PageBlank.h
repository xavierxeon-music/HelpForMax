#ifndef PageBlankH
#define PageBlankH

#include "PageAbstract.h"
#include "ui_PageBlank.h"

namespace Page
{
   class Blank : public Abstract, private Ui::Blank
   {
      Q_OBJECT

   public:
      Blank(MainWindow* mainWindow, const PatchParser::Marker& marker);

   private:
      void update(const QVariant& data) override;
   };

} // namespace Page

#endif // HelpPageBlankH
