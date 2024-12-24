#ifndef PatchRefTreeViewH
#define PatchRefTreeViewH

#include <QTreeView>

#include <QToolButton>

namespace PatchRef
{
   namespace Model
   {
      class Abstract;
   }

   class Widget;

   class TreeView : public QTreeView
   {
      Q_OBJECT

   public:
      TreeView(QWidget* parent);

   public:
      void init(Widget* widget, Model::Abstract* model, int forceRowHeight = -1);
      void setButtons(QToolButton* addButton, QToolButton* removeButton);

   private slots:
      void slotResizeColumns();
      void slotUpdateDigest(const QModelIndex& index);
      void slotAddElement();
      void slotRemoveElement();

   private:
      Widget* widget;
      Model::Abstract* model;
      int forceRowHeight;
   };
} // namespace PatchRef

#endif // NOT PatchRefTreeViewH
