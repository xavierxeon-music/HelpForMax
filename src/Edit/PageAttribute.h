#ifndef PageAttributeH
#define PageAttributeH

#include "PageAbstract.h"
#include "ui_PageAttribute.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class Attribute : public Abstract, private Ui::Attribute
   {
      Q_OBJECT

   public:
      Attribute(MainWindow* mainWindow, const PatchParser::Marker& marker);

   private:
      void update(const QVariant& data) override;

   private:
      DescriptionHighlighter* highlighter;
      QString attributeName;
   };
} // namespace Page

#endif // NOT PageAttributeH
