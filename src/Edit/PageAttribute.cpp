#include "PageAttribute.h"

#include "MainWindow.h"

Page::Attribute::Attribute(MainWindow* mainWindow, const PatchParser::Marker& marker)
   : Abstract(mainWindow, marker)
   , TypeDelegate::Proxy()
   , highlighter(nullptr)
   , attributeName()
   , attributeModel(nullptr)
{
   setupUi(this);
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

   PatchStructure::Attribute& attribute = mainWindow->parserRef().attributeMap[attributeName];
   if (0 == item->column())
   {
      attribute.get = (item->checkState() == Qt::Checked);
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true);
   }
   else if (1 == item->column())
   {
      attribute.set = (item->checkState() == Qt::Checked);
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true);
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
            FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true);
         }
      }
      else
      {
         item->setText(QString::number(attribute.size));
      }
   }
   else if (3 == item->column())
   {
      const PatchStructure::Type type = PatchStructure::toType(item->text());
      if (type != attribute.type)
      {
         attribute.type = type;
         FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true);
      }
   }

   attributeView->resizeColumnToContents(0);
   attributeView->resizeColumnToContents(1);
   attributeView->resizeColumnToContents(2);
}

void Page::Attribute::update(const QVariant& data)
{
   attributeName = data.toString();
   PatchStructure::Attribute& attribute = mainWindow->parserRef().attributeMap[attributeName];
   keyInfo->setText("attribute " + attributeName + " @ " + mainWindow->getCurrentKey());

   monitor(digestEdit, &attribute.digest.text);
   monitor(descrptionEdit, &attribute.digest.description);

   QStandardItem* getItem = attributeModel->invisibleRootItem()->child(0, 0);
   getItem->setCheckState(attribute.get ? Qt::Checked : Qt::Unchecked);

   QStandardItem* setItem = attributeModel->invisibleRootItem()->child(0, 1);
   setItem->setCheckState(attribute.set ? Qt::Checked : Qt::Unchecked);

   QStandardItem* sizeItem = attributeModel->invisibleRootItem()->child(0, 2);
   sizeItem->setText(QString::number(attribute.size));

   QStandardItem* typeItem = attributeModel->invisibleRootItem()->child(0, 3);
   typeItem->setText(PatchStructure::typeName(attribute.type));

   attributeView->resizeColumnToContents(0);
   attributeView->resizeColumnToContents(1);
   attributeView->resizeColumnToContents(2);
}

PatchStructure::Type Page::Attribute::getType(const int index)
{
   Q_UNUSED(index);

   const PatchStructure::Attribute& attribute = mainWindow->parser().attributeMap.value(attributeName);
   return attribute.type;
}
