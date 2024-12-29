#ifndef PatchWidgetH
#define PatchWidgetH

#include <QSplitter>

#include <QStackedWidget>

#include "MaxWidget.h"
#include "PatchContainer.h"
#include "PatchInfo.h"
#include "PatchRefModelArgument.h"
#include "PatchRefModelHeader.h"
#include "PatchRefModelNamedMessage.h"
#include "PatchRefModelOutput.h"
#include "PatchRefModelTypedMessage.h"
#include "PatchRefWidget.h"
#include "SuggestModelArgument.h"
#include "SuggestModelNamedMessage.h"
#include "SuggestModelTypedMessage.h"
#include "SuggestWidget.h"

namespace Patch
{
   class Widget : public QSplitter
   {
      Q_OBJECT

   public:
      Widget(Container* container, const Package::Info* packageInfo, const QString& patchFileName);
      ~Widget();

   public:
      const QString& getPath() const;
      const Package::Info* getPacakgeInfo() const;
      const Info& getPatchInfo() const;
      virtual void writeRef();
      void openInMax();
      void openXML();
      void tranferAllSuggestions();
      bool isDirty() const;
      void setToolsVisible(Container::ToolsVisible toolsVisible);

   private:
      friend class PatchRef::Widget;
      friend class Suggest::Widget;

   private:
      void setDirty();
      void propagateDirty(bool isDirty);

   private:
      Container* container;
      QStackedWidget* stackWidget;
      PatchRef::Widget* refWidget;
      Suggest::Widget* suggestWidget;
      Max::Widget* structureWidget;

      Ref::Structure maxRef;
      Max::Patcher maxPatch;

      PatchRef::Model::Header* headerPatchModel;
      PatchRef::Model::Argument* argumentPatchModel;
      PatchRef::Model::TypedMessage* typedMessagPatcheModel;
      PatchRef::Model::NamedMessage* namedMessagePatchModel;
      PatchRef::Model::Output* outputPatchModel;

      Suggest::Model::Argument* argumentSuggestModel;
      Suggest::Model::NamedMessage* namedMessageSuggestModel;
      Suggest::Model::TypedMessage* typedMessageSuggestModel;

      const Package::Info* packageInfo;
      QString path;
      Patch::Info patchInfo;

      bool dirty;
   };
} // namespace Patch

#endif // NOT PatchWidgetH
