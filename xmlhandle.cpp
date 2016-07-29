#include "xmlhandle.h"

XMLHandle::XMLHandle()
{

}
bool XMLHandle::setXMl(const QString& xmlName)
{
    QFile file(xmlName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!xmlDoc.setContent(&file)) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool XMLHandle::findNodeAttr(QDomNode&rootNode,const QString&nodeName,const QString&attrName,QString&value)
{
    QDomNodeList childNodes = rootNode.childNodes();
    for(int i = 0;i<childNodes.size();i++){
        QDomNode iNode = childNodes.item(i);

        if(iNode.hasChildNodes()){
            if(findNodeAttr(iNode,nodeName,attrName,value))//执行递归
                break;
        }

        if(iNode.nodeName()!=nodeName) //检查node名
            continue;
        if(!iNode.toElement().hasAttribute(attrName)) //检查attr名
            continue;


        if(iNode.toElement().attribute(attrName).size()>0){
            value = iNode.toElement().attribute(attrName);
            return true;
        }else
            return false;
    }
    return false;
}

bool XMLHandle::findNodeValue(QDomNode&rootNode,const QString&nodeName,QString&value)
{
    QString rootNodeName = rootNode.nodeName();

    QDomNodeList childNodes = rootNode.childNodes();
    int nodeNumber = rootNode.childNodes().count();
    for(int i = 0;i<childNodes.size();i++){
        QDomNode iNode = childNodes.item(i);

        if(iNode.hasChildNodes()){
            if(findNodeValue(iNode,nodeName,value))//执行递归
                break;
        }

        if(iNode.nodeName()!=nodeName) //检查node名
            continue;


        if(iNode.toElement().text().size()>0){
            value = iNode.toElement().text();
            return true;
        }else
            return false;
    }
    return false;

}
bool XMLHandle::findValue(const QString&nodeName,QString& value){
    QDomNode rootNode = xmlDoc.childNodes().item(1);
    QString rootnodename = rootNode.nodeName();
    bool ok = findNodeAttr(rootNode,nodeName,"providerID",value);
    return ok;
}
bool XMLHandle::findAttrValue(const QString&nodeName,const QString&attrNode,QString&value){
    return false;
}

bool XMLHandle::findValues(const QString& nodeName, const QStringList &attrName, QStringList &value)
{
    return false;
}
