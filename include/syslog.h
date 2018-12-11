#ifndef SYSLOG_H_INCLUDED
#define SYSLOG_H_INCLUDED

#define ERROR_COLOUR 	"\x1b[31m"
#define WARN_COLOUR 	"\x1b[33m"
#define INFO_COLOUR 	"\x1b[37m"
#define DEFAULT_COLOUR 	"\x1b[39m"

#define MAX_RECORD		(SYSLOG_END-SYSLOG_START)+1
#define MAX_MS_LENGTH   60

#define RECORD_BUFFER	30

#define SYSLOGTASK_REFRESH_RATE_MS	50

#include <stdio.h>

#include "ch.h"
#include "hal.h"

	typedef enum {
  		SYSLOG_UNINIT = 				0,                   /**< Not initialized.                           */
  		SYSLOG_STOP = 					1,                   /**< Stopped.                           	     */
  		SYSLOG_ACTIVE = 				2,                    /**< Active.                                    */
  		SYSLOG_FAIL_INIT_MB =			3
	} Syslog_state_t;

	enum MessageType {
		SYSLOG_WARN,
		SYSLOG_INFO,
		SYSLOG_ERROR
	};

	typedef enum{
		NORMAL		 		= 0,
		WITH_SYSTIME 		= 1,
		SYSTIME_NO_COLOUR	= 2
	} PrintType_t;

	typedef enum{
		RECORD_ADDED	 =	0,
		RECORD_NOT_ADDED =	1
	} AddedRecordReturn_t;

	typedef struct{
		time_t RTCtime;
		systime_t SystemTime;
		char ModuleName[12];
		char Message[MAX_MS_LENGTH];
		uint8_t Type;
	} LogRecord_t;

	typedef struct{
	  LogRecord_t RecordBuffer[RECORD_BUFFER];

	  msg_t FreeRecordBufferQueue[RECORD_BUFFER];
	  mailbox_t FreeRecordBuffer;

	  msg_t FilledRecordBufferQueue[RECORD_BUFFER];
	  mailbox_t FilledRecordBuffer;
	} SyslogBufferPark_t;


	Syslog_state_t SyslogInit(BaseSequentialStream* _sd);

	enum NotificationStates{
		ENABLED =  1,
		DISABLED = 0
	};

	__attribute__ ((_message (printf, 1, 2)))
	AddedRecordReturn_t AddRecord(enum MessageType _MStype, const char *_modulename, const char *_message, ...);

	void SetNotifications(enum NotificationStates state);

	void cmd_GetSysLog(BaseSequentialStream *chp, int argc, char *argv[]);

	uint16_t GetTotalRecordCount(void);
	void PrintRecord(uint16_t id, PrintType_t type);

	void SaveSyslog(void);


#define ADD_SYSLOG(D, X, Y, ...) AddRecord(D, X, Y, ##__VA_ARGS__);


#endif