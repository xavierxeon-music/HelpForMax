#ifndef MaxWidgetH
#define MaxWidgetH

#include <QGraphicsView>

#include <QGraphicsScene>
#include <QJsonObject>

#include "MaxPatcher.h"

namespace Max
{
   class Widget : public QGraphicsView
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent);

   public:
      void load(const Patcher& patcher);

   private:
      void wheelEvent(QWheelEvent* wheelEvent) override;
      void mouseDoubleClickEvent(QMouseEvent* mouseEvent) override;

      void keyPressEvent(QKeyEvent* event) override;
      void keyReleaseEvent(QKeyEvent* event) override;

      void updateZoom(bool save);

   private:
      QGraphicsScene* scene;

      QPen blackBorderPen;
      QPen blackLinePen;
      QPen blueLinePen;
      QBrush whiteBrush;
      QBrush blackBrush;
      QFont font;

      double zoomLevel;
   };
} // namespace Max

#endif // NOT GraphWidgetH
