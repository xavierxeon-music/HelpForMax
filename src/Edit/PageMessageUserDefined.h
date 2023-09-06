#ifndef PageMessageUserDefinedH
#define PageMessageUserDefinedH

#include "PageAbstract.h"
#include "ui_PageMessageUserDefined.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class MessageUserDefined : public Abstract,
                       private Ui::MessageUserDefined
   {
      Q_OBJECT

   public:
      MessageUserDefined(QWidget* parent, Central* central, const Block::Item::Marker& marker);

   private:
      void update(const QVariant& data) override;

   private:
      DescriptionHighlighter* highlighter;
      QString messageName;
   };
} // namespace Page

#endif // NOT PageMessageUserDefinedH
