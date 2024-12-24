#ifndef MaxPatcherH
#define MaxPatcherH

#include "DiscreteMathsGraph.h"
#include "RefStructure.h"

#include "MaxLine.h"
#include "MaxObject.h"

namespace Max
{
   class Patcher : public Ref::Structure, public DiscreteMaths::Graph
   {
   public:
      Patcher();

   public:
      void readPatch(const QString& patchFileName);
      Object* getVertexCast(int vertexIndex) const;
      Line* getEdgeCast(int edgeIndex) const;
      Object::List findAll(const Object::Type& type, bool paramObjectsOnly) const;
      Object::List findAll(const QList<Object::Type>& typeList, bool paramObjectsOnly) const;

   private:
      using TypeBuffer = QMap<Object::Type, Object::List>;

   private:
      void analyse();
      IdMap readObjects(const QJsonObject patcherObject);
      void readLines(const QJsonObject patcherObject, const IdMap& idMap);

   private:
      TypeBuffer typeBuffer;
   };
} // namespace Max

#endif // NOT MaxPatcherH
