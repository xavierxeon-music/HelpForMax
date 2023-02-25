#ifndef PageMessageH
#define PageMessageH

#include "PageAbstract.h"
#include "ui_PageMessage.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class Message : public Abstract, private Ui::Message
   {
      Q_OBJECT

   public:
      Message(MainWindow* mainWindow, const PatchParser::Marker& marker);

   private:
      void update(const QVariant& data) override;

   private:
      DescriptionHighlighter* highlighter;
      QString messageName;
   };
} // namespace Page

#endif // NOT PageMessageH
