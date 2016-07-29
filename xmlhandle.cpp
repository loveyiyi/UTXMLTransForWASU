#include "xmlhandle.h"

XMLHandle::XMLHandle()
{

}

bool XMLHandle::nodeHasAttrValue(QDomNode& iNode ,const QString& attrValue){
    //检查本节点是否包含指定参数
    for(int i = 0 ;i<iNode.attributes().count();i++){
        QString pAttrValue = iNode.attributes().item(i).nodeValue();
        if(attrValue == pAttrValue){
            return true;
        }
    }
    return false;

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
                return true;
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
    QDomNodeList childNodes = rootNode.childNodes();
    for(int i = 0;i<childNodes.size();i++){
        QDomNode iNode = childNodes.item(i);

        if(iNode.hasChildNodes()){
            if(findNodeValue(iNode,nodeName,value))//执行递归
                return true;
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





bool XMLHandle::findNodeOnCond(QDomNode& rootNode,const QString& nodeName,const QStringList& condList,const QString &subNodeName,const QString &attrName,QString &value){
    QDomNodeList childNodes = rootNode.childNodes();
    for(int i = 0;i<childNodes.size();i++){
        //遍历节点
        QDomNode iNode = childNodes.item(i);
        if(iNode.hasChildNodes())
            if(findNodeOnCond(iNode,nodeName,condList,subNodeName,attrName,value))
                return true;

        //NodeName需要相吻合
        if(iNode.nodeName()!=nodeName)
            continue;


        QString condString;
        bool ok;
        foreach(condString,condList){
            if(!nodeHasAttrValue(iNode,condString) ){
                ok = true;
                break;
            }
        }
        if(ok)
            continue;

        if(findNodeValue(iNode,attrName,value)){
            return true;
        }

        if(findNodeAttr(iNode,subNodeName,attrName,value)){
            return true;
        }
    }
    return false;
}

bool XMLHandle::findValueOnCond(const QString& nodeName,const QStringList& condList,const QString &subNodeName,const QString &valueName,QString &value){

    QDomNode rootNode = xmlDoc.childNodes().item(1);
    bool ok = findNodeOnCond(rootNode,nodeName,condList,subNodeName,valueName,value);
    return ok;
}

bool XMLHandle::findValue(const QString&nodeName, QString& value){
    QDomNode rootNode = xmlDoc.childNodes().item(1);
    bool ok = findNodeValue(rootNode,nodeName,value);
    return ok;
}

bool XMLHandle::findAttrValue(const QString&nodeName,const QString&attrNode,QString&value){
    QDomNode rootNode = xmlDoc.childNodes().item(1);
    bool ok = findNodeAttr(rootNode,nodeName,attrNode,value);
    return ok;
}

bool XMLHandle::findAttrValues(const QStringList& nodeName, const QStringList &valueName, QStringList &value){
    if(nodeName.size() != valueName.size() )
        return false;

    for(int i = 0;i<nodeName.size();i++){
        QString tempValue;
        bool ok = findAttrValue(nodeName[i],valueName[i],tempValue);
        if(!ok)
            return ok;
        else
            value.append(tempValue);
    }
    return true;
}

bool XMLHandle::findValues(const QStringList& nodeName,  QStringList &value){

    for(int i = 0;i<nodeName.size();i++){
        QString tempValue;
        bool ok = findValue(nodeName[i],tempValue);
        if(!ok)
            return ok;
        else
            value.append(tempValue);
    }
    return true;
}
