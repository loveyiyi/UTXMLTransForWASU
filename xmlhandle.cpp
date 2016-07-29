#include "xmlhandle.h"

XMLHandle::XMLHandle()
{

}

bool XMLHandle::nodeHasAttrValue(QDomNode iNode ,const QString& attrValue){
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

bool XMLHandle::findValueOnCond(const QString & nodeName,
                                const QString &subNodeName,
                                QString &value){
    value.clear();
    QDomNodeList iNode =xmlDoc.toDocument().elementsByTagName(nodeName);
    for(int i = 0;i<iNode.size();i++){
       if(!iNode.at(i).hasChildNodes())
           continue;
       QDomNodeList iNode_sublist = iNode.item(i).childNodes();
       for(int j = 0;j<iNode_sublist.size();j++){
           if(iNode_sublist.item(j).toElement().nodeName() == subNodeName){
               value = iNode_sublist.item(j).toElement().text();
               return true;
           }
       }

    }
    return false;
}


bool XMLHandle::findValueOnCond(const QString & nodeName,
                                const QStringList&nodeConditions,
                                const QString &subNodeName,
                                QString &value){
    value.clear();
    QDomNodeList iNode =xmlDoc.toDocument().elementsByTagName(nodeName);
    for(int i = 0;i<iNode.size();i++){
        QString condString;
        foreach(condString,nodeConditions){
            if(nodeHasAttrValue(iNode.item(i),condString) ){
                if(subNodeName.size()>0){
                    QDomNodeList subNodeList = iNode.item(i).childNodes();
                    for(int j=0;j<subNodeList.size();j++){
                        QString subNodeTpName = subNodeList.item(j).nodeName();
                        if(subNodeTpName == subNodeName){
                            value = subNodeList.item(j).toElement().text();
                            return true;
                        }
                    }
                }else{
                    value = iNode.item(i).toElement().text();
                    return true;
                }
            }
        }
    }
    return false;
}

bool XMLHandle::findAttrValueOnCond(const QString & nodeName,
                         const QStringList&attrConditions,
                         const QString &subNodeName,
                         const QString&attrName,
                         QString &value){
    value.clear();
    QDomNodeList iNode =xmlDoc.toDocument().elementsByTagName(nodeName);
    for(int i = 0;i<iNode.size();i++){
        QString condString;
        foreach(condString,attrConditions){
            if(nodeHasAttrValue(iNode.item(i),condString) ){
                if(subNodeName.size()>0){
                    QDomNodeList subNodeList = iNode.item(i).childNodes();
                    int count = subNodeList.size();
                    for(int j=0;j<subNodeList.size();j++){
                        if(subNodeList.item(j).nodeName() == subNodeName){
                            value = subNodeList.item(j).toElement().attribute(attrName);
                            return true;
                        }
                    }
                }else{
                    value = iNode.item(i).toElement().attribute(attrName);
                    return true;
                }
            }
        }
    }
    return false;
}

bool XMLHandle::findValue(const QString&nodeName, QString& value){
    value.clear();
    QDomNodeList childNodes = xmlDoc.toDocument().elementsByTagName(nodeName);
    for(int i = 0;i<childNodes.size();i++){
        QDomNode tNode = childNodes.item(i);
        if(tNode.nodeName() == nodeName){
            value = tNode.toElement().text();
            return true;
        }
    }
    return false;
}

bool XMLHandle::findAttrValue(const QString&nodeName,const QString&attrName,QString&value){
    value.clear();
    QDomNodeList allSetNode =xmlDoc.toDocument().elementsByTagName(nodeName);
    for(int i = 0;i<allSetNode.count();i++){
        QDomNode tNode = allSetNode.item(i);
        if(tNode.toElement().hasAttribute(attrName)){
            value = tNode.toElement().attribute(attrName);
            return true;
        }
    }
    return false;
}

