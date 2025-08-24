#ifndef MaxObjectH
#define MaxObjectH

#include "GraphVertex.h"

#include <QJsonObject>
#include <QRectF>

#include "MaxStyle.h"

namespace Max
{
   class Line;

   class Object : public Graph::Vertex
   {
   public:
      using List = QList<Object*>;
      using LineList = QList<Line*>;

      enum class Type
      {
         Other,
         Inlet,
         Outlet,
         PatcherArgs,
         Route,
         RoutePass,
         TypeRoute,
         Unpack
      };

      struct Ports
      {
         int count = 0;
         QList<int> connected;
         LineList lines;
         List objects;
      };

   public:
      Object(const QJsonObject& boxObject);

   public:
      bool isChildOf(const Object* parent) const;

   public:
      QRectF patchRect;
      QString className;
      QString text;
      QString comment;
      QString style;
      Ports inlets;
      Ports outlets;
      Type type;
      QString id;

      bool isParamObject;
   };
} // namespace Max

#endif // NOT MaxObjectH
