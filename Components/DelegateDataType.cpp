#include "DelegateDataType.h"

#include <QComboBox>

Delegate::DataType::DataType(QObject* parent, DataType::Source* source)
   : QStyledItemDelegate(parent)
   , source(source)
{
}

QWidget* Delegate::DataType::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QComboBox* comboBox = new QComboBox(parent);
   comboBox->setFrame(false);
   for (const Max::DataType& type : Max::dataTypeList())
   {
      comboBox->addItem(Max::dataTypeName(type));
   }

   for (int index = 0; index < comboBox->count(); index++)
   {
      comboBox->setItemData(index, Qt::AlignCenter, Qt::TextAlignmentRole);
   }

   return comboBox;
}

void Delegate::DataType::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

   const Max::DataType type = source->getDataType(index.row());
   const int typeIndex = comboBox->findText(Max::dataTypeName(type));
   comboBox->setCurrentIndex(typeIndex);
}

void Delegate::DataType::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

   const QString typeText = comboBox->currentText();
   model->setData(index, typeText, Qt::EditRole);
}

void Delegate::DataType::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(index)

   editor->setGeometry(option.rect);
}
