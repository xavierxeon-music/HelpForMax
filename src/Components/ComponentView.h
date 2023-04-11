#ifndef ComponentsViewH
#define ComponentsViewH

#include "Tools/AbstractItemTreeView.h"

#include "Tools/Central.h"

class MainWindow;
class ComponentsModel;

class ComponentsView : public Abstract::ItemTreeView,
                       private FunctionHub
{
   Q_OBJECT

public:
   ComponentsView(MainWindow* mainWindow, ComponentsModel* model);

public slots:
   void slotReloadPatch();
   void slotOpenInExternalEditor();

private:
   struct Clip
   {
      Block::Item::Marker marker; // what are we copying?

      Block::Structure::Digest digest;
      Block::Structure::Type type;

      Clip(const Block::Item::Marker& marker = Block::Item::Marker::Undefined);
   };

private slots:
   void slotCopy();
   void slotPaste();

private:
   void patchSelected(QString patchPath, QString key) override;
   void clicked(ModelItem* item) override;
   void setModified(bool modified, QString key) override;
   void contextMenuEvent(QContextMenuEvent* event) override;
   QStandardItem* actionItem();

private:
   MainWindow* mainWindow;
   QString helpPath;
   QAction* copyAction;
   QAction* pasteAction;
   Clip clip;
};

#endif // NOT ComponentsViewH
