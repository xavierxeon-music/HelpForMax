#include "SuggestModelOutput.h"

Suggest::Model::Output::Output(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest)
   : Abstract(parent, structure, suggest, Ref::Structure::PatchPart::Output)
{
}

void Suggest::Model::Output::update()
{
   emit signalDataEdited();
}

void Suggest::Model::Output::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Type", "Active"});

   removeContent();

   for (const Max::DataType& type : Max::dataTypeList())
   {
      const Ref::Structure::Output& output = suggest.outputMap[type];

      QStandardItem* typeItem = new QStandardItem(Max::dataTypeName(type));
      typeItem->setEditable(false);
      typeItem->setData(QVariant::fromValue(type), DataMarker);

      QStandardItem* activeItem = new QStandardItem();
      activeItem->setEditable(false);
      activeItem->setCheckable(false);
      activeItem->setIcon(QIcon(":/DocOutput.svg"));
      activeItem->setCheckState(output.active ? Qt::Checked : Qt::Unchecked);

      invisibleRootItem()->appendRow({typeItem, activeItem});
   }

   endResetModel();
   update();
}

void Suggest::Model::Output::buildStructure()
{
   qDebug() << __FUNCTION__;
}

void Suggest::Model::Output::transfer(const QList<int>& rowList)
{
   qDebug() << __FUNCTION__ << rowList;
}
