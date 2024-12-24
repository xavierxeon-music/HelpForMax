#include "MaxLine.h"

#include <QJsonArray>

#include "MaxObject.h"

Max::Line::Line(const QJsonObject& lineObject, const IdMap& idMap)
   : DiscreteMaths::Edge()
   , isParamLine(false)
   , source()
   , dest()
{
   // source
   Object* sourceObject = nullptr;
   {
      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();
      const int sourceIndex = sourceArray.at(1).toInt();

      sourceObject = idMap[sourceId];

      QList<int>& connectedList = sourceObject->outlets.connected;
      if (!connectedList.contains(sourceIndex))
         connectedList.append(sourceIndex);

      sourceObject->outlets.lines.append(this);

      const int outletDist = sourceObject->patchRect.width() / sourceObject->outlets.count;
      const int sourceX = 10 + sourceObject->patchRect.x() + (outletDist * sourceIndex);
      const int sourceY = sourceObject->patchRect.y() + sourceObject->patchRect.height();

      source = QPoint(sourceX, sourceY);
      vertexA = sourceObject;
   }

   // destination
   Object* destObject = nullptr;
   {
      const QJsonArray destArray = lineObject["destination"].toArray();
      const QString destId = destArray.at(0).toString();
      const int destIndex = destArray.at(1).toInt();

      destObject = idMap[destId];

      QList<int>& connectedList = destObject->inlets.connected;
      if (!connectedList.contains(destIndex))
         connectedList.append(destIndex);

      destObject->inlets.lines.append(this);

      const int inletDist = destObject->patchRect.width() / destObject->inlets.count;
      const int destX = 10 + destObject->patchRect.x() + (inletDist * destIndex);
      const int destY = destObject->patchRect.y();

      dest = QPoint(destX, destY);
      vertexB = destObject;
   }

   sourceObject->outlets.objects.append(destObject);
   destObject->inlets.objects.append(sourceObject);
}
