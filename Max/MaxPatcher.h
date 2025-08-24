#ifndef MaxPatcherH
#define MaxPatcherH

#include "Graph.h"
#include "RefStructure.h"

#include "MaxLine.h"
#include "MaxObject.h"

namespace Max
{
   class Patcher : public Ref::Structure, public Graph
   {
   public:
      Patcher();

   public:
      void readPatch(const QString& patchFileName);
      Object* getVertexCast(int vertexIndex) const;
      Line* getEdgeCast(int edgeIndex) const;
      Object::List findAll(const Object::Type& type, bool paramObjectsOnly) const;
      Object::List findAll(const QList<Object::Type>& typeList, bool paramObjectsOnly) const;
      const Style::Map& getStyleMap() const;

   private:
      using TypeBuffer = QMap<Object::Type, Object::List>;

   private:
      void analyse();
      void readStyles(const QJsonObject patcherObject);
      IdMap readObjects(const QJsonObject patcherObject);
      void readLines(const QJsonObject patcherObject, const IdMap& idMap);
      QColor readColor(const QJsonObject object, const QString& key) const;

   private:
      TypeBuffer typeBuffer;
      Style::Map styleMap;
   };
} // namespace Max

#endif // NOT MaxPatcherH
