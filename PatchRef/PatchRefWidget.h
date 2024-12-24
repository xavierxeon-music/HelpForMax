#ifndef PatchRefWidgetH
#define PatchRefWidgetH

#include "ui_PatchRefWidget.h"
#include <QWidget>

#include "RefStructure.h"

namespace Patch
{
   class Widget;
}

namespace Package
{
   class Info;
}

namespace PatchRef
{
   class Widget : public QWidget, private Ui::Widget
   {
      Q_OBJECT

   public:
      Widget(Patch::Widget* patchWidget);

   public:
      void rebuild();
      void update();

   private:
      friend class TreeView;

   private slots:
      void slotSetPatchDigest();
      void slotSaveDigestText();
      void slotSaveDigestDescription();

   private:
      void setDigest(Ref::Structure::Digest* newDigest, const Ref::Structure::PatchPart& part);
      void setIcon(QLabel* iconLabel, Ref::Structure::PatchPart part);

   private:
      Patch::Widget* patchWidget;
      Ref::Structure::Digest* digest;
   };

} // namespace PatchRef

#endif // NOT PatchRefWidgetH
