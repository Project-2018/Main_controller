#include "syslog.h"
#include "chprintf.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "rtc.h"
#include <stdlib.h>

#include "memorymap.h"
#include "storageconf.h"

#include "eeprom.h"


static LogRecord_t Records[MAX_RECORD];

SyslogBufferPark_t SyslogBuffers;

BaseSequentialStream* sd;

Syslog_state_t SysLogState = SYSLOG_UNINIT;

uint8_t Notifications = DISABLED;

uint16_t LastRecordPointer = 0;

static uint16_t NextRecordPtr = 0;


static THD_WORKING_AREA(syslogtask_wa, 1024);
static THD_FUNCTION(syslogtask, p) {

  (void)p;
  chRegSetThreadName("SyslogTask");
  void *pbuf;
  while (TRUE) {

  	msg_t msg = chMBFetchTimeout(&SyslogBuffers.FilledRecordBuffer, (msg_t *)&pbuf, TIME_MS2I(SYSLOGTASK_REFRESH_RATE_MS));
  	if(msg == MSG_OK){

  		memcpy(&Records[NextRecordPtr], pbuf, sizeof(LogRecord_t));

		if(Notifications == ENABLED)
			PrintRecord(NextRecordPtr, WITH_SYSTIME);

		NextRecordPtr++;
		if(NextRecordPtr >= MAX_RECORD){
			NextRecordPtr = 0;
		}

		(void)chMBPostTimeout(&SyslogBuffers.FreeRecordBuffer, (msg_t)pbuf, TIME_INFINITE);
  	}

  	if(Notifications == DISABLED)
  		SaveSyslog();

  }
}

Syslog_state_t SyslogInit(BaseSequentialStream* _sd){
	if(_sd == NULL){
		SysLogState = SYSLOG_STOP;
		return SYSLOG_STOP;
	}
	sd = _sd;

	chprintf((BaseSequentialStream*)sd, "\x1B\x63");
  	chprintf((BaseSequentialStream*)sd, "\x1B[2J\x1B[?25h");

  	AssignToMemoryMap(SYSLOG_POINTER, (uint8_t*)&NextRecordPtr, sizeof(uint16_t));

  	uint8_t syslogptrres = ReadRecordFromEeprom(SYSLOG_POINTER);

  	if(syslogptrres != STORAGELIB_OK){
  		NextRecordPtr = 0;
  	}

  	LastRecordPointer = NextRecordPtr;

  	uint16_t i;
  	for(i = 0; i < MAX_RECORD; i++){
  		AssignToMemoryMap(SYSLOG_START + i, (uint8_t*)&(Records[i]), sizeof(LogRecord_t));
  	}

  	chMBObjectInit(&SyslogBuffers.FreeRecordBuffer, SyslogBuffers.FreeRecordBufferQueue, RECORD_BUFFER);

  	chMBObjectInit(&SyslogBuffers.FilledRecordBuffer, SyslogBuffers.FilledRecordBufferQueue, RECORD_BUFFER);

  	
  	for(i = 0; i < RECORD_BUFFER; i++){
  		if(chMBPostTimeout(&SyslogBuffers.FreeRecordBuffer, (msg_t)&SyslogBuffers.RecordBuffer[i], TIME_INFINITE) != MSG_OK){
  			SysLogState = SYSLOG_FAIL_INIT_MB;
  			return SysLogState;
  		}
  	}

  	chThdCreateStatic(syslogtask_wa, sizeof(syslogtask_wa), NORMALPRIO, syslogtask, NULL);

	SysLogState = SYSLOG_ACTIVE;
	return SYSLOG_ACTIVE;
}



void PrintRecord(uint16_t id, PrintType_t type){


	if(type != SYSTIME_NO_COLOUR){
		switch(Records[id].Type)
		{
			case SYSLOG_WARN:
				chprintf((BaseSequentialStream*)sd, WARN_COLOUR);
			break;
			case SYSLOG_ERROR:
				chprintf((BaseSequentialStream*)sd, ERROR_COLOUR);
			break;
			case SYSLOG_INFO:
				chprintf((BaseSequentialStream*)sd, INFO_COLOUR);
			break;
		}
	}

	uint32_t sec = TIME_I2S(Records[id].SystemTime == 0 ? 1 : Records[id].SystemTime) - 1;
	uint32_t ms =  TIME_I2MS(Records[id].SystemTime) % 1000;
	struct tm tim = {0};
	localtime_r((time_t*)&Records[id].RTCtime, &tim);

	switch(type){
		case NORMAL:
			chprintf((BaseSequentialStream*)sd, "[ %04d%02d%02d.%02d%02d%02d ]", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);
		break;
		case WITH_SYSTIME:
			chprintf((BaseSequentialStream*)sd, "[ %04d%02d%02d.%02d%02d%02d ][ %06d.%03d ]", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec, sec, ms);
		break;
		case SYSTIME_NO_COLOUR:
			chprintf((BaseSequentialStream*)sd, "[ %04d%02d%02d.%02d%02d%02d ][ %06d.%03d ]", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec, sec, ms);
		break;
	}

	switch(Records[id].Type)
	{
		case SYSLOG_WARN:
			chprintf((BaseSequentialStream*)sd, "[WARN]");
		break;
		case SYSLOG_ERROR:
			chprintf((BaseSequentialStream*)sd, "[ERROR]");
		break;
		case SYSLOG_INFO:
			chprintf((BaseSequentialStream*)sd, "[INFO]");
		break;
	}
	chprintf((BaseSequentialStream*)sd, " %s: %s\r\n", Records[id].ModuleName, Records[id].Message);

	if(type != SYSTIME_NO_COLOUR)
		chprintf((BaseSequentialStream*)sd, DEFAULT_COLOUR);
}

AddedRecordReturn_t AddRecord(enum MessageType _MStype, const char *_modulename, const char *_message, ...){
	if (NextRecordPtr >= MAX_RECORD || SysLogState != SYSLOG_ACTIVE)
		return RECORD_NOT_ADDED;

	void *pbuf;
	msg_t ReturnValue = chMBFetchTimeout(&SyslogBuffers.FreeRecordBuffer, (msg_t *)&pbuf, TIME_IMMEDIATE);
	if(ReturnValue != MSG_OK)
		return RECORD_NOT_ADDED;

	LogRecord_t NewRecord;

	va_list vl;
    va_start(vl, _message);
	
	vsnprintf(&NewRecord.Message[0], MAX_MS_LENGTH, _message, vl);

	NewRecord.RTCtime = GetTimeUnixSec();
	NewRecord.SystemTime = chVTGetSystemTime();
	strcpy(&NewRecord.ModuleName[0], _modulename);

	NewRecord.Type = _MStype;

	memcpy(pbuf, &NewRecord, sizeof(LogRecord_t));

	(void)chMBPostTimeout(&SyslogBuffers.FilledRecordBuffer, (msg_t)pbuf, TIME_INFINITE);

	return RECORD_ADDED;
}

void SetNotifications(enum NotificationStates state){
	Notifications = state;
}

uint16_t GetTotalRecordCount(void){
	return NextRecordPtr;
}

void SaveSyslog(void){
	if(NextRecordPtr == LastRecordPointer){
		return;
	}

	while(LastRecordPointer != NextRecordPtr){
		uint8_t res = StoreRecordToEeprom(SYSLOG_START + LastRecordPointer);
		if(res != STORAGELIB_OK)
			return;

		LastRecordPointer++;
		if(LastRecordPointer >= MAX_RECORD)
			LastRecordPointer = 0;
	}

	LastRecordPointer = NextRecordPtr;
	StoreRecordToEeprom(SYSLOG_POINTER);
}

void FormatSyslog(BaseSequentialStream *chp){
	uint16_t i;
	for(i = 0; i < MAX_RECORD; i++){
		Records[i].RTCtime = 0;
		Records[i].SystemTime = 0;
		sprintf(&Records[i].ModuleName[0], " ");
		sprintf(&Records[i].Message[0], " ");
		Records[i].Type = SYSLOG_INFO;
	}
	NextRecordPtr = 0;
	uint8_t res = 0;
	uint8_t TimoutCnt = 0;
	while(true){
		chprintf(chp, "Try reach memory... %d\r\n", TimoutCnt);
		res = StoreBlockToEeprom(SYSLOG_POINTER, SYSLOG_END);
		if(res != STORAGELIB_MEMORY_IN_USE)
			break;
		TimoutCnt++;
		if(TimoutCnt > 50){
			chprintf(chp, "Memory is used by another task...\r\n");
			break;
		}
	}
	if(res == STORAGELIB_OK){
		ADD_SYSLOG(SYSLOG_WARN, "StorageLib", "System log formatted");
	}
}

void cmd_GetSysLog(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argc;
  (void)argv;

  chprintf(chp, "\x1B\x63");
  chprintf(chp, "\x1B[2J");

  if(argc == 0){
	uint16_t i;
	for( i = 0; i < NextRecordPtr; i++)
		PrintRecord(i, WITH_SYSTIME);

  }

  if(argc == 1){
  	uint8_t i = atoi(argv[0]);
  	if(i == 0){
  		chprintf(chp, "Loading syslog...\r\n");
  		ReadBlockFromEeprom(SYSLOG_START, SYSLOG_END);
  		chprintf(chp, "\x1B\x63");
  		chprintf(chp, "\x1B[2J");
  		chprintf(chp, "Uptime in minutes: %d", *GetUptimeMin());
  		uint16_t a;
  		for(a = 0; a < MAX_RECORD; a++){
  			PrintRecord(a, SYSTIME_NO_COLOUR);
  		}
  	}
  	if(i == 1){
  		FormatSyslog(chp);
  		chprintf(chp, "Syslog formatted!\r\n");
  	}
  	if(i == 2){
  		chprintf(chp, "NextRecordPtr: %d\r\n", NextRecordPtr);
  		chprintf(chp, "LastRecordPointer: %d\r\n", LastRecordPointer);
  	}
  }
}
