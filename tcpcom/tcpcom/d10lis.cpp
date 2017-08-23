#include "d10lis.h"
#include <string>
#include "sqlite3pp.h"

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

void splitString(const std::string& str, std::vector<std::string>& toks, char delim)
{
    toks.clear();

    size_t last = 0;
    size_t next = 0;
    while ((next = str.find(delim, last)) != std::string::npos)
    {
        toks.push_back(str.substr(last, next - last));
        last = next + 1;
    }
    toks.push_back(str.substr(last));
}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

void RangeId::parse(const std::string& str, char delim)
{
    std::vector<std::string> temp;
    splitString(str, temp, delim);

    if (temp.size() == 2)
    {
        sPatientId = temp[0];
        sSpecimenId = temp[1];
    }
}

void TestId::parse(const std::string& str, char delim)
{
    if (str.empty())
        return;
    
    std::vector<std::string> temp;
    splitString(str, temp, delim);

    t1 = temp[0];
    t2 = temp[1];
    t3 = temp[2];
    t4 = temp[3];
    if (temp.size() > 4)
        t5 = temp[4];
}

std::string datetime::getDate()
{
    if (!year || !month || !day)
        return("");
    char buf[9];
    sprintf(buf, "%04d%02d%02d", year, month, day);
    std::string str(buf);
    return(str);
}


std::string datetime::getTime()
{
    char buf[7];
    sprintf(buf, "%02d%02d%02d", hour, minute, second);
    std::string str(buf);
    return(str);
}


std::string datetime::getDT()
{
    if (!year || !month || !day)
        return("");
    char buf[15];
    sprintf(buf, "%04d%02d%02d%02d%02d%02d", year, month, day,hour,minute,second);
    std::string str(buf);
    return(str);
}

void datetime::parseDate(const std::string& dtStr)
{
    switch (dtStr.length())
    {
        case 6: //Time
            hour = std::stoi(dtStr.substr(0, 2));
            minute = std::stoi(dtStr.substr(2, 2));
            second = std::stoi(dtStr.substr(4, 2));
            break;
        case 8: //Date
            year = std::stoi(dtStr.substr(0, 4));
            month = std::stoi(dtStr.substr(4, 2));
            day = std::stoi(dtStr.substr(6, 2));
            break;
        case 14: //DateTime
            year = std::stoi(dtStr.substr(0, 4));
            month = std::stoi(dtStr.substr(4, 2));
            day = std::stoi(dtStr.substr(6, 2));
            hour = std::stoi(dtStr.substr(8, 2));
            minute = std::stoi(dtStr.substr(10, 2));
            second = std::stoi(dtStr.substr(12, 2));
            break;
    }

}


/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

std::string d10MessageHeader::getDelimInfoStr()
{
    std::string retStr;
    retStr = sDelimDef.field + sDelimDef.repeat + sDelimDef.component + sDelimDef.escape;
    return retStr;
}

std::string d10MessageHeader::getSenderInfoStr()
{
    std::string retStr;
    retStr = sSenderInfo.sName + sDelimDef.component + sSenderInfo.sInstNo + sDelimDef.component + sSenderInfo.sVersion;
    return retStr;
}

std::string d10MessageHeader::getMessage()
{
    std::string retStr;
    char delim = sDelimDef.field;
    retStr = std::string(&cRecType, &cRecType + 1) + getDelimInfoStr() + delim + sMsgCtrlId + delim + \
        sAccessPassword + delim + getSenderInfoStr() + delim + \
        sSenderStAddr + delim + sReserved + delim + sSendTele + delim + sCharac + delim + sReceiverId + delim + \
        sProcessingId + delim + sVersionNo + delim + sDateTime.getDT() + '\n';
    return(retStr);
}

bool d10MessageHeader::parseMessage(const std::string& msg)
{
    cRecType = msg[0];
    sDelimDef.field = msg[1];
    std::vector<std::string> tokens;
    splitString(msg, tokens, sDelimDef.field);

    if (tokens.size() != 14)
        return false;

    cRecType = tokens[0][0];
    sDelimDef.repeat = tokens[1][0];
    sDelimDef.component = tokens[1][1];
    sDelimDef.escape = tokens[1][2];

    sMsgCtrlId = tokens[2];
    sAccessPassword = tokens[3];

    std::vector<std::string> temp;
    splitString(tokens[4], temp, sDelimDef.component);

    sSenderInfo.sName = temp[0];
    sSenderInfo.sInstNo = temp[1];
    sSenderInfo.sVersion = temp[2];

    sSenderStAddr = tokens[5];
    sReserved = tokens[6];
    sSendTele = tokens[7];
    sCharac = tokens[8];
    sReceiverId = tokens[9];
    sComments = tokens[10];
    sProcessingId = tokens[11];
    sVersionNo = tokens[12];
    sDateTime.parseDate(tokens[13]);

    return true;
}

bool d10MessageHeader::validate()
{
	if (cRecType != 'H')
		return false;
	else if (sDelimDef.field != '|' && sDelimDef.repeat != '\\' && sDelimDef.component != '^' && sDelimDef.escape != '&')
		return false;
	else if (sSenderInfo.sName.empty() || sSenderInfo.sInstNo.empty() || sSenderInfo.sVersion.empty())
		return false;
	else if (sDateTime.getDT().empty())
		return false;
	else
		return true;
}

std::string TestId::toString(char delim)
{
    std::string retStr = t1 + delim + t2 + delim + t3 + delim + t4;
    if(!t5.empty())
        retStr += delim + t5;
    return(retStr);
}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

std::string d10RequestInfoMessage::getRangeIdString(const RangeId& rId)
{
    std::string retStr(rId.sPatientId + sDelimDef.component + rId.sSpecimenId);
    return(retStr);
}

std::string d10RequestInfoMessage::getTestIdString(const TestId& tId)
{
    std::string retStr(tId.t1 + sDelimDef.component + tId.t2 + sDelimDef.component + tId.t3 + sDelimDef.component + tId.t4);
    return(retStr);
}



std::string d10RequestInfoMessage::getMessage()
{
    std::string retStr;
    char delim = sDelimDef.field;
    retStr = std::string(&cRecType, &cRecType + 1) + delim + sSeqNo + delim + \
        getRangeIdString(stRngId) + delim + getRangeIdString(enRngId) + delim + \
        getTestIdString(univTestId) + delim + natureOrReqTimeLimits + delim + begDate.getDate() + delim + endDate.getDate() + delim + reqPhysician + delim + \
        reqPhyTele + delim + userField1 + delim + userField2 + delim + RequestInfoStatCode + '\n';
    return(retStr);
}

bool d10RequestInfoMessage::parseMessage(const std::string& msg)
{

    std::vector<std::string> tokens;
    splitString(msg, tokens, sDelimDef.field);

    if (tokens.size() < 13)
        tokens.insert(tokens.end(), 13 - tokens.size(), "");
        
    try
    {
        cRecType = tokens[0][0];
        sSeqNo = tokens[1];

        stRngId.parse(tokens[2], sDelimDef.component);
        enRngId.parse(tokens[3], sDelimDef.component);
        univTestId.parse(tokens[4], sDelimDef.component);
        natureOrReqTimeLimits = tokens[5];
        begDate.parseDate(tokens[6]);
        endDate.parseDate(tokens[7]);
        reqPhysician = tokens[8];
        reqPhyTele = tokens[9];
        userField1 = tokens[10];
        userField2 = tokens[11];
        RequestInfoStatCode = tokens[12];
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool d10RequestInfoMessage::validate()
{
	if (cRecType != 'O')
		return false;
	else if (sSeqNo.empty())
		return false;
	else if (stRngId.sSpecimenId.empty())
		return false;
	else if (univTestId.t4.empty())
		return false;
	else
		return true;
}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

std::string d10TerminationMessage::getMessage()
{
    std::string retStr;
    const char delim(sDelimDef.field);
    retStr = std::string(&cRecType, &cRecType+1) + delim + sSeqNo + delim + sTermCode + '\n';
    return(retStr);
}

bool d10TerminationMessage::parseMessage(const std::string& msg)
{
    std::vector<std::string> tokens;
    splitString(msg, tokens, sDelimDef.field);

    if (tokens.size() < 3)
        return false;

    cRecType = tokens[0][0];
    sSeqNo = tokens[1];
    sTermCode = tokens[2][0];
    return true;
}

bool d10TerminationMessage::validate()
{
	if (cRecType != 'L')
		return false;
	else if (sSeqNo.empty())
		return false;
	else if (sTermCode != 'N')
		return false;
	else
		return true;
}

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

std::string d10PatientRecordMessage::getMessage()
{
    std::string retStr;
    const char delim(sDelimDef.field);
    retStr = std::string(&cRecType, &cRecType + 1) + delim + \
        sSeqNo + delim + \
        PAPid + delim + \
        LAPid + delim + \
        Pid3 + delim + \
        PatientName + delim + \
        MothersName + delim + \
        Birthdate + delim + \
        Sex + delim + \
        Race + delim + \
        PAddress + delim + \
        PTelephone + delim + \
        PhysId + delim + \
        SPfld1 + delim + \
        SPfld2 + delim + \
        PHeight + delim + \
        PDiagnosis + delim + \
        PMeds + delim + \
        PDiet + delim + \
        PracField1 + delim + \
        PracField2 + delim + \
        AdDisDates + delim + \
        AdStatus + delim + \
        Location + delim + \
        NatureOfAltDiagCode + delim + \
        AltDiagCode + delim + \
        PReligion + delim + \
        MaritalStat + delim + \
        IsolationStatus + delim + \
        Language + delim + \
        HospitalService + delim + \
        HospitalInst + delim + \
        DosageCat + '\n';
    return(retStr);
}

bool d10PatientRecordMessage::parseMessage(const std::string& msg)
{
    std::vector<std::string> tokens;
    splitString(msg, tokens, sDelimDef.field);

    if (tokens.size() < 33)
        tokens.insert(tokens.end(), 33 - tokens.size(), "");
    try
    {
        cRecType = tokens[0][0];
        sSeqNo = tokens[1];
        PAPid = tokens[2];
        LAPid = tokens[3];
        Pid3 = tokens[4];
        PatientName = tokens[5];
        MothersName = tokens[6];
        Birthdate = tokens[7];
        Sex = tokens[8];
        Race = tokens[9];
        PAddress = tokens[10];
        PTelephone = tokens[11];
        PhysId = tokens[12];
        SPfld1 = tokens[13];
        SPfld2 = tokens[14];
        PHeight = tokens[15];
        PDiagnosis = tokens[16];
        PMeds = tokens[17];
        PDiet = tokens[18];
        PracField1 = tokens[19];
        PracField2 = tokens[20];
        AdDisDates = tokens[21];
        AdStatus = tokens[22];
        Location = tokens[23];
        NatureOfAltDiagCode = tokens[24];
        AltDiagCode = tokens[25];
        PReligion = tokens[26];
        MaritalStat = tokens[27];
        IsolationStatus = tokens[28];
        Language = tokens[29];
        HospitalService = tokens[30];
        HospitalInst = tokens[31];
        DosageCat = tokens[32];
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool d10PatientRecordMessage::validate()
{
	if (cRecType != 'P')
		return false;
	else if (sSeqNo.empty())
		return false;
	else
		return true;
}


/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

std::string d10TestOrderRecordMessage::getMessage()
{
    std::string retStr;
    const char delim(sDelimDef.field);
    retStr = std::string(&cRecType, &cRecType + 1) + delim + \
        sSeqNo + delim + \
        SpecimenId + delim + \
        InstSpecId + delim + \
        UnivTestId.toString(sDelimDef.component) + delim + \
        Priority + delim + \
        ODateTime + delim + \
        SpecCollDT + delim + \
        CollEndDT + delim + \
        CollVol + delim + \
        CollectId + delim + \
        AcCode + delim + \
        DangerCode + delim + \
        RelClinInfo + delim + \
        SpecRcvdDT + delim + \
        SpecDesc + delim + \
        SpecType + delim + \
        SpecSource + delim + \
        OrdPhys + delim + \
        PhysTele + delim + \
        userFld1 + delim + \
        userFld2 + delim + \
        labFld1 + delim + \
        labFld2 + delim + \
        LastModDT + delim + \
        InstCharge + delim + \
        InstSectionId + delim + \
        ReportType + delim + \
        Resvd + delim + \
        Loc + delim + \
        NoscFlag + delim + \
        SpecService + delim + \
        SpecimenInst + '\n';
    return(retStr);
}

bool d10TestOrderRecordMessage::parseMessage(const std::string& msg)
{
    std::vector<std::string> tokens;
    splitString(msg, tokens, sDelimDef.field);

    if (tokens.size() < 33)
        tokens.insert(tokens.end(), 33 - tokens.size(), "");
    try
    {
        cRecType = tokens[0][0];
        sSeqNo = tokens[1];        
        SpecimenId = tokens[2];
        InstSpecId = tokens[3];
        UnivTestId.parse(tokens[4],sDelimDef.component);
        Priority = tokens[5];
        ODateTime = tokens[6];
        SpecCollDT = tokens[7];
        CollEndDT = tokens[8];
        CollVol = tokens[9];
        CollectId = tokens[10];
        AcCode = tokens[11];
        DangerCode = tokens[12];
        RelClinInfo = tokens[13];
        SpecRcvdDT = tokens[14];
        SpecDesc = tokens[15];
        SpecType = tokens[16];
        SpecSource = tokens[17];
        OrdPhys = tokens[18];
        PhysTele = tokens[19];
        userFld1 = tokens[20];
        userFld2 = tokens[21];
        labFld1 = tokens[22];
        labFld2 = tokens[23];
        LastModDT = tokens[24];
        InstCharge = tokens[25];
        InstSectionId = tokens[26];
        ReportType = tokens[27];
        Resvd = tokens[28];
        Loc = tokens[29];
        NoscFlag = tokens[30];
        SpecService = tokens[31];
        SpecimenInst = tokens[32];
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool d10TestOrderRecordMessage::validate()
{
	if (cRecType != 'O')
		return false;
	else if (sSeqNo.empty())
		return false;
	else if (SpecimenId.empty())
		return false;
	else if (InstSpecId.empty())
		return false;
	else if (UnivTestId.t4.empty())
		return false;
	else if (ReportType != "F" && ReportType != "Q")
		return false;
	else
		return true;
}


/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------*/

std::string d10ResultRecordMessage::getMessage()
{
    std::string retStr;
    const char delim(sDelimDef.field);
    retStr = std::string(&cRecType, &cRecType + 1) + delim + \
        sSeqNo + delim + \
        UnivTestId.toString(sDelimDef.component) + delim + \
        MeasVal + delim + \
        Units + delim + \
        RefRanges + delim + \
        AbnormalFlag + delim + \
        NatureOfAbnTest + delim + \
        ResultStatus + delim + \
        DTChangeNorm + delim + \
        OpId + delim + \
        DTTestStart.getDT() + delim + \
        DTTestEnd.getDT() + delim + \
        InstrumentId + '\n';
    return(retStr);
}

bool d10ResultRecordMessage::parseMessage(const std::string& msg)
{
    std::vector<std::string> tokens;
    splitString(msg, tokens, sDelimDef.field);

    if (tokens.size() < 14)
        tokens.insert(tokens.end(), 14 - tokens.size(), "");
    try
    {
        cRecType = tokens[0][0];
        sSeqNo = tokens[1];
        UnivTestId.parse(tokens[2],sDelimDef.component);
        MeasVal = tokens[3];
        Units = tokens[4];
        RefRanges = tokens[5];
        AbnormalFlag = tokens[6];
        NatureOfAbnTest = tokens[7];
        ResultStatus = tokens[8];
        DTChangeNorm = tokens[9];
        OpId = tokens[10];
        DTTestStart.parseDate(tokens[11]);
        DTTestEnd.parseDate(tokens[12]);
        InstrumentId = tokens[13];
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool d10ResultRecordMessage::validate()
{
	if (cRecType != 'R')
		return false;
	else if (sSeqNo.empty())
		return false;
	else if (MeasVal.empty())
		return false;
	else if (DTTestEnd.getDT().empty())
		return false;
	else if (UnivTestId.t4.empty())
		return false;
	else
		return true;
}

d10lis::d10lis()
{
    pDb = new sqlite3pp::database("D10.db");
}

d10lis::~d10lis()
{
    delete(pDb);
}

int d10lis::parseMessage (const char* pRawMsg, int size)
{
    std::string sRawMsg(pRawMsg);
    std::string layer2msg = extractLayer2Msg(pRawMsg);
    if(layer2msg.empty() && sRawMsg.find(5) >= 0)
        return 1;
    // Verify Checksum
    //if(!verifyCheckSum(pRawMsg+sRawMsg.find(STX_CHAR),size-sRawMsg.find(STX_CHAR)))
      //  return INVALID_D10_MESSAGE;
    
    // Remove layer one characters. That is first two and last 6
    
    msgType = layer2msg[0];
    switch(msgType)
    {
        case 'H':
            pMsg = new d10MessageHeader();
            break;
        case 'P':
            pMsg = new d10PatientRecordMessage();
            break;
        case 'O':
            pMsg = new d10TestOrderRecordMessage();
            break;
        case 'Q':
            pMsg = new d10RequestInfoMessage();
            break;
        case 'R':
            pMsg = new d10ResultRecordMessage();
            break;
        case 'L':
            pMsg = new d10TerminationMessage();
            break;
        default:
            return INVALID_D10_MESSAGE;
    }

    if(!pMsg->parseMessage(layer2msg))
        return INVALID_D10_MESSAGE;

    if(!pMsg->validate())
        return INVALID_D10_MESSAGE;
    return 1;
}

void d10lis::forgetLastFrame()
{
    delete(pMsg);
    pMsg = NULL;
    msgType = 0;
}

void d10lis::processData()
{
    std::string sProp;
    sqlite3pp::command* pCmd;
    switch(msgType)
    {
        case 'H':
            rxCurrOrder = "";
            break;
        case 'O':
            rxCurrOrder = dynamic_cast<d10TestOrderRecordMessage*>(pMsg)->InstSpecId;
            break;
        case 'R':
            sProp = dynamic_cast<d10ResultRecordMessage*>(pMsg)->UnivTestId.t4 + ' ' + dynamic_cast<d10ResultRecordMessage*>(pMsg)->UnivTestId.t5;
            pCmd = new sqlite3pp::command(*pDb, "INSERT INTO D10Result (orderID,Property,Value) VALUES (?, ?, ?)");
            pCmd->bind(1, rxCurrOrder, sqlite3pp::nocopy);
            pCmd->bind(2,sProp, sqlite3pp::nocopy);
            pCmd->bind(3,dynamic_cast<d10ResultRecordMessage*>(pMsg)->MeasVal, sqlite3pp::nocopy);
            pCmd->execute();
            break;
    }
}


void d10lis::calcCheckSum(const char* buf, int size, unsigned char* high, unsigned char* low)
{
	int checksum = 0;
	int i;
	char strChkSum[10];
	for (i = size - 1; i > 0; --i)
	{
		checksum += (unsigned char)buf[i];
	}

	checksum %= 256;

	sprintf(strChkSum, "%.2X", checksum);
	*high = strChkSum[0];
	*low = strChkSum[1];
}


bool d10lis::verifyCheckSum(const char* msgStr, int size)
{
	// The string is saw. Remove the characters not needed for checksum before passing.
	unsigned char aHighCS, aLowCS, eHighCS, eLowCS;
	aHighCS = msgStr[size - 5];
	aLowCS = msgStr[size - 4];

    char* pMsgSubstr = &msgStr[1];

	calcCheckSum(pMsgSubstr, size-6, &eHighCS, &eLowCS);

	if (eHighCS != aHighCS || eLowCS != aLowCS)
		return false;
	else
		return true;
}

std::string d10lis::wrapLayer2Msg(const std::string& layer2msg)
{
	std::string retStr(" ");
	unsigned char chkH, chkL;
	char buf[5];

	retStr[0] = STX_CHAR;
	retStr += std::to_string(txMsgNum);
	retStr += layer2msg + (char)ETX_CHAR;
	calcCheckSum(retStr.c_str(),retStr.length(), &chkH, &chkL);
	buf[0] = chkH;
	buf[1] = chkL;
	buf[2] = CR_CHAR;
	buf[3] = LF_CHAR;
	buf[4] = 0;

	retStr += buf;
	incrementMsgNum();
	return(retStr);
}

std::string d10lis::extractLayer2Msg(const char* pRawMsg)
{
    std::string layer1msg(pRawMsg);
    int stPos = layer1msg.find(STX_CHAR);
    if (stPos < 0)
        return "";
    int enPos = layer1msg.find(ETX_CHAR) - stPos-3;
	std::string retStr = layer1msg.substr(stPos+2,enPos);
	return(retStr);
}

bool d10lis::verifyMsgIntegrity(const std::string& layer1msg)
{
	if ((unsigned char)layer1msg[0] != STX_CHAR || layer1msg.find((char)ETX_CHAR) == std::string::npos)
		return false;
	return true;	
}