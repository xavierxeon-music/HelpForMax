#ifndef ComponentWidgetH
#define ComponentWidgetH

#include <QSplitter>

#include "Tools/AbstractWidget.h"

#include "PageAbstract.h"

class QStackedWidget;

namespace Component
{
   class Widget : public Abstract::Widget,
                  public FunctionHub
   {
   public:
      Widget(QWidget* parent, Central* central);

   public slots:
      void slotShowComponents();
      void slotShowMetaTags();
      void slotShowSeeAlso();

   private:
      using EditorMap = QMap<Block::Item::Marker, Page::Abstract*>;

   private slots:
      void slotSavePatches();

   private:
      void patchSelected(QString patchPath, QString key) override;
      void componentSelected(Block::Item::Marker marker, QVariant data) override;
      void restoreState() override;
      void saveState() override;

      void setComponentStack(const int& index);
      void setupComponents();

      void setupEdit();

      template <typename EditorType>
      EditorType* addEditor(const Block::Item::Marker& marker);

   private:
      QSplitter* splitter;
      QStackedWidget* comonentStackWidget;
      QStackedWidget* editStackWidget;
      EditorMap editorMap;
   };
} // namespace Component

#endif // NOT ComponentWidgetH
