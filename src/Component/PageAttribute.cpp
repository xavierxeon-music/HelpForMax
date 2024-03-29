#include "PageAttribute.h"

#include "Tools/Lock.h"

Page::Attribute::Attribute(QWidget* parent, Central* central, const Block::Marker& marker)
   : Abstract(parent, central, marker)
   , TypeDelegate::Proxy()
   , highlighter(nullptr)
   , attributeName()
   , attributeModel(nullptr)
   , updating(false)
{
   setupUi(this);
   layout()->setContentsMargins(0, 0, 0, 0);

   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("ATTRIBUTE");

   attributeModel = new QStandardItemModel(this);
   attributeModel->setHorizontalHeaderLabels({"Get", "Set", "Size", "Type"});

   QStandardItem* getItem = new QStandardItem();
   getItem->setEditable(false);
   getItem->setCheckable(true);

   QStandardItem* setItem = new QStandardItem();
   setItem->setEditable(false);
   setItem->setCheckable(true);

   QStandardItem* sizeItem = new QStandardItem();
   QStandardItem* typeItem = new QStandardItem();

   attributeModel->appendRow({getItem, setItem, sizeItem, typeItem});

   connect(attributeModel, &QStandardItemModel::itemChanged, this, &Attribute::slotItemChanged);
   attributeView->setModel(attributeModel);
   attributeView->setRootIsDecorated(false);
   attributeView->setItemDelegateForColumn(3, new TypeDelegate(this, this));
}

void Page::Attribute::slotItemChanged(QStandardItem* item)
{
   if (0 != item->row())
      return;

   if (updating)
      return;

   Structure::Attribute& attribute = central->blockRef().attributeMap[attributeName];
   if (0 == item->column())
   {
      attribute.get = (item->checkState() == Qt::Checked);
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
   }
   else if (1 == item->column())
   {
      attribute.set = (item->checkState() == Qt::Checked);
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
   }
   else if (2 == item->column())
   {
      const QString text = item->text();
      bool isNumber = false;
      const int number = text.toInt(&isNumber);

      if (isNumber)
      {
         if (number != attribute.size)
         {
            attribute.size = number;
            FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
         }
      }
      else
      {
         item->setText(QString::number(attribute.size));
      }
   }
   else if (3 == item->column())
   {
      const Structure::Type type = Structure::toType(item->text());
      if (type != attribute.type)
      {
         attribute.type = type;
         FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
      }
   }

   attributeView->resizeColumnToContents(0);
   attributeView->resizeColumnToContents(1);
   attributeView->resizeColumnToContents(2);
}

void Page::Attribute::update(const QVariant& data)
{
   attributeName = data.toString();
   Structure::Attribute& attribute = central->blockRef().attributeMap[attributeName];
   keyInfo->setText("attribute " + attributeName + " @ " + central->getCurrentKey());

   monitor(digestEdit, &attribute.digest.text, central->getCurrentKey());
   monitor(descrptionEdit, &attribute.digest.description, central->getCurrentKey());

   {
      Lock lock(updating);

      QStandardItem* getItem = attributeModel->invisibleRootItem()->child(0, 0);
      getItem->setCheckState(attribute.get ? Qt::Checked : Qt::Unchecked);

      QStandardItem* setItem = attributeModel->invisibleRootItem()->child(0, 1);
      setItem->setCheckState(attribute.set ? Qt::Checked : Qt::Unchecked);

      QStandardItem* sizeItem = attributeModel->invisibleRootItem()->child(0, 2);
      sizeItem->setText(QString::number(attribute.size));

      QStandardItem* typeItem = attributeModel->invisibleRootItem()->child(0, 3);
      typeItem->setText(Structure::typeName(attribute.type));
   }

   attributeView->resizeColumnToContents(0);
   attributeView->resizeColumnToContents(1);
   attributeView->resizeColumnToContents(2);
}

Structure::Type Page::Attribute::getType(const int index)
{
   Q_UNUSED(index);

   const Structure::Attribute& attribute = central->block().attributeMap.value(attributeName);
   return attribute.type;
}
