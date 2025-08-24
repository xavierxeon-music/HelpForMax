#ifndef MaxLineH
#define MaxLineH

#include "GraphEdge.h"

#include <QJsonObject>
#include <QPoint>

namespace Max
{
   class Object;
   using IdMap = QMap<QString, Object*>;

   class Line : public Graph::Edge
   {
   public:
      Line(const QJsonObject& lineObject, const IdMap& idMap);

   public:
      QPoint source;
      QPoint dest;
      bool isParamLine;
   };
} // namespace Max

#endif // NOT MaxLineH
