#ifndef PageMessageFreeH
#define PageMessageFreeH

#include "PageAbstract.h"
#include "ui_PageMessageFree.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class MessageFree : public Abstract,
                       private Ui::MessageFree
   {
      Q_OBJECT

   public:
      MessageFree(QWidget* parent, Central* central, const Block::Item::Marker& marker);

   private:
      void update(const QVariant& data) override;

   private:
      DescriptionHighlighter* highlighter;
      QString messageName;
   };
} // namespace Page

#endif // NOT PageMessageFreeH
