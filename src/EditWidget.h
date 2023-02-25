#ifndef HelpEditWidgetH
#define HelpEditWidgetH

#include <QStackedWidget>

#include "HelpPageAbstract.h"
#include "HelpPatchParser.h"

namespace Help
{
   class Persona;

   namespace Edit
   {
      class Widget : public QStackedWidget,
                     private Persona::FunctionHub
      {
         Q_OBJECT

      public:
         Widget(Persona* persona);

      private:
         using EditorMap = QMap<PatchParser::Marker, Page::Abstract*>;

      private:
         template <typename EditorType>
         void addEditor(const PatchParser::Marker& marker);

         void patchSelected(QString patchPath, QString key) override;
         void componentSelected(PatchParser::Marker marker, QVariant data) override;

      private:
         Persona* persona;
         EditorMap editorMap;
      };
   } // namespace Edit
} // namespace Help

#endif // NOT HelpEditWidgetH
