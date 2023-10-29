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
      using EditorMap = QMap<Block::Marker, Page::Abstract*>;

   private slots:
      void slotSavePatches();

   private:
      void patchSelected(QString patchPath, QString key) override;
      void componentSelected(Block::Marker marker, QVariant data) override;
      void restoreState() override;
      void saveState() override;

      void setComponentStack(const int& index);
      void setupComponents();

      void setupEdit();

      template <typename EditorType>
      EditorType* addEditor(const Block::Marker& marker);

   private:
      QSplitter* splitter;
      QStackedWidget* comonentStackWidget;
      QStackedWidget* editStackWidget;
      EditorMap editorMap;
   };
} // namespace Component

#endif // NOT ComponentWidgetH
