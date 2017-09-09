#ifndef _D10LIS_H_
#define _D10LIS_H_

#include <string>
#include <vector>
#include "sqlite3pp.h"

#define HEADER_MSG 1
#define TERMINATION_MSG 2
#define PATIENT_MSG 3
#define ORDER_MSG 4
#define RESULT_MSG 5
#define REQUEST_MSG 6
#define ENQ_MSG 7

/* SUPPORTING STRUCTURES */
/* --------------------- */

struct delimDef
{
    char field = '|';
    char repeat = '\\';
    char component = '^';
    char escape = '&';
};

struct senderInfo
{
    std::string sName;
    std::string sInstNo;
    std::string sVersion;
};

struct datetime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    std::string getDate();
    std::string getTime();
    std::string getDT();
    void parseDate(const std::string& dtStr);
};

struct RangeId
{
    std::string sPatientId;
    std::string sSpecimenId;
    void parse(const std::string&, char delim);
};

struct TestId
{
    std::string t1;
    std::string t2;
    std::string t3;
    std::string t4;
    std::string t5;
    void parse(const std::string&, char delim);
    std::string toString(char delim);
};

/* D10MESSAGE
 * Base class for all messages. Takes care of emitting propper message string
 */
class d10Message
{
public:    
    char                cRecType;
    delimDef            sDelimDef;   
public:
    virtual bool parseMessage(const std::string&) = 0;
    virtual std::string getMessage() = 0;
    virtual bool validate() = 0;
    virtual int getType() = 0;
};

class d10MessageHeader : public d10Message
{
public:
    std::string         sMsgCtrlId;
    std::string         sAccessPassword;
    senderInfo          sSenderInfo;
    std::string         sSenderStAddr;
    std::string         sReserved;
    std::string         sSendTele;
    std::string         sCharac;
    std::string         sReceiverId;
    std::string         sComments;
    std::string         sProcessingId;
    std::string         sVersionNo;
    datetime            sDateTime;

public:
    std::string getDelimInfoStr();
    std::string getSenderInfoStr();
    std::string getMessage();
    bool parseMessage(const std::string& msg);
    bool validate();
    int getType() { return HEADER_MSG; }
};


class d10TerminationMessage : public d10Message
{
public:
    std::string         sSeqNo;
    char                sTermCode;
public:
    std::string getMessage();
    bool parseMessage(const std::string& msg);
    bool validate();
    int getType() { return TERMINATION_MSG; }
};


class d10RequestInfoMessage : public d10Message
{
public:
    std::string         sSeqNo;
    RangeId             stRngId;
    RangeId             enRngId;
    TestId              univTestId;
    std::string         natureOrReqTimeLimits;
    datetime            begDate;
    datetime            endDate;
    std::string         reqPhysician;
    std::string         reqPhyTele;
    std::string         userField1;
    std::string         userField2;
    std::string         RequestInfoStatCode;
public:
    std::string getRangeIdString(const RangeId&);
    std::string getTestIdString(const TestId& tId);
    std::string getMessage();
    bool parseMessage(const std::string& msg);
    bool validate();
    int getType() { return REQUEST_MSG; }
};

class d10PatientRecordMessage : public d10Message
{
public:
    std::string         sSeqNo;
    std::string         PAPid;
    std::string         LAPid;
    std::string         Pid3;
    std::string         PatientName;
    std::string         MothersName;
    std::string         Birthdate;
    std::string         Sex;
    std::string         Race;
    std::string         PAddress;
    std::string         PTelephone;
    std::string         PhysId;
    std::string         SPfld1;
    std::string         SPfld2;
    std::string         PHeight;
    std::string         PDiagnosis;
    std::string         PMeds;
    std::string         PDiet;
    std::string         PracField1;
    std::string         PracField2;
    std::string         AdDisDates;
    std::string         AdStatus;
    std::string         Location;
    std::string         NatureOfAltDiagCode;
    std::string         AltDiagCode;
    std::string         PReligion;
    std::string         MaritalStat;
    std::string         IsolationStatus;
    std::string         Language;
    std::string         HospitalService;
    std::string         HospitalInst;
    std::string         DosageCat;
    
public:
    std::string getMessage();
    bool parseMessage(const std::string& msg);
    bool validate();
    int getType() { return PATIENT_MSG; }
};


class d10TestOrderRecordMessage : public d10Message
{
public:
    std::string         sSeqNo;
    std::string         SpecimenId;
    std::string         InstSpecId;
    TestId              UnivTestId;
    std::string         Priority;
    std::string         ODateTime;
    std::string         SpecCollDT;
    std::string         CollEndDT;
    std::string         CollVol;
    std::string         CollectId;
    std::string         AcCode;
    std::string         DangerCode;
    std::string         RelClinInfo;
    std::string         SpecRcvdDT;
    std::string         SpecDesc;
    std::string         SpecType;
    std::string         SpecSource;
    std::string         OrdPhys;
    std::string         PhysTele;
    std::string         userFld1;
    std::string         userFld2;
    std::string         labFld1;
    std::string         labFld2;
    std::string         LastModDT;
    std::string         InstCharge;
    std::string         InstSectionId;
    std::string         ReportType;
    std::string         Resvd;
    std::string         Loc;
    std::string         NoscFlag;
    std::string         SpecService;
    std::string         SpecimenInst;
public:
    std::string getMessage();
    bool parseMessage(const std::string& msg);
    bool validate();
    int getType() { return ORDER_MSG; }
};

class d10ResultRecordMessage : public d10Message
{
public:
    std::string         sSeqNo;
    TestId              UnivTestId;
    std::string         MeasVal;
    std::string         Units;
    std::string         RefRanges;
    std::string         AbnormalFlag;
    std::string         NatureOfAbnTest;
    std::string         ResultStatus;
    std::string         DTChangeNorm;
    std::string         OpId;
    datetime            DTTestStart;
    datetime            DTTestEnd;
    std::string         InstrumentId;    
public:
    std::string getMessage();
    bool parseMessage(const std::string& msg);
    bool validate();
    int getType() { return RESULT_MSG; }
};

class d10EnqMessage : public d10Message
{
public:
    std::string getMessage(){ return ""; }
    bool parseMessage(const std::string& msg){}
    bool validate(){return true;}
    int getType() { return ENQ_MSG; }
};

#define INVALID_D10_MESSAGE 0
#define EOT_CHAR  4
#define ETX_CHAR  3
#define STX_CHAR  2
#define ACK_CHAR  6
#define NAK_CHAR  21
#define CR_CHAR   10
#define LF_CHAR   13

class d10lis
{
private:
    std::string rxCurrOrder;
    sqlite3pp::database* pDb;
    unsigned short txMsgNum;
public:
    d10lis();
    ~d10lis();

    d10Message* parseMessage (const std::string&, bool&);
    void forgetLastFrame();
    void processData(d10Message* pMsg);

    void calcCheckSum(const char* buf, int size, unsigned char* high, unsigned char* low);
    bool verifyCheckSum(const char* msgStr, int size);

    std::string wrapLayer2Msg(const std::string& layer2msg);
    std::string extractLayer2Msg(const std::string& layer1msg);

    bool verifyMsgIntegrity(const std::string& layer1msg);

    void resetMsgNum() { txMsgNum = 1; }
    void incrementMsgNum() { txMsgNum = (txMsgNum == 7) ? 0 : ++txMsgNum; }
    };




#endif //_D10LIS_H_
