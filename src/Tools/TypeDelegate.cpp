#include "TypeDelegate.h"

#include <QComboBox>

TypeDelegate::TypeDelegate(QObject* parent, TypeDelegate::Proxy* proxy)
   : QStyledItemDelegate(parent)
   , proxy(proxy)
{
}

QWidget* TypeDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QComboBox* comboBox = new QComboBox(parent);
   comboBox->setFrame(false);
   for (const Structure::Type& type : Structure::typeList())
   {
      comboBox->addItem(Structure::typeName(type));
   }

   for (int index = 0; index < comboBox->count(); index++)
   {
      comboBox->setItemData(index, Qt::AlignCenter, Qt::TextAlignmentRole);
   }

   return comboBox;
}

void TypeDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

   const Structure::Type type = proxy->getType(index.row());
   const int typeIndex = comboBox->findText(Structure::typeName(type));
   comboBox->setCurrentIndex(typeIndex);
}

void TypeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

   const QString typeText = comboBox->currentText();
   model->setData(index, typeText, Qt::DisplayRole);
}

void TypeDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(index)

   editor->setGeometry(option.rect);
}
