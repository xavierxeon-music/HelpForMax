#include "SuggestModelArgument.h"

Suggest::Model::Argument::Argument(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest)
   : Abstract(parent, structure, suggest, Ref::Structure::PatchPart::Argument)
{
}

void Suggest::Model::Argument::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Name", "Type"});

   while (0 < invisibleRootItem()->rowCount())
      invisibleRootItem()->removeRow(0);

   for (int row = 0; row < suggest.argumentList.count(); row++)
   {
      const Ref::Structure::Argument& argument = suggest.argumentList.at(row);

      QStandardItem* nameItem = new QStandardItem();
      nameItem->setEditable(false);
      nameItem->setText(argument.name);

      QStandardItem* typeItem = new QStandardItem();
      nameItem->setEditable(false);
      typeItem->setText(Max::dataTypeName(argument.dataType));

      invisibleRootItem()->appendRow({nameItem, typeItem});
   }

   endResetModel();
}
