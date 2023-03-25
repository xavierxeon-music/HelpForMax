#include "BlockRef.h"

#include <QDebug>
#include <QFile>

#include "Tools/Central.h"

Block::Ref::Ref(Item* item)
   : refPath()
   , item(item)
{
   const QString packagePath = Central::getPackagePath();
   refPath = packagePath + "/docs/" + item->key + ".maxref.xml";
}

void Block::Ref::readXML()
{
   QFile file(refPath);
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QByteArray content = maxFileToDom(file.readAll());
   file.close();

   QString errorMessage;
   QDomDocument doc;
   if (!doc.setContent(content, false, &errorMessage))
   {
      qWarning() << "unable to read xml" << errorMessage;
      return;
   }

   const QDomElement rootElement = doc.documentElement();
   readDigest(rootElement, item->patchDigest);
   if (item->patchDigest.description.isEmpty())
      item->markUndocumented(item->patchDigest);

   item->extras.openAsBPatcher = (1 == rootElement.attribute("as_bpatcher", "0").toInt());

   {
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
               item->extras.metaTagList.append(text);
         }
      }
   }

   {
      const QDomElement outputListElement = findFirstDirectChildElementWithAttributes(rootElement, "misc", {{"name", "Outputs"}});
      if (!outputListElement.isNull())
      {
         for (const QDomElement& outletElement : compileAllDirectChildElements(outputListElement, "entry"))
         {
            const int id = outletElement.attribute("id").toInt();

            Structure::Output output;
            output.name = outletElement.attribute("name");

            readDigest(outletElement, output.digest);
            if (output.digest.description.isEmpty())
               item->markUndocumented(output.digest);

            item->outputMap[id] = output;
         }
      }
   }

   {
      const QDomElement objArgListElement = rootElement.firstChildElement("objarglist");
      if (!objArgListElement.isNull())
      {
         for (const QDomElement& arguemntElement : compileAllDirectChildElements(objArgListElement, "objarg"))
         {
            Structure::Argument argument;
            argument.name = arguemntElement.attribute("name");
            argument.optional = ("1" == arguemntElement.attribute("optional"));
            argument.type = Structure::toType(arguemntElement.attribute("type"));

            readDigest(arguemntElement, argument.digest);
            if (argument.digest.text.isEmpty())
               item->markUndocumented(argument.digest);

            item->argumentList.append(argument);
         }
      }
   }

   {
      const QDomElement attributeListElement = rootElement.firstChildElement("attributelist");
      if (!attributeListElement.isNull())
      {
         for (const QDomElement& attributeElement : compileAllDirectChildElements(attributeListElement, "attribute"))
         {
            const QString name = attributeElement.attribute("name");

            Structure::Attribute attribute;
            attribute.get = ("1" == attributeElement.attribute("get"));
            attribute.set = ("1" == attributeElement.attribute("set"));
            attribute.type = Structure::toType(attributeElement.attribute("type"));
            attribute.size = attributeElement.attribute("size").toInt();

            readDigest(attributeElement, attribute.digest);
            if (attribute.digest.text.isEmpty())
               item->markUndocumented(attribute.digest);

            item->attributeMap[name] = attribute;
         }
      }
   }

   {
      const QDomElement messageListElement = rootElement.firstChildElement("methodlist");
      if (!messageListElement.isNull())
      {
         for (const QDomElement& messageElement : compileAllDirectChildElements(messageListElement, "method"))
         {
            const QString name = messageElement.attribute("name");

            Structure::Message message;

            const QDomElement argListElement = messageElement.firstChildElement("arglist");
            if (!argListElement.isNull())
            {
               for (const QDomElement& arguemntElement : compileAllDirectChildElements(argListElement, "arg"))
               {
                  Structure::Argument argument;
                  argument.name = arguemntElement.attribute("name");
                  argument.optional = ("1" == arguemntElement.attribute("optional"));
                  argument.type = Structure::toType(arguemntElement.attribute("type"));

                  message.arguments.append(argument);
               }
            }

            readDigest(messageElement, message.digest);
            if (item->patchDigest.description.isEmpty())
               item->markUndocumented(item->patchDigest);

            const bool isStandard = ("1" == messageElement.attribute("standard"));
            if (isStandard)
            {
               const Structure::Type type = Structure::toType(name);
               item->messageStandardMap[type] = message;
            }
            else
            {
               item->messageFreeMap[name] = message;
            }
         }
      }
   }

   {
      const QDomElement seeAlsoListElement = rootElement.firstChildElement("seealsolist");
      if (!seeAlsoListElement.isNull())
      {
         for (QDomElement element = seeAlsoListElement.firstChildElement("seealso"); !element.isNull(); element = element.nextSiblingElement("seealso"))
         {
            const QString& name = element.attribute("name");
            item->extras.seeAlsoList.append(name);
         }
      }
   }
}

void Block::Ref::writeXML()
{
   QDomDocument doc;

   QDomElement rootElement = doc.createElement("c74object");
   doc.appendChild(rootElement);
   rootElement.setAttribute("name", item->key);
   rootElement.setAttribute("as_bpatcher", item->extras.openAsBPatcher);
   addDigest(rootElement, item->patchDigest);

   {
      QDomElement metaDataElement = createSubElement(rootElement, "metadatalist");
      createSubElement(metaDataElement, "metadata", Central::getAuthor(), {{"name", "author"}});
      createSubElement(metaDataElement, "metadata", Central::getPackageName(), {{"name", "tag"}});
      for (const QString& tag : item->extras.metaTagList)
         createSubElement(metaDataElement, "metadata", tag, {{"name", "tag"}});
   }

   {
      QDomElement parserElement = createSubElement(rootElement, "parser");
      parserElement.setAttribute("inlet_count", item->inletCount);
   }

   {
      QDomElement outputListElement = createSubElement(rootElement, "misc");
      outputListElement.setAttribute("name", "Outputs");
      for (Structure::Output::Map::ConstIterator it = item->outputMap.constBegin(); it != item->outputMap.constEnd(); it++)
      {
         QDomElement outputElement = createSubElement(outputListElement, "entry");
         outputElement.setAttribute("name", it.value().name);
         outputElement.setAttribute("id", it.key());

         addDigest(outputElement, it.value().digest);
      }
   }

   {
      QDomElement objArgListElement = createSubElement(rootElement, "objarglist");
      for (const Structure::Argument& argument : item->argumentList)
      {
         QDomElement arguemntElement = createSubElement(objArgListElement, "objarg");
         arguemntElement.setAttribute("name", argument.name);
         arguemntElement.setAttribute("optional", argument.optional);
         arguemntElement.setAttribute("type", Structure::typeName(argument.type));

         addDigest(arguemntElement, argument.digest);
      }
   }

   {
      QDomElement attributeListElement = createSubElement(rootElement, "attributelist");
      for (Structure::Attribute::Map::ConstIterator it = item->attributeMap.constBegin(); it != item->attributeMap.constEnd(); it++)
      {
         const Structure::Attribute& attribute = it.value();

         QDomElement attributeElement = createSubElement(attributeListElement, "attribute");
         attributeElement.setAttribute("name", it.key());
         attributeElement.setAttribute("get", attribute.get);
         attributeElement.setAttribute("set", attribute.set);
         attributeElement.setAttribute("type", Structure::typeName(attribute.type));
         attributeElement.setAttribute("size", attribute.size);

         addDigest(attributeElement, attribute.digest);
      }
   }

   {
      QDomElement messageListElement = createSubElement(rootElement, "methodlist");

      auto addMessage = [&](const Structure::Message& message, const QString& name, const bool isStandard)
      {
         QDomElement messageElement = createSubElement(messageListElement, "method");
         messageElement.setAttribute("name", name);
         messageElement.setAttribute("standard", isStandard);

         if (!message.arguments.empty() && !isStandard)
         {
            QDomElement argListElement = createSubElement(messageElement, "arglist");
            for (const Structure::Argument& argument : message.arguments)
            {
               QDomElement arguemntElement = createSubElement(argListElement, "arg");
               arguemntElement.setAttribute("name", argument.name);
               arguemntElement.setAttribute("optional", argument.optional);
               arguemntElement.setAttribute("type", Structure::typeName(argument.type));
            }
         }

         addDigest(messageElement, message.digest);
      };

      for (Structure::Message::StandardMap::ConstIterator it = item->messageStandardMap.constBegin(); it != item->messageStandardMap.constEnd(); it++)
      {
         const Structure::Message& message = it.value();
         const QString& name = Structure::typeName(it.key());
         addMessage(message, name, true);
      }

      for (Structure::Message::FreeMap::ConstIterator it = item->messageFreeMap.constBegin(); it != item->messageFreeMap.constEnd(); it++)
      {
         const Structure::Message& message = it.value();
         const QString& name = it.key();
         addMessage(message, name, false);
      }
   }

   {
      QDomElement seeAlsoListElement = createSubElement(rootElement, "seealsolist");
      for (const QString& seeAlso : item->extras.seeAlsoList)
      {
         createSubElement(seeAlsoListElement, "seealso", QString(), {{"name", seeAlso}});
      }
   }

   QFile file(refPath);
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

QDomElement Block::Ref::createSubElement(QDomElement parent, const QString& name, const QString& text, const TagMap& tagMap)
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

void Block::Ref::addDigest(const QDomElement& parentElement, const Structure::Digest& digest)
{
   createSubElement(parentElement, "digest", digest.text);
   if (!digest.description.isEmpty())
   {
      QString description = digest.description;
      description.replace("\n", "&lt;br/&gt;");
      createSubElement(parentElement, "description", description);
   }
}

void Block::Ref::readDigest(const QDomElement& parentElement, Structure::Digest& digest) const
{
   const QDomElement textElement = parentElement.firstChildElement("digest");
   digest.text = readText(textElement);

   const QDomElement descriptionElement = parentElement.firstChildElement("description");
   digest.description = readText(descriptionElement);
}

QString Block::Ref::readText(const QDomElement& element) const
{
   if (element.isNull())
      return QString();

   const QDomText textNode = element.firstChild().toText();
   if (textNode.isNull())
      return QString();

   return textNode.data();
}

QDomElement Block::Ref::findFirstDirectChildElementWithAttributes(const QDomElement& element, const QString& tag, const TagMap& tagMap) const
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
         return childElement;
   }
   return QDomElement();
}

QList<QDomElement> Block::Ref::compileAllDirectChildElements(const QDomElement& element, const QString& tag, const TagMap& tagMap) const
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

QByteArray Block::Ref::domToMaxFile(QByteArray domXML) const
{
   domXML.replace("&amp;", "&");
   domXML.replace("&lt;", "<");
   domXML.replace("&gt;", ">");

   return domXML;
}

QByteArray Block::Ref::maxFileToDom(QByteArray maxXML) const
{
   for (const QByteArray& tag : Item::descriptionMaxTags)
   {
      maxXML.replace("<" + tag + ">", "&lt;" + tag + "&gt;");
      maxXML.replace("</" + tag + ">", "&lt;/" + tag + "&gt;");
   }
   maxXML.replace("<br/>", "\n");

   return maxXML;
}
