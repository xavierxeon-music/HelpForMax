#include "PagePatch.h"

#include <QButtonGroup>

#include "MainWindow.h"

Page::Patch::Patch(MainWindow* mainWindow, const Block::Item::Marker& marker)
   : Abstract(mainWindow, marker)
   , highlighter(nullptr)
   , standardMethodGroup(nullptr)
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("PATCH");

   standardMethodGroup = new QButtonGroup(this);
   standardMethodGroup->addButton(messageBangButton, static_cast<int>(Block::Structure::Type::Bang));
   standardMethodGroup->addButton(messageIntButton, static_cast<int>(Block::Structure::Type::Integer));
   standardMethodGroup->addButton(messageFloatButton, static_cast<int>(Block::Structure::Type::Float));
   standardMethodGroup->addButton(messageListButton, static_cast<int>(Block::Structure::Type::List));
   standardMethodGroup->addButton(messageSignalButton, static_cast<int>(Block::Structure::Type::Signal));

   connect(standardMethodGroup, &QButtonGroup::idClicked, this, &Patch::slotAddStandardMethond);
}

void Page::Patch::slotAddStandardMethond(int typeId)
{
   const Block::Structure::Type type = static_cast<Block::Structure::Type>(typeId);
   if (mainWindow->block().messageStandardMap.contains(type))
      return;

   Block::Structure::Argument argument;
   argument.type = type;

   Block::Structure::Message message;
   message.arguments.append(argument);

   mainWindow->blockRef().messageStandardMap[type] = message;
   callOnOtherHubInstances(&Central::setModified, true, mainWindow->getCurrentKey());
}

void Page::Patch::update(const QVariant& data)
{
   Q_UNUSED(data)

   keyInfo->setText("patch @ " + mainWindow->getCurrentKey());
   monitor(openAsBPatcherCheck, &mainWindow->blockRef().extras.openAsBPatcher, mainWindow->getCurrentKey());
   monitor(metaTagEdit, &mainWindow->blockRef().extras.metaTagList, mainWindow->getCurrentKey());
   monitor(digestEdit, &mainWindow->blockRef().patchDigest.text, mainWindow->getCurrentKey());
   monitor(descrptionEdit, &mainWindow->blockRef().patchDigest.description, mainWindow->getCurrentKey());
   monitor(seeAlsoEdit, &mainWindow->blockRef().extras.seeAlsoList, mainWindow->getCurrentKey());
}
