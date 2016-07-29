#include "wasuhandle.h"

WASUHandle::WASUHandle()
{


}

void WASUHandle::findAddItem(const QString & addNodeName,const QString & idName,QStringList &addIDList){
    addIDList.clear();
    QDomNodeList isNoAddAsset =  xmlDoc.elementsByTagName(addNodeName);
    QStringList addAsset;
    for(int i = 0;i<isNoAddAsset.count();i++){
        QDomNode addassNode = isNoAddAsset.item(i);
        QString assetID = addassNode.firstChild().toElement().attribute(idName);//assetID是里面每个元素的ID
        addIDList.append(assetID);
    }
    //int count = addAsset.size();
    return;
}

bool WASUHandle::findAssetIDfromWASU(QMap<QString,QStringList> &xmlID){
    xmlID.clear();
    QStringList addAsset;
    findAddItem("adi:OpenGroupAsset","assetID",addAsset);

    QDomNodeList assContentList =  xmlDoc.elementsByTagName("adi:AssociateContent");
    for(int i = 0;i<assContentList.count();i++){
        QString groupAssetID = assContentList.item(i).toElement().attribute("groupAssetID");//groupAssetID是一个节目的总ID
        QString assetID = assContentList.item(i).toElement().attribute("assetID");//assetID是里面每个元素的ID
        if(!addAsset.contains(groupAssetID))
            continue;
        xmlID[groupAssetID].append(assetID);
    }
    return true;
}

bool WASUHandle::findGourpIDfromWASU(QMap<QString,QStringList> &xmlID){
    xmlID.clear();
    QStringList addAsset;
    findAddItem("adi:OpenGroupAsset","assetID",addAsset);


    QDomNodeList assGroupList =  xmlDoc.elementsByTagName("adi:AssociateGroup");
    for(int i = 0;i<assGroupList.count();i++){
        QString targetGroupAssetID = assGroupList.item(i).toElement().attribute("targetGroupAssetID"); //GROUP的主ID
        QString sourceGroupAssetID = assGroupList.item(i).toElement().attribute("sourceGroupAssetID");    //asset是里面每个元素的ID
        if(!addAsset.contains(sourceGroupAssetID))
            continue;
        xmlID[targetGroupAssetID].append(sourceGroupAssetID);
    }

    return true;
}

void WASUHandle::createFilePath(QString &file){
    if(file.size()<1)
        return;

    QString localPath = "ftp://hnbk:rss123@172.25.52.15:21//NAS/nas02/huashu2.3M/";
    localPath.append(file);
    localPath.append(".ts");
    file = localPath.toHtmlEscaped();
    return;
}

void WASUHandle::createFilePath_HD(QString &file){
    if(file.size()<1)
        return;

    QString localPath = "ftp://hnbk:rss123@172.25.52.15:21//NAS/nas02/huashu4M/";
    localPath.append(file);
    localPath.append(".ts");
    file = localPath.toHtmlEscaped();
    return;
}


void WASUHandle::createPicPath(QString &file){
    if(file.size()<1)
        return;

    QString localPath = "ftp://hnbk:rss123@172.25.52.15:21//NAS/nas02/picture/";
    localPath.append(file);
    localPath.append(".jpg");
    file = localPath;
    return;
}

bool WASUHandle::WASUDatetime2CNTV(const QString& wasuDatetime,QString& cntvDatetime){
    cntvDatetime.clear();
    if(wasuDatetime.size()<1)
        return false;

    QDateTime tpDateTime=QDateTime::fromString(wasuDatetime,"yyyy-MM-dd");
    cntvDatetime = tpDateTime.toString("yyyyMMddhhmmss");
    return true;
}

bool WASUHandle::findGroupAssetIDfromWASU(const QString &GroupID, QStringList &findAssetList){
    QDomNodeList assGroupList =  xmlDoc.elementsByTagName("adi:AssociateContent");
    for(int i = 0;i<assGroupList.count();i++){
        if(assGroupList.item(i).toElement().attribute("assetID")==GroupID){
            findAssetList.append(assGroupList.item(i).toElement().attribute("assetID"));
        }
    }
    return true;
}


bool WASUHandle::WASUID2CNTVID(const int&Type,const QString& wasuCode,QString& cntvCode){
    cntvCode.clear();

    QString wasuSample = "CP23010020160308070700";
    if(wasuCode.size()!=wasuSample.size())
        return false;

    cntvCode = wasuCode;
    QString frontCode;
    if(Type<10)
        frontCode = "000023010000000"+QString::number(Type);
    else
        frontCode = "00002301000000"+QString::number(Type);

    cntvCode.replace("CP2301",frontCode);

    //cntvCode = "";
    return true;
}

bool WASUHandle::WASUID2CNTVID_HD(const int&Type,const QString& wasuCode,QString& cntvCode){
    cntvCode.clear();

    QString wasuSample = "CP23010020160308070700";
    if(wasuCode.size()!=wasuSample.size())
        return false;

    cntvCode = wasuCode;
    QString frontCode;
    if(Type<10)
        frontCode = "000123010000000"+QString::number(Type);
    else
        frontCode = "00012301000000"+QString::number(Type);

    cntvCode.replace("CP2301",frontCode);

    //cntvCode = "";
    return true;
}

void WASUHandle::createXMLforUT(QStringList &outputString,QStringList &outputString_hd){
    if(!xmlDoc.hasChildNodes())
        return;


    //获得目标文件中的adi:AssociateContent assetID合集,assetID就是具体每个Program,pic    
    findAssetIDfromWASU(idVector);

    //获得目标文件中的adi:AssociateGroup assetID合集,取得下属的相关code
    findGourpIDfromWASU(idGroupVector);

    //判断是否连续剧更新
    QString tmpOutputString;
    if(idGroupVector.size()>0){
        //sd
        createSeriesXML(tmpOutputString);
        outputString.append(tmpOutputString);
        handleSeriesXML(outputString);
        //hd
        createSeriesXML_HD(tmpOutputString);
        outputString_hd.append(tmpOutputString);
        handleSeriesXML_HD(outputString_hd);
    }else{
        //sd
        handleMovieXML(outputString);
        //hd
        handleMovieXML_HD(outputString_hd);
    }

    return;
}


void WASUHandle::createSeriesXML_HD(QString &outputString){
    QPair<QString,QString> nodePair;

    UTUnit seriesVector;
    CNTV_XML_Vector seriesXml;
    QString wasuSeriesCode = idGroupVector.firstKey();
    //Serial Parent Code
    QString seriesParentCode;
    QString tmpserialParentCode = wasuSeriesCode;//get parent from targetGroupAssetID
    WASUID2CNTVID_HD(CNTV_T_Series,tmpserialParentCode,seriesParentCode);

    //This Series Code (Pic is a Parent)
    QString seriesPicCode;
    tmpserialParentCode = wasuSeriesCode;
    WASUID2CNTVID_HD(CNTV_T_Picture,tmpserialParentCode,seriesPicCode);

    //处理series的Object信息
    nodePair.first = "Action";
    nodePair.second = "REGIST";
    seriesXml.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementType";
    nodePair.second = "Series";
    seriesXml.nodeAttrVector.append(nodePair);

    nodePair.first = "Code";
    nodePair.second = seriesParentCode;
    seriesXml.nodeAttrVector.append(nodePair);

    nodePair.first = "ID";
    nodePair.second = seriesParentCode;
    seriesXml.nodeAttrVector.append(nodePair);

    //series的property
    nodePair.first = "CPID";
    nodePair.second = "WASUCP";
    seriesXml.pVector.append(nodePair);

    QString tmpstartDateTime;
    findAttrValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"adi:AssetLifetime","startDateTime",tmpstartDateTime);
    QString videoStartTime;
    WASUDatetime2CNTV(tmpstartDateTime,videoStartTime);
    nodePair.first = "LicensingWindowStart";
    nodePair.second = videoStartTime;
    seriesXml.pVector.append(nodePair);

    QString tmpItemCount;
    findValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"vod:Items",tmpItemCount);
    nodePair.first = "VolumnCount";
    nodePair.second = tmpItemCount;
    seriesXml.pVector.append(nodePair);

    QString tmpendDateTime;
    findAttrValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"adi:AssetLifetime","endDateTime",tmpendDateTime);
    QString videoEndTime;
    WASUDatetime2CNTV(tmpendDateTime,videoEndTime);
    nodePair.first = "LicensingWindowEnd";
    nodePair.second = videoEndTime;
    seriesXml.pVector.append(nodePair);

    QString tmpTitleFull;
    findValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"vod:TitleFull",tmpTitleFull);
    QString tmpTitleFull1 = tmpTitleFull+"HD";
    nodePair.first = "Name";
    nodePair.second = tmpTitleFull1;
    seriesXml.pVector.append(nodePair);

    QString tmpActor;
    findValueOnCond("vod:Actor",QStringList()<<"Actor","vod:LastNameFirst",tmpActor);
    nodePair.first = "Kpeople";
    nodePair.second = tmpActor;
    seriesXml.pVector.append(nodePair);

    QString tmpDirector;
    findValueOnCond("vod:Director","vod:LastNameFirst",tmpDirector);
    nodePair.first = "Director";
    nodePair.second = tmpDirector;
    seriesXml.pVector.append(nodePair);

    QString tmpSummaryMedium;
    findValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"vod:SummaryMedium",tmpSummaryMedium);
    nodePair.first = "Description";
    nodePair.second = tmpSummaryMedium;
    seriesXml.pVector.append(nodePair);

    seriesVector.appendObj(seriesXml);

    //处理图片
    CNTV_XML_Vector cntvVector_picture;
    nodePair.first = "Action";
    nodePair.second = "REGIST";
    cntvVector_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "Code";
    nodePair.second = seriesPicCode;
    cntvVector_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementType";
    nodePair.second = "Picture";
    cntvVector_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ID";
    nodePair.second = seriesPicCode;
    cntvVector_picture.nodeAttrVector.append(nodePair);

    //object的property
    nodePair.first = "FileURL";
    QString pic_path = tmpTitleFull;
    createPicPath(pic_path);
    nodePair.second= pic_path;
    cntvVector_picture.pVector.append(nodePair);

    seriesVector.appendObj(cntvVector_picture);


    //处理MAPPING的PIC
    CNTV_XML_Vector series_map_picture;

    nodePair.first = "Action";
    nodePair.second = "REGIST";
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementCode";
    nodePair.second = seriesParentCode;
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementID";
    nodePair.second = seriesParentCode;
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementType";
    nodePair.second = "Series";
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ParentCode";
    nodePair.second = seriesPicCode;
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ParentID";
    nodePair.second = seriesPicCode;
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ParentType";
    nodePair.second = "Picture";
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "Sequence";
    nodePair.second = "99";
    series_map_picture.pVector.append(nodePair);

    nodePair.first = "Type";
    nodePair.second = "1";
    series_map_picture.pVector.append(nodePair);

    seriesVector.appendMap(series_map_picture);

    seriesVector.createUTXML(outputString);


}

void WASUHandle::handleSeriesXML_HD(QStringList &outputString){

    QPair<QString,QString> nodePair;
    QString wasuSeriesCode = idGroupVector.firstKey();
    //Serial Parent Code
    QString seriesParentCode;
    QString tmpserialParentCode = wasuSeriesCode;//get parent from targetGroupAssetID
    WASUID2CNTVID_HD(CNTV_T_Series,tmpserialParentCode,seriesParentCode);

    //============================================处理连续剧单集==================================================

    QMapIterator<QString, QStringList> i(idVector);
    while (i.hasNext()) {
        i.next();

        UTUnit tpUTVector;

        //初始化临时变量
        QString XML_TitleFull;
        QString programCode;
        //某一个groupAssetID
        QString tmpGroupAssetID = i.key();
        QString tmpserialParentCode = tmpGroupAssetID;//set parent with groupAssetID
        WASUID2CNTVID_HD(CNTV_T_Program,tmpserialParentCode,programCode);
        //============================================处理OBJECT下的PROGRAM信息==================================================
        CNTV_XML_Vector cntvVector_program;

        //object的node参数
        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "Code";
        nodePair.second = programCode;
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ID";
        nodePair.second = programCode;
        cntvVector_program.nodeAttrVector.append(nodePair);


        //object的property
        nodePair.first = "CPID";
        nodePair.second = "WASUCP";
        cntvVector_program.pVector.append(nodePair);

        nodePair.first = "SeriesFlag";
        nodePair.second = "1";
        cntvVector_program.pVector.append(nodePair);

        QString tmpTitleFull;
        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:TitleFull",tmpTitleFull);
        XML_TitleFull = tmpTitleFull;
        tmpTitleFull.append("HD");
        nodePair.first = "Name";
        nodePair.second = tmpTitleFull;        
        cntvVector_program.pVector.append(nodePair);

        QString tmpstartDateTime;
        findAttrValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"adi:AssetLifetime","startDateTime",tmpstartDateTime);
        QString videoStartTime;
        WASUDatetime2CNTV(tmpstartDateTime,videoStartTime);
        nodePair.first = "LicensingWindowStart";
        nodePair.second = videoStartTime;
        cntvVector_program.pVector.append(nodePair);

        QString tmpendDateTime;
        findAttrValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"adi:AssetLifetime","endDateTime",tmpendDateTime);
        QString videoEndTime;
        WASUDatetime2CNTV(tmpendDateTime,videoEndTime);
        nodePair.first = "LicensingWindowEnd";
        nodePair.second = videoEndTime;
        cntvVector_program.pVector.append(nodePair);

        QString tmpActor;
        findValueOnCond("vod:Actor",QStringList()<<"Actor","vod:LastNameFirst",tmpActor);
        nodePair.first = "Kpeople";
        nodePair.second = tmpActor;
        cntvVector_program.pVector.append(nodePair);

        QString tmpDuration;
        QString tmpFindDurationCode = i.value().first();
        findValueOnCond("vod:Video",QStringList()<<tmpFindDurationCode,"vod:Duration",tmpDuration);
        nodePair.first = "Duration";
        nodePair.second = QString::number(tmpDuration.toInt()/60);
        //nodePair.second = tmpDuration;
        cntvVector_program.pVector.append(nodePair);

        QString tmpDirector;
        findValueOnCond("vod:Director","vod:LastNameFirst",tmpDirector);
        nodePair.first = "Director";
        nodePair.second = tmpDirector;
        cntvVector_program.pVector.append(nodePair);

        QString tmpSummaryMedium;
        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:SummaryMedium",tmpSummaryMedium);
        nodePair.first = "Description";
        nodePair.second = tmpSummaryMedium;
        cntvVector_program.pVector.append(nodePair);

        tpUTVector.appendObj(cntvVector_program);


        //===============================================处理MAPPING的PROGRAM===============================================
        CNTV_XML_Vector cntvVector_map_program;

        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementCode";
        nodePair.second = programCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementID";
        nodePair.second = programCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        //object的property
        QString tmpEpisodeID;
        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:EpisodeID",tmpEpisodeID);
        nodePair.first = "Sequence";
        nodePair.second = tmpEpisodeID;
        cntvVector_map_program.pVector.append(nodePair);


        nodePair.first = "ParentType";
        nodePair.second = "Series";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ParentCode";
        nodePair.second = seriesParentCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ParentID";
        nodePair.second = seriesParentCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);


        tpUTVector.appendMap(cntvVector_map_program);


       //处理关联在同一个Asset下的资源
        QString tmpContentID;
        QDomNodeList assGroupList =  xmlDoc.elementsByTagName("adi:AssociateContent");
        foreach (tmpContentID, i.value()) {
            QString contentType;

            for(int j = 0;j<assGroupList.count();j++){
                if(assGroupList.item(j).toElement().attribute("assetID")==tmpContentID){
                    contentType = assGroupList.item(j).toElement().attribute("type");
                    break;
                }
            }
            QString assetCNTVCode;


            if(contentType=="Video") {
                WASUID2CNTVID_HD(CNTV_T_Movie,tmpContentID,assetCNTVCode);
                //============================================处理OBJECT下的FILE信息==================================================
                CNTV_XML_Vector cntvVector_file;
                nodePair.first = "Action";
                nodePair.second = "REGIST";
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "Code";
                nodePair.second = assetCNTVCode;
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementType";
                nodePair.second = "Movie";
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ID";
                nodePair.second = assetCNTVCode;
                cntvVector_file.nodeAttrVector.append(nodePair);


                //object的property
                nodePair.first = "FileURL";
                QString file_path = XML_TitleFull;
                createFilePath_HD(file_path);
                nodePair.second = file_path;
                cntvVector_file.pVector.append(nodePair);

                nodePair.first = "Type";
                nodePair.second = "1";
                cntvVector_file.pVector.append(nodePair);

                nodePair.first = "MediaSpec";
                nodePair.second = "TS-VBR-H.264-4000-1080i-25-MP2-128";
                cntvVector_file.pVector.append(nodePair);

                tpUTVector.appendObj(cntvVector_file);

                //===============================================处理MAPPING的FILE===============================================
                CNTV_XML_Vector cntvVector_map_file;
                nodePair.first = "Action";
                nodePair.second = "REGIST";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementCode";
                nodePair.second = assetCNTVCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementID";
                nodePair.second = assetCNTVCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementType";
                nodePair.second = "Movie";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentCode";
                nodePair.second = programCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentID";
                nodePair.second = programCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentType";
                nodePair.second = "Program";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                tpUTVector.appendMap(cntvVector_map_file);

               }

        }
        QString thisProgram;
        tpUTVector.createUTXML(thisProgram);
        outputString.append(thisProgram);
       // break;
    }
}

void WASUHandle::handleMovieXML_HD(QStringList&outputString){
    QPair<QString,QString> nodePair;

    QMapIterator<QString, QStringList> i(idVector);
    while (i.hasNext()) {
        i.next();

        UTUnit tpUTVector;
        QPair<QString,QString> nodePair;


        //初始化临时变量
        QString tmpTitleFull;
        QString tmpGroupAssetID = i.key();
        QString programCode;
        WASUID2CNTVID_HD(CNTV_T_Program,tmpGroupAssetID,programCode);
        QString pictrueCode;
        WASUID2CNTVID_HD(CNTV_T_Picture,tmpGroupAssetID,pictrueCode);


        //============================================处理OBJECT下的PROGRAM信息==================================================
        CNTV_XML_Vector cntvVector_program;

        //object的node参数
        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "Code";
        nodePair.second = programCode;
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ID";
        nodePair.second = programCode;
        cntvVector_program.nodeAttrVector.append(nodePair);

        //object的property
        nodePair.first = "CPID";
        nodePair.second = "WASUCP";
        cntvVector_program.pVector.append(nodePair);


        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:TitleFull",tmpTitleFull);
        QString tmpTitleFull1 = tmpTitleFull + "HD";
        nodePair.first = "Name";
        nodePair.second = tmpTitleFull1;
        cntvVector_program.pVector.append(nodePair);

        QString tmpstartDateTime;
        findAttrValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"adi:AssetLifetime","startDateTime",tmpstartDateTime);
        QString videoStartTime;
        WASUDatetime2CNTV(tmpstartDateTime,videoStartTime);
        nodePair.first = "LicensingWindowStart";
        nodePair.second = videoStartTime;
        cntvVector_program.pVector.append(nodePair);

        QString tmpendDateTime;
        findAttrValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"adi:AssetLifetime","endDateTime",tmpendDateTime);
        QString videoEndTime;
        WASUDatetime2CNTV(tmpendDateTime,videoEndTime);
        nodePair.first = "LicensingWindowEnd";
        nodePair.second = videoEndTime;
        cntvVector_program.pVector.append(nodePair);

        QString tmpActor;
        findValueOnCond("vod:Actor",QStringList()<<"Actor","vod:LastNameFirst",tmpActor);
        nodePair.first = "Kpeople";
        nodePair.second = tmpActor;
        cntvVector_program.pVector.append(nodePair);

        QString tmpDuration;
        QString tmpFindDurationCode = i.value().first();
        findValueOnCond("vod:Video",QStringList()<<tmpFindDurationCode,"vod:Duration",tmpDuration);
        nodePair.first = "Duration";
        nodePair.second = QString::number(tmpDuration.toInt()/60);
        //nodePair.second = tmpDuration;
        cntvVector_program.pVector.append(nodePair);

        QString tmpDirector;
        findValueOnCond("vod:Director","vod:LastNameFirst",tmpDirector);
        nodePair.first = "Director";
        nodePair.second = tmpDirector;
        cntvVector_program.pVector.append(nodePair);

        QString tmpSummaryMedium;
        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:SummaryMedium",tmpSummaryMedium);
        nodePair.first = "Description";
        nodePair.second = tmpSummaryMedium;
        cntvVector_program.pVector.append(nodePair);

        tpUTVector.appendObj(cntvVector_program);


        //===============================================处理PROGRAM的MAPPING===============================================
        CNTV_XML_Vector cntvVector_map_program;

        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementCode";
        nodePair.second = programCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementID";
        nodePair.second = programCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        //object的property

        nodePair.first = "ParentType";
        nodePair.second = "Picture";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ParentCode";
        nodePair.second = pictrueCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ParentID";
        nodePair.second = pictrueCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "Sequence";
        nodePair.second = "99";
        cntvVector_map_program.pVector.append(nodePair);

        nodePair.first = "Type";
        nodePair.second = "1";
        cntvVector_map_program.pVector.append(nodePair);


        tpUTVector.appendMap(cntvVector_map_program);



        //============================================处理OBJECT下的PIC信息==================================================
        CNTV_XML_Vector cntvVector_picture;
        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_picture.nodeAttrVector.append(nodePair);

        nodePair.first = "Code";
        nodePair.second = pictrueCode;
        cntvVector_picture.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Picture";
        cntvVector_picture.nodeAttrVector.append(nodePair);

        nodePair.first = "ID";
        nodePair.second = pictrueCode;
        cntvVector_picture.nodeAttrVector.append(nodePair);


        //object的property
        nodePair.first = "FileURL";
        QString pic_path = tmpTitleFull;
        createPicPath(pic_path);
        nodePair.second= pic_path;
        cntvVector_picture.pVector.append(nodePair);



        tpUTVector.appendObj(cntvVector_picture);


       //处理关联在同一个Asset下的资源
        QString tmpContentID;
        foreach (tmpContentID, i.value()) {
            QString contentType;
            QDomNodeList assGroupList =  xmlDoc.elementsByTagName("adi:AssociateContent");
            for(int i = 0;i<assGroupList.count();i++){
                if(assGroupList.item(i).toElement().attribute("assetID")==tmpContentID){
                    contentType = assGroupList.item(i).toElement().attribute("type");
                    break;
                }
            }
            QString assetCNTVCode;

            if(contentType=="Video") {
                WASUID2CNTVID_HD(CNTV_T_Movie,tmpContentID,assetCNTVCode);
                //============================================处理OBJECT下的FILE信息==================================================
                CNTV_XML_Vector cntvVector_file;
                nodePair.first = "Action";
                nodePair.second = "REGIST";
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "Code";
                nodePair.second = assetCNTVCode;
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementType";
                nodePair.second = "Movie";
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ID";
                nodePair.second = assetCNTVCode;
                cntvVector_file.nodeAttrVector.append(nodePair);


                //object的property
                nodePair.first = "FileURL";
                QString file_path = tmpTitleFull;
                createFilePath_HD(file_path);
                nodePair.second = file_path;
                cntvVector_file.pVector.append(nodePair);

                nodePair.first = "Type";
                nodePair.second = "1";
                cntvVector_file.pVector.append(nodePair);

                nodePair.first = "MediaSpec";
                nodePair.second = "TS-VBR-H.264-4000-1080i-25-MP2-128";
                cntvVector_file.pVector.append(nodePair);

                tpUTVector.appendObj(cntvVector_file);

                //===============================================处理MAPPING的FILE===============================================
                CNTV_XML_Vector cntvVector_map_file;
                nodePair.first = "Action";
                nodePair.second = "REGIST";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementCode";
                nodePair.second = assetCNTVCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementID";
                nodePair.second = assetCNTVCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementType";
                nodePair.second = "Movie";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentCode";
                nodePair.second = programCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentID";
                nodePair.second = programCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentType";
                nodePair.second = "Program";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                tpUTVector.appendMap(cntvVector_map_file);

            }

        }
        QString thisProgram;
        tpUTVector.createUTXML(thisProgram);
        outputString.append(thisProgram);
       // break;
    }

}
void WASUHandle::createSeriesXML(QString &outputString){
    QPair<QString,QString> nodePair;

    UTUnit seriesVector;
    CNTV_XML_Vector seriesXml;
    QString wasuSeriesCode = idGroupVector.firstKey();
    //Serial Parent Code
    QString seriesParentCode;
    QString tmpserialParentCode = wasuSeriesCode;//get parent from targetGroupAssetID
    WASUID2CNTVID(CNTV_T_Series,tmpserialParentCode,seriesParentCode);

    //This Series Code (Pic is a Parent)
    QString seriesPicCode;
    tmpserialParentCode = wasuSeriesCode;
    WASUID2CNTVID(CNTV_T_Picture,tmpserialParentCode,seriesPicCode);

    //处理series的Object信息
    nodePair.first = "Action";
    nodePair.second = "REGIST";
    seriesXml.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementType";
    nodePair.second = "Series";
    seriesXml.nodeAttrVector.append(nodePair);

    nodePair.first = "Code";
    nodePair.second = seriesParentCode;
    seriesXml.nodeAttrVector.append(nodePair);

    nodePair.first = "ID";
    nodePair.second = seriesParentCode;
    seriesXml.nodeAttrVector.append(nodePair);

    //series的property
    nodePair.first = "CPID";
    nodePair.second = "WASUCP";
    seriesXml.pVector.append(nodePair);

    QString tmpstartDateTime;
    findAttrValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"adi:AssetLifetime","startDateTime",tmpstartDateTime);
    QString videoStartTime;
    WASUDatetime2CNTV(tmpstartDateTime,videoStartTime);
    nodePair.first = "LicensingWindowStart";
    nodePair.second = videoStartTime;
    seriesXml.pVector.append(nodePair);

    QString tmpItemCount;
    findValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"vod:Items",tmpItemCount);
    nodePair.first = "VolumnCount";
    nodePair.second = tmpItemCount;
    seriesXml.pVector.append(nodePair);

    QString tmpendDateTime;
    findAttrValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"adi:AssetLifetime","endDateTime",tmpendDateTime);
    QString videoEndTime;
    WASUDatetime2CNTV(tmpendDateTime,videoEndTime);
    nodePair.first = "LicensingWindowEnd";
    nodePair.second = videoEndTime;
    seriesXml.pVector.append(nodePair);

    QString tmpTitleFull;
    findValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"vod:TitleFull",tmpTitleFull);
    nodePair.first = "Name";
    nodePair.second = tmpTitleFull;
    seriesXml.pVector.append(nodePair);

    QString tmpActor;
    findValueOnCond("vod:Actor",QStringList()<<"Actor","vod:LastNameFirst",tmpActor);
    nodePair.first = "Kpeople";
    nodePair.second = tmpActor;
    seriesXml.pVector.append(nodePair);

    QString tmpDirector;
    findValueOnCond("vod:Director","vod:LastNameFirst",tmpDirector);
    nodePair.first = "Director";
    nodePair.second = tmpDirector;
    seriesXml.pVector.append(nodePair);

    QString tmpSummaryMedium;
    findValueOnCond("vod:Title",QStringList()<<wasuSeriesCode,"vod:SummaryMedium",tmpSummaryMedium);
    nodePair.first = "Description";
    nodePair.second = tmpSummaryMedium;
    seriesXml.pVector.append(nodePair);

    seriesVector.appendObj(seriesXml);

    //处理图片
    CNTV_XML_Vector cntvVector_picture;
    nodePair.first = "Action";
    nodePair.second = "REGIST";
    cntvVector_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "Code";
    nodePair.second = seriesPicCode;
    cntvVector_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementType";
    nodePair.second = "Picture";
    cntvVector_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ID";
    nodePair.second = seriesPicCode;
    cntvVector_picture.nodeAttrVector.append(nodePair);

    //object的property
    nodePair.first = "FileURL";
    QString pic_path = tmpTitleFull;
    createPicPath(pic_path);
    nodePair.second= pic_path;
    cntvVector_picture.pVector.append(nodePair);

    seriesVector.appendObj(cntvVector_picture);


    //处理MAPPING的PIC
    CNTV_XML_Vector series_map_picture;

    nodePair.first = "Action";
    nodePair.second = "REGIST";
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementCode";
    nodePair.second = seriesParentCode;
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementID";
    nodePair.second = seriesParentCode;
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ElementType";
    nodePair.second = "Series";
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ParentCode";
    nodePair.second = seriesPicCode;
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ParentID";
    nodePair.second = seriesPicCode;
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "ParentType";
    nodePair.second = "Picture";
    series_map_picture.nodeAttrVector.append(nodePair);

    nodePair.first = "Sequence";
    nodePair.second = "99";
    series_map_picture.pVector.append(nodePair);

    nodePair.first = "Type";
    nodePair.second = "1";
    series_map_picture.pVector.append(nodePair);

    seriesVector.appendMap(series_map_picture);

    seriesVector.createUTXML(outputString);


}

void WASUHandle::handleSeriesXML(QStringList &outputString){

    QPair<QString,QString> nodePair;
    QString wasuSeriesCode = idGroupVector.firstKey();
    //Serial Parent Code
    QString seriesParentCode;
    QString tmpserialParentCode = wasuSeriesCode;//get parent from targetGroupAssetID
    WASUID2CNTVID(CNTV_T_Series,tmpserialParentCode,seriesParentCode);

    //============================================处理连续剧单集==================================================

    QMapIterator<QString, QStringList> i(idVector);
    while (i.hasNext()) {
        i.next();

        UTUnit tpUTVector;

        //初始化临时变量
        QString XML_TitleFull;
        QString programCode;
        //某一个groupAssetID
        QString tmpGroupAssetID = i.key();
        QString tmpserialParentCode = tmpGroupAssetID;//set parent with groupAssetID
        WASUID2CNTVID(CNTV_T_Program,tmpserialParentCode,programCode);
        //============================================处理OBJECT下的PROGRAM信息==================================================
        CNTV_XML_Vector cntvVector_program;

        //object的node参数
        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "Code";
        nodePair.second = programCode;
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ID";
        nodePair.second = programCode;
        cntvVector_program.nodeAttrVector.append(nodePair);


        //object的property
        nodePair.first = "CPID";
        nodePair.second = "WASUCP";
        cntvVector_program.pVector.append(nodePair);

        nodePair.first = "SeriesFlag";
        nodePair.second = "1";
        cntvVector_program.pVector.append(nodePair);

        QString tmpTitleFull;
        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:TitleFull",tmpTitleFull);
        nodePair.first = "Name";
        nodePair.second = tmpTitleFull;
        XML_TitleFull = tmpTitleFull;
        cntvVector_program.pVector.append(nodePair);

        QString tmpstartDateTime;
        findAttrValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"adi:AssetLifetime","startDateTime",tmpstartDateTime);
        QString videoStartTime;
        WASUDatetime2CNTV(tmpstartDateTime,videoStartTime);
        nodePair.first = "LicensingWindowStart";
        nodePair.second = videoStartTime;
        cntvVector_program.pVector.append(nodePair);

        QString tmpendDateTime;
        findAttrValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"adi:AssetLifetime","endDateTime",tmpendDateTime);
        QString videoEndTime;
        WASUDatetime2CNTV(tmpendDateTime,videoEndTime);
        nodePair.first = "LicensingWindowEnd";
        nodePair.second = videoEndTime;
        cntvVector_program.pVector.append(nodePair);

        QString tmpActor;
        findValueOnCond("vod:Actor",QStringList()<<"Actor","vod:LastNameFirst",tmpActor);
        nodePair.first = "Kpeople";
        nodePair.second = tmpActor;
        cntvVector_program.pVector.append(nodePair);

        QString tmpDuration;
        QString tmpFindDurationCode = i.value().first();
        findValueOnCond("vod:Video",QStringList()<<tmpFindDurationCode,"vod:Duration",tmpDuration);
        nodePair.first = "Duration";
        nodePair.second = QString::number(tmpDuration.toInt()/60);
        //nodePair.second = tmpDuration;
        cntvVector_program.pVector.append(nodePair);

        QString tmpDirector;
        findValueOnCond("vod:Director","vod:LastNameFirst",tmpDirector);
        nodePair.first = "Director";
        nodePair.second = tmpDirector;
        cntvVector_program.pVector.append(nodePair);

        QString tmpSummaryMedium;
        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:SummaryMedium",tmpSummaryMedium);
        nodePair.first = "Description";
        nodePair.second = tmpSummaryMedium;
        cntvVector_program.pVector.append(nodePair);

        tpUTVector.appendObj(cntvVector_program);


        //===============================================处理MAPPING的PROGRAM===============================================
        CNTV_XML_Vector cntvVector_map_program;

        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementCode";
        nodePair.second = programCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementID";
        nodePair.second = programCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        //object的property
        QString tmpEpisodeID;
        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:EpisodeID",tmpEpisodeID);
        nodePair.first = "Sequence";
        nodePair.second = tmpEpisodeID;
        cntvVector_map_program.pVector.append(nodePair);


        nodePair.first = "ParentType";
        nodePair.second = "Series";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ParentCode";
        nodePair.second = seriesParentCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ParentID";
        nodePair.second = seriesParentCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);


        tpUTVector.appendMap(cntvVector_map_program);


       //处理关联在同一个Asset下的资源
        QString tmpContentID;
        QDomNodeList assGroupList =  xmlDoc.elementsByTagName("adi:AssociateContent");
        foreach (tmpContentID, i.value()) {
            QString contentType;

            for(int j = 0;j<assGroupList.count();j++){
                if(assGroupList.item(j).toElement().attribute("assetID")==tmpContentID){
                    contentType = assGroupList.item(j).toElement().attribute("type");
                    break;
                }
            }
            QString assetCNTVCode;


            if(contentType=="Video") {
                WASUID2CNTVID(CNTV_T_Movie,tmpContentID,assetCNTVCode);
                //============================================处理OBJECT下的FILE信息==================================================
                CNTV_XML_Vector cntvVector_file;
                nodePair.first = "Action";
                nodePair.second = "REGIST";
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "Code";
                nodePair.second = assetCNTVCode;
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementType";
                nodePair.second = "Movie";
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ID";
                nodePair.second = assetCNTVCode;
                cntvVector_file.nodeAttrVector.append(nodePair);


                //object的property
                nodePair.first = "FileURL";
                QString file_path = XML_TitleFull;
                createFilePath(file_path);
                nodePair.second = file_path;
                cntvVector_file.pVector.append(nodePair);

                nodePair.first = "Type";
                nodePair.second = "1";
                cntvVector_file.pVector.append(nodePair);

                nodePair.first = "MediaSpec";
                nodePair.second = "TS-CBR-H.264-2300-576i-25-MP2-128";
                cntvVector_file.pVector.append(nodePair);


                tpUTVector.appendObj(cntvVector_file);

                //===============================================处理MAPPING的FILE===============================================
                CNTV_XML_Vector cntvVector_map_file;
                nodePair.first = "Action";
                nodePair.second = "REGIST";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementCode";
                nodePair.second = assetCNTVCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementID";
                nodePair.second = assetCNTVCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementType";
                nodePair.second = "Movie";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentCode";
                nodePair.second = programCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentID";
                nodePair.second = programCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentType";
                nodePair.second = "Program";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                tpUTVector.appendMap(cntvVector_map_file);

               }

        }
        QString thisProgram;
        tpUTVector.createUTXML(thisProgram);
        outputString.append(thisProgram);
       // break;
    }
}

void WASUHandle::handleMovieXML(QStringList&outputString){
    QPair<QString,QString> nodePair;

    QMapIterator<QString, QStringList> i(idVector);
    while (i.hasNext()) {
        i.next();

        UTUnit tpUTVector;
        QPair<QString,QString> nodePair;


        //初始化临时变量
        QString tmpTitleFull;
        QString tmpGroupAssetID = i.key();
        QString programCode;
        WASUID2CNTVID(CNTV_T_Program,tmpGroupAssetID,programCode);
        QString pictrueCode;
        WASUID2CNTVID(CNTV_T_Picture,tmpGroupAssetID,pictrueCode);


        //============================================处理OBJECT下的PROGRAM信息==================================================
        CNTV_XML_Vector cntvVector_program;

        //object的node参数
        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "Code";
        nodePair.second = programCode;
        cntvVector_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ID";
        nodePair.second = programCode;
        cntvVector_program.nodeAttrVector.append(nodePair);

        //object的property
        nodePair.first = "CPID";
        nodePair.second = "WASUCP";
        cntvVector_program.pVector.append(nodePair);


        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:TitleFull",tmpTitleFull);
        nodePair.first = "Name";
        nodePair.second = tmpTitleFull;
        cntvVector_program.pVector.append(nodePair);

        QString tmpstartDateTime;
        findAttrValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"adi:AssetLifetime","startDateTime",tmpstartDateTime);
        QString videoStartTime;
        WASUDatetime2CNTV(tmpstartDateTime,videoStartTime);
        nodePair.first = "LicensingWindowStart";
        nodePair.second = videoStartTime;
        cntvVector_program.pVector.append(nodePair);

        QString tmpendDateTime;
        findAttrValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"adi:AssetLifetime","endDateTime",tmpendDateTime);
        QString videoEndTime;
        WASUDatetime2CNTV(tmpendDateTime,videoEndTime);
        nodePair.first = "LicensingWindowEnd";
        nodePair.second = videoEndTime;
        cntvVector_program.pVector.append(nodePair);

        QString tmpActor;
        findValueOnCond("vod:Actor",QStringList()<<"Actor","vod:LastNameFirst",tmpActor);
        nodePair.first = "Kpeople";
        nodePair.second = tmpActor;
        cntvVector_program.pVector.append(nodePair);

        QString tmpDuration;
        QString tmpFindDurationCode = i.value().first();
        findValueOnCond("vod:Video",QStringList()<<tmpFindDurationCode,"vod:Duration",tmpDuration);
        nodePair.first = "Duration";
        nodePair.second = QString::number(tmpDuration.toInt()/60);
        //nodePair.second = tmpDuration;
        cntvVector_program.pVector.append(nodePair);

        QString tmpDirector;
        findValueOnCond("vod:Director","vod:LastNameFirst",tmpDirector);
        nodePair.first = "Director";
        nodePair.second = tmpDirector;
        cntvVector_program.pVector.append(nodePair);

        QString tmpSummaryMedium;
        findValueOnCond("vod:Title",QStringList()<<tmpGroupAssetID,"vod:SummaryMedium",tmpSummaryMedium);
        nodePair.first = "Description";
        nodePair.second = tmpSummaryMedium;
        cntvVector_program.pVector.append(nodePair);

        tpUTVector.appendObj(cntvVector_program);


        //===============================================处理PROGRAM的MAPPING===============================================
        CNTV_XML_Vector cntvVector_map_program;

        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementCode";
        nodePair.second = programCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementID";
        nodePair.second = programCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Program";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        //object的property

        nodePair.first = "ParentType";
        nodePair.second = "Picture";
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ParentCode";
        nodePair.second = pictrueCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "ParentID";
        nodePair.second = pictrueCode;
        cntvVector_map_program.nodeAttrVector.append(nodePair);

        nodePair.first = "Sequence";
        nodePair.second = "99";
        cntvVector_map_program.pVector.append(nodePair);

        nodePair.first = "Type";
        nodePair.second = "1";
        cntvVector_map_program.pVector.append(nodePair);


        tpUTVector.appendMap(cntvVector_map_program);



        //============================================处理OBJECT下的PIC信息==================================================
        CNTV_XML_Vector cntvVector_picture;
        nodePair.first = "Action";
        nodePair.second = "REGIST";
        cntvVector_picture.nodeAttrVector.append(nodePair);

        nodePair.first = "Code";
        nodePair.second = pictrueCode;
        cntvVector_picture.nodeAttrVector.append(nodePair);

        nodePair.first = "ElementType";
        nodePair.second = "Picture";
        cntvVector_picture.nodeAttrVector.append(nodePair);

        nodePair.first = "ID";
        nodePair.second = pictrueCode;
        cntvVector_picture.nodeAttrVector.append(nodePair);


        //object的property
        nodePair.first = "FileURL";
        QString pic_path = tmpTitleFull;
        createPicPath(pic_path);
        nodePair.second= pic_path;
        cntvVector_picture.pVector.append(nodePair);



        tpUTVector.appendObj(cntvVector_picture);


       //处理关联在同一个Asset下的资源
        QString tmpContentID;
        foreach (tmpContentID, i.value()) {
            QString contentType;
            QDomNodeList assGroupList =  xmlDoc.elementsByTagName("adi:AssociateContent");
            for(int i = 0;i<assGroupList.count();i++){
                if(assGroupList.item(i).toElement().attribute("assetID")==tmpContentID){
                    contentType = assGroupList.item(i).toElement().attribute("type");
                    break;
                }
            }
            QString assetCNTVCode;

            if(contentType=="Video") {
                WASUID2CNTVID(CNTV_T_Movie,tmpContentID,assetCNTVCode);
                //============================================处理OBJECT下的FILE信息==================================================
                CNTV_XML_Vector cntvVector_file;
                nodePair.first = "Action";
                nodePair.second = "REGIST";
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "Code";
                nodePair.second = assetCNTVCode;
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementType";
                nodePair.second = "Movie";
                cntvVector_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ID";
                nodePair.second = assetCNTVCode;
                cntvVector_file.nodeAttrVector.append(nodePair);


                //object的property
                nodePair.first = "FileURL";
                QString file_path = tmpTitleFull;
                createFilePath(file_path);
                nodePair.second = file_path;
                cntvVector_file.pVector.append(nodePair);

                nodePair.first = "Type";
                nodePair.second = "1";
                cntvVector_file.pVector.append(nodePair);

                nodePair.first = "MediaSpec";
                nodePair.second = "TS-CBR-H.264-2300-576i-25-MP2-128";
                cntvVector_file.pVector.append(nodePair);

                tpUTVector.appendObj(cntvVector_file);

                //===============================================处理MAPPING的FILE===============================================
                CNTV_XML_Vector cntvVector_map_file;
                nodePair.first = "Action";
                nodePair.second = "REGIST";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementCode";
                nodePair.second = assetCNTVCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementID";
                nodePair.second = assetCNTVCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ElementType";
                nodePair.second = "Movie";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentCode";
                nodePair.second = programCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentID";
                nodePair.second = programCode;
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                nodePair.first = "ParentType";
                nodePair.second = "Program";
                cntvVector_map_file.nodeAttrVector.append(nodePair);

                tpUTVector.appendMap(cntvVector_map_file);

            }

        }
        QString thisProgram;
        tpUTVector.createUTXML(thisProgram);
        outputString.append(thisProgram);
       // break;
    }

}
