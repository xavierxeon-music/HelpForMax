#include "SuggestWidget.h"

#include <QTimer>

#include "PatchWidget.h"

Suggest::Widget::Widget(Patch::Widget* patchWidget)
   : QWidget(patchWidget)
   , patchWidget(patchWidget)
   , refTreeViewList()
{
   setupUi(this);

   static const QString transferArrow = QString::fromUtf8("\u27a4");
   static const QString styleSheet("text-align:left;");

   transferAllButton->setText("Transfer All " + transferArrow);
   transferAllButton->setStyleSheet(styleSheet);
   transferAllButton->setIcon(QIcon(":/PatchTransferAll.svg"));
   connect(transferAllButton, &QAbstractButton::clicked, this, &Widget::slotTransferAll);

   transferSelectedButton->setText("Transfer Selected " + transferArrow);
   transferSelectedButton->setStyleSheet(styleSheet);
   transferSelectedButton->setIcon(QIcon(":/PatchTransferSelected.svg"));
   connect(transferSelectedButton, &QAbstractButton::clicked, this, &Widget::slotTransferSelected);

   auto setIcon = [&](QLabel* iconLabel, Ref::Structure::PatchPart part)
   {
      iconLabel->setPixmap(patchWidget->maxRef.partIcon(part).pixmap(16, 16));
   };

   setIcon(argumentIcon, Ref::Structure::PatchPart::Argument);
   setIcon(typedMessageIcon, Ref::Structure::PatchPart::MessageTyped);
   setIcon(attributeIcon, Ref::Structure::PatchPart::Attribute);
   setIcon(nameMessageIcon, Ref::Structure::PatchPart::MessageNamed);

   // set suggest models
   argumentSuggestTree->init(patchWidget->argumentSuggestModel);
   typedMessageSuggestTree->init(patchWidget->typedMessageSuggestModel);
   namedMessageSuggestTree->init(patchWidget->namedMessageSuggestModel);

   // set reference models
   auto prepareRefTree = [&](QTreeView* treeView, PatchRef::Model::Abstract* model)
   {
      treeView->setModel(model);
      treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
      treeView->setRootIsDecorated(false);
      treeView->setUniformRowHeights(true);

      refTreeViewList.append(treeView);

      connect(model, &PatchRef::Model::Abstract::signalDataEdited, this, &Widget::slotResizeRefColumns);
   };

   prepareRefTree(argumentPatchTree, patchWidget->argumentPatchModel);
   prepareRefTree(typedMessagePatchTree, patchWidget->typedMessagPatcheModel);
   prepareRefTree(namedMessagePatchTree, patchWidget->namedMessagePatchModel);
}

void Suggest::Widget::update()
{
   Model::Abstract::callOnAllInstances(patchWidget, &RefModel::update);
}

void Suggest::Widget::rebuild()
{
   Model::Abstract::callOnAllInstances(patchWidget, &RefModel::rebuild);
}

void Suggest::Widget::slotTransferAll()
{
   for (TreeView* treeView : {argumentSuggestTree, typedMessageSuggestTree, namedMessageSuggestTree})
      treeView->transferAll();

   QString& shortText = patchWidget->maxRef.header.digest.shortText;
   if ("Hello World" == shortText)
   {
      QString name = patchWidget->getPatchInfo().name;
      name.replace("wa.", "");
      name.replace(".", " ");
      name.replace("_", " ");
      shortText = name;
   }

   PatchRef::Model::Abstract::callOnAllInstances(patchWidget, &RefModel::rebuild);
   Model::Abstract::callOnAllInstances(patchWidget, &RefModel::update);
}

void Suggest::Widget::slotTransferSelected()
{
   for (TreeView* treeView : {argumentSuggestTree, typedMessageSuggestTree, namedMessageSuggestTree})
      treeView->transferSelected();

   PatchRef::Model::Abstract::callOnAllInstances(patchWidget, &RefModel::rebuild);
   Model::Abstract::callOnAllInstances(patchWidget, &RefModel::update);
}

void Suggest::Widget::slotResizeRefColumns()
{
   auto resizeIternal = [this]()
   {
      for (QTreeView* treeView : refTreeViewList)
      {
         for (int col = 0; col < treeView->model()->columnCount(); col++)
         {
            treeView->resizeColumnToContents(col);
         }
      }
   };

   QTimer::singleShot(10, this, resizeIternal);
}
