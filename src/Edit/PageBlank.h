#ifndef HelpPageBlankH
#define HelpPageBlankH

#include "HelpPageAbstract.h"
#include "ui_HelpPageBlank.h"

namespace Help
{
   namespace Page
   {
      class Blank : public Abstract, private Ui::Blank
      {
         Q_OBJECT

      public:
         Blank(Persona* persona, const PatchParser::Marker& marker);

      private:
         void update(const QVariant& data) override;
      };

   } // namespace Page
} // namespace Help

#endif // HelpPageBlankH
