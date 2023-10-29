#include "PagePatch.h"

#include <QButtonGroup>

Page::Patch::Patch(QWidget* parent, Central* central, const Block::Marker& marker)
   : Abstract(parent, central, marker)
   , highlighter(nullptr)
   , standardMethodGroup(nullptr)
{
   setupUi(this);
   layout()->setContentsMargins(0, 0, 0, 0);

   highlighter = new DescriptionHighlighter(descrptionEdit->document());

   keyInfo->setText("PATCH");

   standardMethodGroup = new QButtonGroup(this);
   standardMethodGroup->addButton(messageBangButton, static_cast<int>(Structure::Type::Bang));
   standardMethodGroup->addButton(messageIntButton, static_cast<int>(Structure::Type::Integer));
   standardMethodGroup->addButton(messageFloatButton, static_cast<int>(Structure::Type::Float));
   standardMethodGroup->addButton(messageListButton, static_cast<int>(Structure::Type::List));
   standardMethodGroup->addButton(messageSignalButton, static_cast<int>(Structure::Type::Signal));
   standardMethodGroup->addButton(messageMultiSignalButton, static_cast<int>(Structure::Type::MultiSignal));
   standardMethodGroup->addButton(messageAnythingButton, static_cast<int>(Structure::Type::Anything));
   standardMethodGroup->addButton(messageMatrixButton, static_cast<int>(Structure::Type::Matrix));
   standardMethodGroup->addButton(messageDictButton, static_cast<int>(Structure::Type::Dictionary));

   connect(standardMethodGroup, &QButtonGroup::idClicked, this, &Patch::slotAddStandardMethond);

   selectMetaTagsButton->setIcon(QIcon(":/ArrowLeft.svg"));
   selectSeeAlsoButton->setIcon(QIcon(":/ArrowLeft.svg"));

   selectMetaTagsButton->hide();

   connect(selectMetaTagsButton, &QToolButton::clicked, this, &Patch::signalShowMetaTags);
   connect(selectSeeAlsoButton, &QToolButton::clicked, this, &Patch::signalShowSeeAlso);
}

void Page::Patch::slotAddStandardMethond(int typeId)
{
   const Structure::Type type = static_cast<Structure::Type>(typeId);
   if (central->block().messageStandardMap.contains(type))
      return;

   Structure::Argument argument;
   argument.type = type;

   Structure::Message message;
   message.arguments.append(argument);

   central->blockRef().messageStandardMap[type] = message;
   callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
}

void Page::Patch::update(const QVariant& data)
{
   Q_UNUSED(data)

   keyInfo->setText("patch @ " + central->getCurrentKey());
   monitor(openAsCombo, (int*)&central->blockRef().patch.patcherType, central->getCurrentKey());
   monitor(metaTagEdit, &central->blockRef().patch.metaTagList, central->getCurrentKey());
   monitor(digestEdit, &central->blockRef().patch.digest.text, central->getCurrentKey());
   monitor(descrptionEdit, &central->blockRef().patch.digest.description, central->getCurrentKey());
   monitor(seeAlsoEdit, &central->blockRef().patch.seeAlsoList, central->getCurrentKey());
}
