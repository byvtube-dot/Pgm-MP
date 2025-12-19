#pragma once
#include "SLogBase.h"

#ifdef __cplusplus
extern "C"
{
#endif
	namespace sloglib
	{
		class SMemoryPimpl;
		class SMemory : public SMemoryBase
		{
		private:
			SMemoryPimpl* _pPimpl;

		public:
			BOOL	__stdcall open();
			BOOL	__stdcall close();
			BOOL	__stdcall recv(Data& data, UINT32 milliSeconds);
			BOOL	__stdcall send(Data& data, UINT32 milliSeconds);

		public:
			explicit SMemory(const char* mapName, const char* mmiName = NULL, const char* seqName = NULL);
			virtual ~SMemory();
		};

		class SLogPimpl;
		class SLog : public SLogBase
		{
		private:
			SLogPimpl* _pPimpl;

		public:
			bool	__stdcall logOverFlow();
			Data&	__stdcall logData();
			bool	__stdcall logProcess(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* lotId, const char* recipeId, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
			bool	__stdcall logTransfer(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* materialType, const char* fromDevice, const char* toDevice, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
			bool	__stdcall logFunction(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* materialType, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
			bool	__stdcall logLot(const char* deviceId, const char* eventId, const char* lotId, const char* recipeId, const char* carrierId, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
			bool	__stdcall logConfigure(const char* deviceId, const char* cfgId, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
			bool	__stdcall logAlarm(const char* deviceId, const char* eventId, const char* alarmCode, const char* status, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
			bool	__stdcall logComm(const char* deviceId, const char* eventId, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);

		public:
			explicit SLog(SYSTEMTIME* pSystemTime);
			virtual ~SLog();
		};
	}//namespace
#ifdef __cplusplus
}
#endif  //__cplusplus
