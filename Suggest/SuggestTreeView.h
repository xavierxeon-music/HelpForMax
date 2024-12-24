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
      ~TreeView();

   public:
      void init(Model::Abstract* model, QToolButton* transferButton);

      static void setAllVisible(bool visible);

   private slots:
      void slotResizeColumns();
      void slotTransfer();

   private:
      static QList<QWidget*> instanceList;
      Model::Abstract* model;
   };
} // namespace Suggest

#endif // NOT SuggestTreeViewH
