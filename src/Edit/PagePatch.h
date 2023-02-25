#ifndef HelpPagePatchH
#define HelpPagePatchH

#include "HelpPageAbstract.h"
#include "ui_HelpPagePatch.h"

#include "HelpDescriptionHighlighter.h"

namespace Help
{
   namespace Page
   {
      class Patch : public Abstract, private Ui::Patch
      {
         Q_OBJECT

      public:
         Patch(Persona* persona, const PatchParser::Marker& marker);

      private slots:
         void slotAddStandardMethond(int type);

      private:
         void update(const QVariant& data) override;

      private:
         DescriptionHighlighter* highlighter;
         QButtonGroup* standardMethodGroup;
      };
   } // namespace Page
} // namespace Help

#endif // NOT HelpPagePatchH
