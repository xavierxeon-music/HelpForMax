#ifndef ComponentViewH
#define ComponentViewH

#include "Tools/AbstractItemTreeView.h"
#include "Tools/Central.h"

namespace Component
{
   class Model;

   class View : public Abstract::ItemTreeView,
                private FunctionHub
   {
      Q_OBJECT

   public:
      View(QWidget* parent, Central* central, Model* model);

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
      Central* central;
      QString helpPath;
      QAction* copyAction;
      QAction* pasteAction;
      Clip clip;
   };
} // namespace Component

#endif // NOT ComponentViewH
