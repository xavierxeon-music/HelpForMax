#include "ArgumentView.h"

ArgumentView::ArgumentView(QWidget* parent)
   : QTreeView(parent)
   , TypeDelegate::Proxy()
   , FunctionHub()
   , key()
   , argumentList()
   , argumentModel(nullptr)
   , nameEditable(false)
{
   setRootIsDecorated(false);

   argumentModel = new QStandardItemModel(this);
   connect(argumentModel, &QStandardItemModel::itemChanged, this, &ArgumentView::slotItemChanged);
   setModel(argumentModel);

   setItemDelegateForColumn(2, new TypeDelegate(this, this));
}

void ArgumentView::allowNameEdit(bool allow)
{
   nameEditable = allow;
}

void ArgumentView::clearMonitors(const QString& patchKey)
{
   key = patchKey;

   argumentList.clear();
   argumentModel->clear();
   argumentModel->setHorizontalHeaderLabels({"O", "Name", "Type"});
}

void ArgumentView::monitor(Block::Structure::Argument* argument)
{
   QStandardItem* optionalItem = new QStandardItem();
   optionalItem->setCheckable(true);
   optionalItem->setCheckState(argument->optional ? Qt::Checked : Qt::Unchecked);

   QStandardItem* nameItem = new QStandardItem(argument->name);
   nameItem->setEditable(nameEditable);

   QStandardItem* typeItem = new QStandardItem(Block::Structure::typeName(argument->type));

   argumentModel->appendRow({optionalItem, nameItem, typeItem});
   argumentList.append(argument);

   resizeColumnToContents(0);
   resizeColumnToContents(1);
}

void ArgumentView::slotItemChanged(QStandardItem* item)
{
   const int index = item->row();
   Block::Structure::Argument* argument = argumentList[index];

   if (0 == item->column())
   {
      argument->optional = (item->checkState() == Qt::Checked);
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, key);
   }
   else if (1 == item->column())
   {
      const QString& name = item->text();
      if (name != argument->name)
      {
         argument->name = name;
         FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, key);
      }
   }
   else if (2 == item->column())
   {
      const Block::Structure::Type type = Block::Structure::toType(item->text());
      if (type != argument->type)
      {
         argument->type = type;
         FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, key);
      }
   }
}

Block::Structure::Type ArgumentView::getType(const int index)
{
   return argumentList.at(index)->type;
}
