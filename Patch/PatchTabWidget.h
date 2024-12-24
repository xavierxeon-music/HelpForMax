#ifndef PatchTabWidgetH
#define PatchTabWidgetH

#include "RecentTabWidget.h"

#include <QMenu>
#include <QToolBar>

#include "Locker.h"
#include "MaxPatcher.h"

namespace Package
{
   class Info;
}

namespace Patch
{
   class Widget;

   class TabWidget : public RecentTabWidget
   {
      Q_OBJECT

   public:
      enum class ToolVisibility
      {
         None = 0x00,
         Suggestions = 0x01,
         Structure = 0x02
      };
      Q_ENUM(ToolVisibility)
      Q_DECLARE_FLAGS(ToolsVisible, ToolVisibility)

      using SplitterLocker = Locker<"Splitter">;

   public:
      TabWidget(QWidget* parent);

   signals:
      void signalCheckDirty();

   public:
      void createActions();
      void init();
      void emitSignalCheckDirty();
      const ToolsVisible& getToolsVisible() const;

   signals:
      void signalRefWritten(const QString& patchPath);

   public slots:
      void slotShowPatch(const QString& patchFileName);
      void slotCloseAllPatches(const Package::Info* packageInfo);

   private slots:
      void slotPromptLoadPatch();
      void slotWriteRef();
      void slotClosePatch();
      void slotWriteAllRefs();
      void slotOpenInMax();
      void slotOpenXML();
      void slotShowSuggestions(bool enabled);
      void slotShowStructure(bool enabled);
      void slotTabSplitterChanged();

   private:
      Entry creatreEntry(const QFileInfo& fileInfo) override;
      void updateTabNames();
      void toggleVisibility(bool enabled, const ToolVisibility& value);
      void writeSettings();

   private:
      ToolsVisible toolsVisible;
      QList<int> splitterSizes;
   };
} // namespace Patch

Q_DECLARE_OPERATORS_FOR_FLAGS(Patch::TabWidget::ToolsVisible)

#endif // NOT PatchTabWidgetH
