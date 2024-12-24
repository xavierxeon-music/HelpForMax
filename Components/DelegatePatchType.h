#ifndef DelegatePatchTypeH
#define DelegatePatchTypeH

#include <QStyledItemDelegate>

#include "RefStructure.h"

namespace Delegate
{
   class PatchType : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      struct Source
      {
         virtual Ref::Structure::PatchType getPatchType(const int index) = 0;
      };

   public:
      PatchType(QObject* parent, Source* source);

   public:
      QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
      void setEditorData(QWidget* editor, const QModelIndex& index) const override;
      void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
      void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

   private:
      Source* source;
   };
} // namespace Delegate

#endif // NOT DelegatePatchTypeH
