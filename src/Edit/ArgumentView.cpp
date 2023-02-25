#include "ArgumentView.h"

#include <QComboBox>

// view

ArgumentView::ArgumentView(QWidget* parent)
   : QTreeView(parent)
   , FunctionHub()
   , argumentList()
   , argumentModel(nullptr)
   , nameEditable(false)
{
   setRootIsDecorated(false);

   argumentModel = new QStandardItemModel(this);
   connect(argumentModel, &QStandardItemModel::itemChanged, this, &ArgumentView::slotItemChanged);
   setModel(argumentModel);

   setItemDelegateForColumn(2, new TypeDelegate(this));
}

void ArgumentView::allowNameEdit(bool allow)
{
   nameEditable = allow;
}

void ArgumentView::clearMonitors()
{
   argumentList.clear();
   argumentModel->clear();
   argumentModel->setHorizontalHeaderLabels({"O", "Name", "Type"});
}

void ArgumentView::monitor(PatchStructure::Argument* argument)
{
   QStandardItem* optionalItem = new QStandardItem();
   optionalItem->setCheckable(true);
   optionalItem->setCheckState(argument->optional ? Qt::Checked : Qt::Unchecked);

   QStandardItem* nameItem = new QStandardItem(argument->name);
   nameItem->setEditable(nameEditable);

   QStandardItem* typeItem = new QStandardItem(PatchStructure::typeName(argument->type));

   argumentModel->appendRow({optionalItem, nameItem, typeItem});
   argumentList.append(argument);

   resizeColumnToContents(0);
   resizeColumnToContents(1);
}

void ArgumentView::slotItemChanged(QStandardItem* item)
{
   const int index = item->row();
   PatchStructure::Argument* argument = argumentList[index];

   if (0 == item->column())
   {
      argument->optional = (item->checkState() == Qt::Checked);
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true);
   }
   else if (1 == item->column())
   {
      const QString& name = item->text();
      if (name != argument->name)
      {
         argument->name = name;
         FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true);
      }
   }
   else if (2 == item->column())
   {
      const PatchStructure::Type type = PatchStructure::toType(item->text());
      if (type != argument->type)
      {
         argument->type = type;
         FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true);
      }
   }
}

// delegate

TypeDelegate::TypeDelegate(ArgumentView* view)
   : QStyledItemDelegate(view)
   , view(view)
{
}

QWidget* TypeDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QComboBox* comboBox = new QComboBox(parent);
   comboBox->setFrame(false);
   for (const PatchStructure::Type& type : PatchStructure::typeList())
   {
      comboBox->addItem(PatchStructure::typeName(type));
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

   const PatchStructure::Type type = view->argumentList.at(index.row())->type;
   const int typeIndex = comboBox->findText(PatchStructure::typeName(type));
   comboBox->setCurrentIndex(typeIndex);
}

void TypeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
   const QString typeText = comboBox->currentText();

   model->setData(index, comboBox->currentText(), Qt::DisplayRole);
}

void TypeDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(index)

   editor->setGeometry(option.rect);
}
