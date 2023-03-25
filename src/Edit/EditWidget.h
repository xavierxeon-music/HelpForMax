#ifndef EditWidgetH
#define EditWidgetH

#include <QStackedWidget>

#include "Edit/PageAbstract.h"
#include "Tools/PatchParser.h"

namespace Edit
{
   class Widget : public QStackedWidget,
                  private FunctionHub
   {
      Q_OBJECT

   public:
      Widget(MainWindow* mainWindow);

   private:
      using EditorMap = QMap<Block::Item::Marker, Page::Abstract*>;

   private:
      template <typename EditorType>
      void addEditor(const Block::Item::Marker& marker);

      void patchSelected(QString patchPath, QString key) override;
      void componentSelected(Block::Item::Marker marker, QVariant data) override;

   private:
      MainWindow* mainWindow;
      EditorMap editorMap;
   };
} // namespace Edit

#endif // NOT EditWidgetH
