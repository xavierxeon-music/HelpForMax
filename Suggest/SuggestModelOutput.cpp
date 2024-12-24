#include "SuggestModelOutput.h"

Suggest::Model::Output::Output(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest)
   : Abstract(parent, structure, suggest, Ref::Structure::PatchPart::Output)
{
}

void Suggest::Model::Output::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Type", "Active"});

   while (0 < invisibleRootItem()->rowCount())
      invisibleRootItem()->removeRow(0);

   for (const Max::DataType& type : Max::dataTypeList())
   {
      const Ref::Structure::Output& output = suggest.outputMap[type];

      QStandardItem* typeItem = new QStandardItem(Max::dataTypeName(type));
      typeItem->setEditable(false);
      typeItem->setData(QVariant::fromValue(type));

      QStandardItem* activeItem = new QStandardItem();
      activeItem->setEditable(false);
      activeItem->setCheckable(false);
      activeItem->setIcon(QIcon(":/DocOutput.svg"));
      activeItem->setCheckState(output.active ? Qt::Checked : Qt::Unchecked);

      invisibleRootItem()->appendRow({typeItem, activeItem});
   }

   endResetModel();
}
