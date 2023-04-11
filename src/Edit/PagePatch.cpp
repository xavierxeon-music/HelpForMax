#include "PagePatch.h"

#include <QButtonGroup>

Page::Patch::Patch(QWidget* parent, Central* central, const Block::Item::Marker& marker)
   : Abstract(parent, central, marker)
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
   standardMethodGroup->addButton(messageMultiSignalButton, static_cast<int>(Block::Structure::Type::MultiSignal));
   standardMethodGroup->addButton(messageAnythingButton, static_cast<int>(Block::Structure::Type::Anything));

   connect(standardMethodGroup, &QButtonGroup::idClicked, this, &Patch::slotAddStandardMethond);
}

void Page::Patch::slotAddStandardMethond(int typeId)
{
   const Block::Structure::Type type = static_cast<Block::Structure::Type>(typeId);
   if (central->block().messageStandardMap.contains(type))
      return;

   Block::Structure::Argument argument;
   argument.type = type;

   Block::Structure::Message message;
   message.arguments.append(argument);

   central->blockRef().messageStandardMap[type] = message;
   callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
}

void Page::Patch::update(const QVariant& data)
{
   Q_UNUSED(data)

   keyInfo->setText("patch @ " + central->getCurrentKey());
   monitor(openAsBPatcherCheck, &central->blockRef().patch.openAsBPatcher, central->getCurrentKey());
   monitor(metaTagEdit, &central->blockRef().patch.metaTagList, central->getCurrentKey());
   monitor(digestEdit, &central->blockRef().patch.digest.text, central->getCurrentKey());
   monitor(descrptionEdit, &central->blockRef().patch.digest.description, central->getCurrentKey());
   monitor(seeAlsoEdit, &central->blockRef().patch.seeAlsoList, central->getCurrentKey());
}
