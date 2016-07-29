#include "utunit.h"

UTUnit::UTUnit()
{
    CNTV_XML_Vector cccccc;
    objVector.clear();
    mapVector.clear();
/* 测试用代码
    cccccc.elementType = 1;
    QPair<QString,QString > tempaaa;
    tempaaa.first = "aaaaa";
    tempaaa.second = "bbbb";
    cccccc.nodeAttrVector.append(tempaaa);
    QPair<QString,QString > tempaaabbb;
    tempaaabbb.first = "aaaaa111";
    tempaaabbb.second = "bbbb111";
    cccccc.pVector.append(tempaaabbb);
    tempaaabbb.first = "aaaaa222";
    tempaaabbb.second = "bbbb222";
    cccccc.pVector.append(tempaaabbb);

    mapVector.append(cccccc);
    objVector.append(cccccc);*/

}
bool UTUnit::appendObj(const CNTV_XML_Vector &tVector){
    objVector.append(tVector);
    return true;
}

bool UTUnit::appendMap(const CNTV_XML_Vector &tVector){
    mapVector.append(tVector);
    return true;
}
bool UTUnit::createUTXML(QString& outputString ){
    QDomDocument UTXml;
    QDomNode adiNode;
    QDomNode objRootNode;
    QDomNode mapRootNode;

    //处理文件头信息
    QDomNode headNode( UTXml.createProcessingInstruction( "xml", "version=\"1.0\" \"encoding=\"UTF-8\" \"standalone=\"no\"" ) );
    UTXml.insertBefore( headNode, UTXml.firstChild() );

    //建立ADI的主NODE
    adiNode = UTXml.createElement("ADI");
    UTXml.insertAfter( adiNode, headNode );
    adiNode.toElement().setAttribute("StaffID","607");
    adiNode.toElement().setAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");


    //处理文件以及编目的相关信息
    objRootNode = UTXml.createElement("Object");
    adiNode.appendChild(objRootNode);
    for(int i = 0;i<objVector.size();i++){
        QDomNode objCatalogNode=UTXml.createElement("Object");
        objRootNode.appendChild(objCatalogNode);

        for(int j = 0;j<objVector[i].nodeAttrVector.size();j++){
            objCatalogNode.toElement().setAttribute(objVector[i].nodeAttrVector[j].first,
                                                    objVector[i].nodeAttrVector[j].second);
        }
        for(int j = 0;j<objVector[i].pVector.size();j++){
            QDomNode objPropertyNode=UTXml.createElement("Property");
            objCatalogNode.appendChild(objPropertyNode);

            objPropertyNode.toElement().setAttribute("Name",objVector[i].pVector[j].first);
            if(!objVector[i].pVector[j].second.isEmpty())
                objPropertyNode.appendChild(UTXml.createTextNode(objVector[i].pVector[j].second));

        }

    }


    //处理文件的映射信息
    mapRootNode = UTXml.createElement("Mappings");
    adiNode.appendChild(mapRootNode);
    for(int i = 0;i<mapVector.size();i++){
        QDomNode mapCatalogNode=UTXml.createElement("Mapping");
        for(int j = 0;j<mapVector[i].nodeAttrVector.size();j++){
            mapCatalogNode.toElement().setAttribute(mapVector[i].nodeAttrVector[j].first,
                                                    mapVector[i].nodeAttrVector[j].second);
        }
        for(int j = 0;j<mapVector[i].pVector.size();j++){
            QDomNode mapPropertyNode=UTXml.createElement("Property");
            mapPropertyNode.toElement().setAttribute("Name",mapVector[i].pVector[j].first);
            if(!mapVector[i].pVector[j].second.isEmpty())
                mapPropertyNode.appendChild(UTXml.createTextNode(mapVector[i].pVector[j].second));
            mapCatalogNode.appendChild(mapPropertyNode);
        }
        mapRootNode.appendChild(mapCatalogNode);
    }

    outputString = UTXml.toString();
    return true;
}
