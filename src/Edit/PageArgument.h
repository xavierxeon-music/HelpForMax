#ifndef HelpPageArgumentH
#define HelpPageArgumentH

#include "HelpPageAbstract.h"
#include "ui_HelpPageArgument.h"

#include "HelpDescriptionHighlighter.h"

namespace Help
{
   namespace Page
   {
      class Argument : public Abstract, private Ui::Argument
      {
         Q_OBJECT

      public:
         Argument(Persona* persona, const PatchParser::Marker& marker);

      private:
         void update(const QVariant& data) override;

      private:
         DescriptionHighlighter* highlighter;
         int argumentIndex;
      };
   } // namespace Page
} // namespace Help

#endif // NOT HelpPageArgumentH
