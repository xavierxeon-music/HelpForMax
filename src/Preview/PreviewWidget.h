#ifndef PreviewWidgetH
#define PreviewWidgetH

#include <QWidget>

#include <QGraphicsScene>

#include "Tools/Central.h"

namespace Preview
{
   class Widget : public QWidget,
                  private FunctionHub
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent, Central* central);

   private slots:
      void slotOpenInMax();

   private:
      void patchSelected(QString patchPath, QString key) override;

   private:
      Central* central;
      QGraphicsScene* scene;
   };
} // namespace Preview

#endif // NOT PreviewWidgetH
