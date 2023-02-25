#ifndef HelpArgumentViewH
#define HelpArgumentViewH

#include <QStyledItemDelegate>
#include <QTreeView>

#include <QStandardItemModel>

#include "PatchStructure.h"

namespace Help
{

   class ArgumentView : public QTreeView
   {
      Q_OBJECT

   public:
      ArgumentView(QWidget* parent);

   public:
      void allowNameEdit(bool allow);
      void clearMonitors();
      void monitor(PatchStructure::Argument* argument);

   private:
      friend class TypeDelegate;

   private slots:
      void slotItemChanged(QStandardItem* item);

   private:
      QList<PatchStructure::Argument*> argumentList;
      QStandardItemModel* argumentModel;
      bool nameEditable;
   };

   class TypeDelegate : public QStyledItemDelegate
   {
      Q_OBJECT
   public:
      TypeDelegate(ArgumentView* view);

   public:
      QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
      void setEditorData(QWidget* editor, const QModelIndex& index) const override;
      void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
      void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

   private:
      ArgumentView* view;
   };
} // namespace Help

#endif // NOT HelpArgumentViewH
