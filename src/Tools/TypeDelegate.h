#ifndef TypeDelegateH
#define TypeDelegateH

#include <QStyledItemDelegate>

#include "PatchStructure.h"

class TypeDelegate : public QStyledItemDelegate
{
   Q_OBJECT

public:
   struct Proxy
   {
      virtual PatchStructure::Type getType(const int index) = 0;
   };

public:
   TypeDelegate(QObject* parent, Proxy* proxy);

public:
   QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
   void setEditorData(QWidget* editor, const QModelIndex& index) const override;
   void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
   void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
   Proxy* proxy;
};

#endif // NOT TypeDelegateH
