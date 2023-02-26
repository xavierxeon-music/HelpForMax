#ifndef PageMessageStandardH
#define PageMessageStandardH

#include "PageAbstract.h"
#include "ui_PageMessageStandard.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class MessageStandard : public Abstract, private Ui::MessageStandard
   {
      Q_OBJECT

   public:
      MessageStandard(MainWindow* mainWindow, const PatchParser::Marker& marker);

   private:
      void update(const QVariant& data) override;

   private:
      DescriptionHighlighter* highlighter;
      PatchStructure::Type messageType;
   };
} // namespace Page

#endif // NOT PageMessageStandardH
