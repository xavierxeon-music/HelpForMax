#include "HelpPatchParser.h"

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonValue>

#include <Central.h>
#include <JSONModel.h>
#include <Message.h>

const QList<QByteArray> Help::PatchParser::descriptionMaxTags = {"o", "m", "at", "ar", "b", "u", "i"};


Help::PatchParser::PatchParser()
   : PatchStructure()
   , patchName()
   , patchPath()
   , helpPath()
{
}

Help::PatchParser::PatchParser(const QString& patchPath)
   : PatchStructure()
   , patchName()
   , patchPath(patchPath)
   , helpPath()
{
   const QFileInfo patchInfo(patchPath);
   patchName = patchInfo.fileName().replace(".maxpat", "");
   const QString packagePath = Central::getPackagePath();

   helpPath = packagePath + "/docs/" + patchName + ".maxref.xml";

   readXML();
   addJSON();

   if (patchDigest.text.isEmpty())
      patchDigest.text = "Hello World";

   if (!QFile::exists(helpPath))
      writeXML();
}

void Help::PatchParser::writeXML()
{
   QDomDocument doc;

   QDomElement rootElement = doc.createElement("c74object");
   doc.appendChild(rootElement);
   rootElement.setAttribute("name", patchName);
   addDigest(rootElement, patchDigest);

   {
      QDomElement metaDataElement = createSubElement(rootElement, "metadatalist");
      createSubElement(metaDataElement, "metadata", Central::getAuthor(), {{"name", "author"}});
      createSubElement(metaDataElement, "metadata", Central::getPackageName(), {{"name", "tag"}});
      for (const QString& tag : metaTagList)
         createSubElement(metaDataElement, "metadata", tag, {{"name", "tag"}});
   }

   {
      QDomElement parserElement = createSubElement(rootElement, "parser");
      parserElement.setAttribute("inlet_count", inletCount);
   }

   {
      QDomElement outputListElement = createSubElement(rootElement, "misc");
      outputListElement.setAttribute("name", "Outputs");
      for (Output::Map::ConstIterator it = outputMap.constBegin(); it != outputMap.constEnd(); it++)
      {
         QDomElement outputElement = createSubElement(outputListElement, "entry");
         outputElement.setAttribute("name", it.value().name);
         outputElement.setAttribute("id", it.key());

         addDigest(outputElement, it.value().digest);
      }
   }

   {
      QDomElement objArgListElement = createSubElement(rootElement, "objarglist");
      for (const Argument& argument : argumentList)
      {
         QDomElement arguemntElement = createSubElement(objArgListElement, "objarg");
         arguemntElement.setAttribute("name", argument.name);
         arguemntElement.setAttribute("optional", argument.optional);
         arguemntElement.setAttribute("type", typeName(argument.type));

         addDigest(arguemntElement, argument.digest);
      }
   }

   {
      QDomElement attributeListElement = createSubElement(rootElement, "attributelist");
      for (Attribute::Map::ConstIterator it = attributeMap.constBegin(); it != attributeMap.constEnd(); it++)
      {
         const Attribute& attribute = it.value();

         QDomElement attributeElement = createSubElement(attributeListElement, "attribute");
         attributeElement.setAttribute("name", it.key());
         attributeElement.setAttribute("get", attribute.get);
         attributeElement.setAttribute("set", attribute.set);
         attributeElement.setAttribute("type", typeName(attribute.type));
         attributeElement.setAttribute("size", attribute.size);

         addDigest(attributeElement, attribute.digest);
      }
   }

   {
      QDomElement messageListElement = createSubElement(rootElement, "methodlist");

      auto addMessage = [&](const Message& message, const QString& name, const bool isStandaard)
      {
         QDomElement messageElement = createSubElement(messageListElement, "method");
         messageElement.setAttribute("name", name);
         messageElement.setAttribute("standard", isStandaard);

         if (!message.arguments.empty())
         {
            QDomElement argListElement = createSubElement(messageElement, "arglist");
            for (const Argument& argument : message.arguments)
            {
               QDomElement arguemntElement = createSubElement(argListElement, "arg");
               arguemntElement.setAttribute("name", argument.name);
               arguemntElement.setAttribute("optional", argument.optional);
               arguemntElement.setAttribute("type", typeName(argument.type));
            }
         }

         addDigest(messageElement, message.digest);
      };

      for (Message::StandardMap::ConstIterator it = messageStandardMap.constBegin(); it != messageStandardMap.constEnd(); it++)
      {
         const Message& message = it.value();
         const QString& name = typeName(it.key());
         addMessage(message, name, false);
      }

      for (Message::FreeMap::ConstIterator it = messageFreeMap.constBegin(); it != messageFreeMap.constEnd(); it++)
      {
         const Message& message = it.value();
         const QString& name = it.key();
         addMessage(message, name, false);
      }
   }

   {
      QDomElement seeAlsoListElement = createSubElement(rootElement, "seealsolist");
      for (const QString& seeAlso : seeAlsoList)
      {
         createSubElement(seeAlsoListElement, "seealso", QString(), {{"name", seeAlso}});
      }
   }

   QFile file(helpPath);
   if (!file.open(QIODevice::WriteOnly))
      return;

   QByteArray content;
   content += "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n";
   content += "<?xml-stylesheet href=\"./_c74_ref.xsl\" type=\"text/xsl\"?>\n";
   content += doc.toByteArray(4);

   content = domToMaxFile(content);

   file.write(content);
   file.close();
}

QDomElement Help::PatchParser::createSubElement(QDomElement parent, const QString& name, const QString& text, const TagMap& tagMap)
{
   QDomElement element = parent.ownerDocument().createElement(name);
   parent.appendChild(element);

   if (!text.isEmpty())
   {
      QDomText textNode = parent.ownerDocument().createTextNode(text);
      element.appendChild(textNode);
   }

   for (TagMap::ConstIterator it = tagMap.constBegin(); it != tagMap.constEnd(); it++)
   {
      element.setAttribute(it.key(), it.value());
   }

   return element;
}

void Help::PatchParser::addDigest(const QDomElement& parentElement, const Digest& digest)
{
   createSubElement(parentElement, "digest", digest.text);
   if (!digest.description.isEmpty())
   {
      QString description = digest.description;
      description.replace("\n", "&lt;br/&gt;");
      createSubElement(parentElement, "description", description);
   }
}

void Help::PatchParser::readXML()
{
   QFile file(helpPath);
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QByteArray content = maxFileToDom(file.readAll());
   file.close();

   QString errorMessage;
   QDomDocument doc;
   if (!doc.setContent(content, false, &errorMessage))
   {
      ::Message::error(errorMessage);
      return;
   }

   const QDomElement rootElement = doc.documentElement();
   readDigest(rootElement, patchDigest);

   const QDomElement metaDataElement = rootElement.firstChildElement("metadatalist");
   if (!metaDataElement.isNull())
   {
      const QString& packageName = Central::getPackageName();
      for (const QDomElement& element : compileAllDirectChildElements(metaDataElement, "metadata"))
      {
         const QString& name = element.attribute("name");
         if ("tag" != name)
            continue;

         const QString text = readText(element);
         if (packageName != text)
            metaTagList.append(text);
      }
   }

   const QDomElement outputListElement = findFirstDirectChildElementWithAttributes(rootElement, "misc", {{"name", "Outputs"}});
   if (!outputListElement.isNull())
   {
      for (const QDomElement& outletElement : compileAllDirectChildElements(outputListElement, "entry"))
      {
         const int id = outletElement.attribute("id").toInt();

         Output output;
         output.name = outletElement.attribute("name");

         readDigest(outletElement, output.digest);

         const QString description = readText(outletElement);

         outputMap[id] = output;
      }
   }

   const QDomElement objArgListElement = rootElement.firstChildElement("objarglist");
   if (!objArgListElement.isNull())
   {
      for (const QDomElement& arguemntElement : compileAllDirectChildElements(objArgListElement, "objarg"))
      {
         Argument argument;
         argument.name = arguemntElement.attribute("name");
         argument.optional = ("1" == arguemntElement.attribute("optional"));
         argument.type = toType(arguemntElement.attribute("type"));

         readDigest(arguemntElement, argument.digest);

         argumentList.append(argument);
      }
   }

   const QDomElement attributeListElement = rootElement.firstChildElement("attributelist");
   if (!attributeListElement.isNull())
   {
      for (const QDomElement& attributeElement : compileAllDirectChildElements(attributeListElement, "attribute"))
      {
         const QString name = attributeElement.attribute("name");

         Attribute attribute;
         attribute.get = ("1" == attributeElement.attribute("get"));
         attribute.set = ("1" == attributeElement.attribute("set"));
         attribute.type = toType(attributeElement.attribute("type"));
         attribute.size = attributeElement.attribute("size").toInt();

         readDigest(attributeElement, attribute.digest);

         attributeMap[name] = attribute;
      }
   }

   const QDomElement messageListElement = rootElement.firstChildElement("methodlist");
   if (!messageListElement.isNull())
   {
      for (const QDomElement& messageElement : compileAllDirectChildElements(messageListElement, "method"))
      {
         const QString name = messageElement.attribute("name");

         Message message;

         const QDomElement argListElement = messageElement.firstChildElement("arglist");
         if (!argListElement.isNull())
         {
            for (const QDomElement& arguemntElement : compileAllDirectChildElements(argListElement, "arg"))
            {
               Argument argument;
               argument.name = arguemntElement.attribute("name");
               argument.optional = ("1" == arguemntElement.attribute("optional"));
               argument.type = toType(arguemntElement.attribute("type"));

               message.arguments.append(argument);
            }
         }

         readDigest(messageElement, message.digest);

         const bool isStandard = ("1" == messageElement.attribute("standard"));
         if (isStandard)
         {
            const Type type = toType(name);
            messageStandardMap[type] = message;
         }
         else
         {
            messageFreeMap[name] = message;
         }
      }
   }

   const QDomElement seeAlsoListElement = rootElement.firstChildElement("seealsolist");
   if (!seeAlsoListElement.isNull())
   {
      for (QDomElement element = seeAlsoListElement.firstChildElement("seealso"); !element.isNull(); element = element.nextSiblingElement("seealso"))
      {
         const QString& name = element.attribute("name");
         seeAlsoList.append(name);
      }
   }
}

void Help::PatchParser::readDigest(const QDomElement& parentElement, Digest& digest) const
{
   const QDomElement textElement = parentElement.firstChildElement("digest");
   digest.text = readText(textElement);

   const QDomElement descriptionElement = parentElement.firstChildElement("description");
   digest.description = readText(descriptionElement);
}

QString Help::PatchParser::readText(const QDomElement& element) const
{
   if (element.isNull())
      return QString();

   const QDomText textNode = element.firstChild().toText();
   if (textNode.isNull())
      return QString();

   return textNode.data();
}

QDomElement Help::PatchParser::findFirstDirectChildElementWithAttributes(const QDomElement& element, const QString& tag, const TagMap& tagMap) const
{
   for (QDomElement childElement = element.firstChildElement(tag); !childElement.isNull(); childElement = childElement.nextSiblingElement(tag))
   {
      auto hasTags = [&]()
      {
         if (tagMap.empty())
            return true;

         for (TagMap::ConstIterator it = tagMap.constBegin(); it != tagMap.constEnd(); it++)
         {
            if (!childElement.hasAttribute(it.key()))
               continue;

            if (it.value() == childElement.attribute(it.key()))
               return true;
         }

         return false;
      };

      if (hasTags())
         return element;
   }
   return QDomElement();
}

QList<QDomElement> Help::PatchParser::compileAllDirectChildElements(const QDomElement& element, const QString& tag, const TagMap& tagMap) const
{
   QList<QDomElement> list;
   for (QDomElement childElement = element.firstChildElement(tag); !childElement.isNull(); childElement = childElement.nextSiblingElement(tag))
   {
      auto hasTags = [&]()
      {
         if (tagMap.empty())
            return true;

         for (TagMap::ConstIterator it = tagMap.constBegin(); it != tagMap.constEnd(); it++)
         {
            if (!childElement.hasAttribute(it.key()))
               continue;

            if (it.value() == childElement.attribute(it.key()))
               return true;
         }

         return false;
      };

      if (!hasTags())
         continue;

      list.append(childElement);
   }

   return list;
}

void Help::PatchParser::addJSON()
{
   QJsonObject object = JSON::fromFile(patchPath);
   if (object.empty())
      return;

   const QJsonObject patcherObject = object["patcher"].toObject();
   const QJsonArray boxArray = patcherObject["boxes"].toArray();
   const QJsonArray lineArray = patcherObject["lines"].toArray();

   struct Inlet
   {
      int index;
      QStringList connectedIdList;
   };

   using InletConnectionMap = QMap<QString, Inlet>;
   InletConnectionMap inletConnectionMap;

   // parse inlets, outlets and patcherargs
   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if ("inlet" == className)
      {
         const int index = boxObject["index"].toInt();
         const QString id = boxObject["id"].toString();
         inletConnectionMap[id] = {index, QStringList()};
      }
      else if ("outlet" == className)
      {
         const int index = boxObject["index"].toInt();
         Output& output = findOrCreateOutput(index);

         const QString comment = boxObject["comment"].toString();
         if (output.name.isEmpty())
            output.name = comment;
      }
      else if ("newobj" == className)
      {
         const QString text = boxObject["text"].toString();
         if (!text.startsWith("patcherargs"))
            continue;

         const QStringList contentList = text.split(" ", Qt::SkipEmptyParts);
         enum class State
         {
            Argument,
            AttributeName,
            AttributeArg
         };

         State state = State::Argument;

         for (int i = 1; i < contentList.size(); i++)
         {
            const QString& arg = contentList.at(i);

            // maybe advance state
            if (arg.startsWith("@"))
               state = State::AttributeName;
            else if (State::AttributeName == state)
               state = State::AttributeArg;

            if (State::Argument == state)
            {
               Argument argument;
               argument.name = QString::number(i);
               argument.optional = true;

               if (i > argumentList.count())
                  argumentList.append(argument);
            }
            else if (State::AttributeName == state)
            {
               Attribute attribute;
               const QString& name = arg.mid(1);
               if (!attributeMap.contains(name))
                  attributeMap[name] = attribute;
            }
            else if (State::AttributeArg == state)
            {
            }
         }
      }
   }

   inletCount = inletConnectionMap.count();

   // find objects connected to inlets
   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      const QJsonArray destArray = lineObject["destination"].toArray();
      const QString destId = destArray.at(0).toString();

      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();

      if (inletConnectionMap.contains(destId))
         inletConnectionMap[destId].connectedIdList.append(sourceId);
      else if (inletConnectionMap.contains(sourceId))
         inletConnectionMap[sourceId].connectedIdList.append(destId);
   }

   // parse routes connected to inlets for  messages
   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if ("newobj" == className)
      {
         const QString text = boxObject["text"].toString();
         if (!text.startsWith("route"))
            continue;

         // check if connected to inlet
         int connectedIndex = [&]() -> int
         {
            const QString routeId = boxObject["id"].toString();
            for (InletConnectionMap::ConstIterator it = inletConnectionMap.constBegin(); it != inletConnectionMap.constEnd(); it++)
            {
               for (const QString& id : it.value().connectedIdList)
               {
                  if (id == routeId)
                     return it.value().index;
               }
            }
            return -1;
         }();

         if (-1 == connectedIndex)
            continue;

         const QStringList contentList = text.split(" ", Qt::SkipEmptyParts);
         for (int i = 1; i < contentList.size(); i++)
         {
            const QString& messageText = contentList.at(i);

            if (!messageFreeMap.contains(messageText))
               messageFreeMap[messageText] = Message{};
         }
      }
   }
}

PatchStructure::Output& Help::PatchParser::findOrCreateOutput(const int id)
{
   if (!outputMap.contains(id))
      outputMap[id] = Output{};

   return outputMap[id];
}

QByteArray Help::PatchParser::domToMaxFile(QByteArray domXML) const
{
   domXML.replace("&amp;", "&");
   domXML.replace("&lt;", "<");
   domXML.replace("&gt;", ">");

   return domXML;
}

QByteArray Help::PatchParser::maxFileToDom(QByteArray maxXML) const
{
   for (const QByteArray& tag : descriptionMaxTags)
   {
      maxXML.replace("<" + tag + ">", "&lt;" + tag + "&gt;");
      maxXML.replace("</" + tag + ">", "&lt;/" + tag + "&gt;");
   }
   maxXML.replace("<br/>", "\n");

   return maxXML;
}
