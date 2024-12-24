#include "MaxWidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QGraphicsItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMouseEvent>
#include <QSettings>
#include <QWheelEvent>

#include "MaxLine.h"
#include "MaxObject.h"

Max::Widget::Widget(QWidget* parent)
   : QGraphicsView(parent)
   , scene(nullptr)
   , blackBorderPen(Qt::black)
   , blackLinePen(Qt::darkGray, 3.0, Qt::SolidLine)
   , blueLinePen(Qt::blue, 3.0, Qt::SolidLine)
   , whiteBrush(Qt::white)
   , blackBrush(Qt::black)
   , font()
   , zoomLevel(1.0)
{
   setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   setInteractive(false);

   font.setPixelSize(10);

   QBrush bgBrush = QApplication::palette().window();

   scene = new QGraphicsScene(this);
   scene->setBackgroundBrush(bgBrush);

   setScene(scene);
   setAlignment(Qt::AlignLeft | Qt::AlignTop);
   setViewportMargins(10, 10, 0, 0);

   {
      QSettings settings;
      zoomLevel = settings.value("Graph/Zoom", 1.0).toDouble();
   }

   updateZoom(false);
}

void Max::Widget::load(const Patcher& patcher)
{
   scene->clear();

   for (int lineIndex = 0; lineIndex < patcher.edgeCount(); lineIndex++)
   {
      Line* line = patcher.getEdgeCast(lineIndex);
      scene->addLine(QLineF(line->source, line->dest), line->isParamLine ? blueLinePen : blackLinePen);
   }

   for (int vertIndex = 0; vertIndex < patcher.vertexCount(); vertIndex++)
   {
      Object* object = patcher.getVertexCast(vertIndex);
      const QRectF& patchRect = object->patchRect;

      QString toolTip = object->comment;
      if (!toolTip.isEmpty())
         toolTip += " ";
      toolTip += "(" + object->id + ")";

      QGraphicsRectItem* rectItem = scene->addRect(QRectF(0, 0, patchRect.width(), patchRect.height()), object->isParamObject ? blueLinePen : blackBorderPen, whiteBrush);
      rectItem->setToolTip(toolTip);
      rectItem->setPos(patchRect.x(), patchRect.y());

      QGraphicsRectItem* topBar = scene->addRect(QRectF(0, 0, patchRect.width(), 2), blackBorderPen, blackBrush);
      topBar->setToolTip(toolTip);
      topBar->setPos(patchRect.x(), patchRect.y());

      QGraphicsRectItem* bottomBar = scene->addRect(QRectF(0, patchRect.height(), patchRect.width(), 2), blackBorderPen, blackBrush);
      bottomBar->setToolTip(toolTip);
      bottomBar->setPos(patchRect.x(), patchRect.y());

      QGraphicsSimpleTextItem* textItem = scene->addSimpleText(object->text, font);
      textItem->setToolTip(toolTip);
      textItem->setPos(patchRect.x() + 5, patchRect.y() + 5);
   }

   updateZoom(false);
}

void Max::Widget::wheelEvent(QWheelEvent* wheelEvent)
{
   const double delta = 0.001 * wheelEvent->angleDelta().y();
   const double factor = std::pow(2.0, delta);

   zoomLevel *= factor;
   updateZoom(true);
}

void Max::Widget::mouseDoubleClickEvent(QMouseEvent* mouseEvent)
{
   QGraphicsItem* item = itemAt(mouseEvent->pos());
   if (!item)
   {
      zoomLevel = 1.0;
      updateZoom(true);
   }
}

void Max::Widget::keyPressEvent(QKeyEvent* event)
{
   const bool enabled = Qt::ShiftModifier & event->modifiers();
   setDragMode(enabled ? ScrollHandDrag : NoDrag);
}

void Max::Widget::keyReleaseEvent(QKeyEvent* event)
{
   const bool enabled = Qt::ShiftModifier & event->modifiers();
   setDragMode(enabled ? ScrollHandDrag : NoDrag);
}

void Max::Widget::updateZoom(bool save)
{
   setTransform(QTransform::fromScale(zoomLevel, zoomLevel));

   if (!save)
      return;

   QSettings settings;
   settings.setValue("Graph/Zoom", zoomLevel);
}
