#ifndef ResultWidgetH
#define ResultWidgetH

#include "Tools/AbstractWidget.h"

#include <QTextEdit>

namespace Result
{
   class Widget : public Abstract::Widget,
                  private FunctionHub
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent, Central* central);

   private slots:
      void slotApplyChanges();

   private:
      void patchSelected(QString patchPath, QString key) override;

   private:
      QTextEdit* textEdit;
   };
} // namespace Result

#endif // NOT ResultWidgetH
