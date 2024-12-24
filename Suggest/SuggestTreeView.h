#ifndef SuggestTreeViewH
#define SuggestTreeViewH

#include <QTreeView>

#include <QToolButton>

namespace Patch
{
   class Widget;
} // namespace Patch

namespace Suggest
{
   namespace Model
   {
      class Abstract;
   }

   class TreeView : public QTreeView
   {
      Q_OBJECT

   public:
      TreeView(QWidget* parent);

   public:
      void init(Model::Abstract* model);
      void transferAll();
      void transferSelected();

   private slots:
      void slotResizeColumns();

   private:
      Model::Abstract* model;
   };
} // namespace Suggest

#endif // NOT SuggestTreeViewH
