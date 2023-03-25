#ifndef PagePatchH
#define PagePatchH

#include "PageAbstract.h"
#include "ui_PagePatch.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class Patch : public Abstract, private Ui::Patch
   {
      Q_OBJECT

   public:
      Patch(MainWindow* mainWindow, const Block::Item::Marker& marker);

   private slots:
      void slotAddStandardMethond(int typeId);

   private:
      void update(const QVariant& data) override;

   private:
      DescriptionHighlighter* highlighter;
      QButtonGroup* standardMethodGroup;
   };
} // namespace Page

#endif // NOT PagePatchH
