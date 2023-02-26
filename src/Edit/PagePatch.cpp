#include "PagePatch.h"

#include <QButtonGroup>

#include "MainWindow.h"

Page::Patch::Patch(MainWindow* mainWindow, const PatchParser::Marker& marker)
   : Abstract(mainWindow, marker)
   , highlighter(nullptr)
   , standardMethodGroup(nullptr)
{
   setupUi(this);
   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("PATCH");

   standardMethodGroup = new QButtonGroup(this);
   standardMethodGroup->addButton(messageBangButton, static_cast<int>(PatchStructure::Type::Bang));
   standardMethodGroup->addButton(messageIntButton, static_cast<int>(PatchStructure::Type::Integer));
   standardMethodGroup->addButton(messageFloatButton, static_cast<int>(PatchStructure::Type::Float));
   standardMethodGroup->addButton(messageListButton, static_cast<int>(PatchStructure::Type::List));
   standardMethodGroup->addButton(messageSignalButton, static_cast<int>(PatchStructure::Type::Signal));

   connect(standardMethodGroup, &QButtonGroup::idClicked, this, &Patch::slotAddStandardMethond);
}

void Page::Patch::slotAddStandardMethond(int typeId)
{
   const PatchStructure::Type type = static_cast<PatchStructure::Type>(typeId);
   if (mainWindow->parser().messageStandardMap.contains(type))
      return;

   PatchStructure::Argument argument;
   argument.type = type;

   PatchStructure::Message message;
   message.arguments.append(argument);

   mainWindow->parserRef().messageStandardMap[type] = message;
   callOnOtherHubInstances(&Central::setModified, true);
}

void Page::Patch::update(const QVariant& data)
{
   Q_UNUSED(data)

   keyInfo->setText("patch @ " + mainWindow->getCurrentKey());
   monitor(metaTagEdit, &mainWindow->parserRef().metaTagList);
   monitor(digestEdit, &mainWindow->parserRef().patchDigest.text);
   monitor(descrptionEdit, &mainWindow->parserRef().patchDigest.description);
   monitor(seeAlsoEdit, &mainWindow->parserRef().seeAlsoList);
}
