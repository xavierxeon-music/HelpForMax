#ifndef EditWidgetH
#define EditWidgetH

#include <QStackedWidget>

#include "Edit/PageAbstract.h"
#include "PatchParser.h"

namespace Edit
{
   class Widget : public QStackedWidget,
                  private FunctionHub
   {
      Q_OBJECT

   public:
      Widget(MainWindow* mainWindow);

   private:
      using EditorMap = QMap<PatchParser::Marker, Page::Abstract*>;

   private:
      template <typename EditorType>
      void addEditor(const PatchParser::Marker& marker);

      void patchSelected(QString patchPath, QString key) override;
      void componentSelected(PatchParser::Marker marker, QVariant data) override;

   private:
      MainWindow* mainWindow;
      EditorMap editorMap;
   };
} // namespace Edit

#endif // NOT EditWidgetH
