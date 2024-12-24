#ifndef DelegateDataTypeH
#define DelegateDataTypeH

#include <QStyledItemDelegate>

#include "RefStructure.h"

namespace Delegate
{
   class DataType : public QStyledItemDelegate
   {
      Q_OBJECT

   public:
      struct Source
      {
         virtual Max::DataType getDataType(const int index) = 0;
      };

   public:
      DataType(QObject* parent, Source* source);

   public:
      QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
      void setEditorData(QWidget* editor, const QModelIndex& index) const override;
      void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
      void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

   private:
      Source* source;
   };
} // namespace Delegate

#endif // NOT DelegateDataTypeH
