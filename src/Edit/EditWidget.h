#ifndef EditWidgetH
#define EditWidgetH

#include <QStackedWidget>

#include "Edit/PageAbstract.h"

namespace Edit
{
   class Widget : public QStackedWidget,
                  private FunctionHub
   {
      Q_OBJECT

   public:
      Widget(QWidget* parent, Central* central);

   signals:
      void signalShowMetaTags();
      void signalShowSeeAlso();

   private:
      using EditorMap = QMap<Block::Item::Marker, Page::Abstract*>;

   private:
      template <typename EditorType>
      EditorType* addEditor(const Block::Item::Marker& marker);

      void patchSelected(QString patchPath, QString key) override;
      void componentSelected(Block::Item::Marker marker, QVariant data) override;

   private:
      Central* central;
      EditorMap editorMap;
   };
} // namespace Edit

#endif // NOT EditWidgetH
