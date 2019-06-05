#include "stdafx.h"
#include "TFG_log.h"

namespace TFG
{

	namespace Log
	{


		namespace {
			std::string TruncateFileName(const char * const fileNamePtr) {

				size_t const s = strlen(fileNamePtr);
				size_t const TRUNC_SIZE = 16;
				size_t numPaddingSpaces = 0;
				size_t offsetTruncatedFile = 0;
				if (s <= TRUNC_SIZE) {
					numPaddingSpaces = TRUNC_SIZE - s;
					offsetTruncatedFile = 0;
				}
				else {
					numPaddingSpaces = 0;
					size_t const b1 = (s % 16);
					size_t const b2 = s - b1;
					size_t const b3 = b2 - 16;
					offsetTruncatedFile = b1 + b3;
				}
				const char * const fileNameEndPtr = fileNamePtr + offsetTruncatedFile;
				std::string rv = std::string(fileNameEndPtr);
				if (numPaddingSpaces > 0) {
					rv.insert(0, numPaddingSpaces, ' ');
				}
#if 0
				if (s > TRUNC_SIZE) {
					rv[0] = rv[1] = rv[2] = '.';
				}
#endif
				return rv;
			}
		}

		PerFileData::PerFileData(PerModuleData & perModuleData, const char *const fileNamePtr) : m_PerModuleData(perModuleData), m_FileName(fileNamePtr), m_TruncatedFileName(TruncateFileName(fileNamePtr)), m_LocalLevel(Level_Always)
		{
			perModuleData.OnPerFileData(*this);
			UpdateLevel();
		}

		void PerFileData::UpdateLevel()
		{
			m_CurrentLevel = std::max(m_LocalLevel, m_PerModuleData.m_CurrentLevel);
		}

		PerModuleData::PerModuleData(Globals & globals, const char * const namePtr) : m_Globals(globals), m_LocalLevel(Level_Always), m_NamePtr(namePtr) { 
			m_Globals.OnPerModuleData(*this);
			UpdateLevel(); 
		}

		void PerModuleData::OnPerFileData(PerFileData & perFileData) {
			std::unique_lock<std::mutex> _unique_lock(m_Mutex);
			m_PerFileDataArray.push_back(&perFileData);
		}

		void PerModuleData::UpdateLevel()
		{
			m_CurrentLevel = std::max(m_LocalLevel, m_Globals.GetCurrentLevel());
			std::unique_lock<std::mutex> _unique_lock(m_Mutex);
			std::for_each(m_PerFileDataArray.begin(), m_PerFileDataArray.end(), [](PerFileData * perFileDataPtr) {
				perFileDataPtr->UpdateLevel();
			});
		}

		Globals & Globals::Get()
		{
			static Globals s_Globals;
			return s_Globals;
		}

		Globals::Globals()
		{
#ifdef _DEBUG
			SetCurrentLevel(Level_Debug);
#else
			SetCurrentLevel(Level_Warning);
#endif
		}

		void Globals::OnPerModuleData(PerModuleData & perModuleData) {
			std::unique_lock<std::mutex> _unique_lock(mutex);
			m_PerModuleDataArray.push_back(&perModuleData);
		}

		void Init()
		{
			Globals &globals = Globals::Get();
		}

		void Deinit()
		{
		}

		void SetGlobalLevel(Level const level) {
			Globals & globals = Globals::Get();
			globals.SetCurrentLevel(level);
		}

		void Globals::SetCurrentLevel(Level const in_level)
		{
			m_CurrentLevel = in_level;
			std::unique_lock<std::mutex> _unique_lock(mutex);
			std::for_each(m_PerModuleDataArray.begin(), m_PerModuleDataArray.end(), [](PerModuleData * perModuleDataPtr) {
				perModuleDataPtr->UpdateLevel();
			});
		}

#ifdef ANDROID
		static android_LogPriority ConvertToAndroidLogPriority(Level const in_level)
		{
			if (in_level == Level_Always)
				return ANDROID_LOG_UNKNOWN;
			if (in_level == Level_Severe)
				return ANDROID_LOG_FATAL;
			if (in_level == Level_Error)
				return ANDROID_LOG_ERROR;
			if (in_level == Level_Warning)
				return ANDROID_LOG_WARN;
			if (in_level == Level_Notify)
				return ANDROID_LOG_INFO;
			if (in_level == Level_Info)
				return ANDROID_LOG_INFO;
			if (in_level == Level_Debug)
				return ANDROID_LOG_DEBUG;
			if (in_level == Level_Debug2)
				return ANDROID_LOG_DEBUG;
			if (in_level == Level_Trace)
				return ANDROID_LOG_VERBOSE;
			if (in_level == Level_Trace2)
				return ANDROID_LOG_VERBOSE;
			if (in_level == Level_Never)
				return ANDROID_LOG_SILENT;
			return ANDROID_LOG_UNKNOWN;
		}
#endif
		static void _log_str(const char *const in_)
		{
#if defined(ANDROID)
			TFG_ThreadGlobals *const TFG_ThreadGlobals_p = TFG_ThreadGlobals_Get();
			android_LogPriority const logPriority = ConvertToAndroidLogPriority(TFG_ThreadGlobals_p->std_level);
			__android_log_write(logPriority, TFG_ThreadGlobals_p->function_psz, in_);
#elif defined(WIN32)
			OutputDebugStringA(in_);
			//#elif defined(__APPLE__)
#endif
			printf("%s", in_);
		}

		inline int32_t TFG_vsprintf_s(char *const in_buffer, size_t const in_buffer_count, const char *const format, va_list argList)
		{
#if defined(_MSC_VER)
			return vsprintf_s(in_buffer, in_buffer_count, format, argList);
#elif defined(__GNUC__)
			return vsprintf(in_buffer, format, argList);
#endif
		}

		inline int32_t TFG_sprintf_s(char *const in_buffer, size_t const in_buffer_count, const char *const format, ...)
		{
			va_list argList;
			va_start(argList, format);
			int32_t const rv = TFG_vsprintf_s(in_buffer, in_buffer_count, format, argList);
			va_end(argList);
			return rv;
		}

		inline int TFG_strcat_s(char *dest, size_t destsz, const char *src)
		{
#if defined(_MSC_VER)
			return strcat_s(dest, destsz, src);
#elif defined(__GNUC__)
			strcat(dest, src);
			return 0;
#endif
		}

		inline uint32_t TFG_GetCurrentThreadId()
		{
#if defined(_WIN32)
			return GetCurrentThreadId();
#elif defined(ANDROID)
			return gettid();
#else
			mach_port_t machTID = pthread_mach_thread_np(pthread_self());
			return machTID;
#endif
		}

		const char *LevelToHeaderString(Level const in_level)
		{
			switch (in_level)
			{
			case Level_Always:
				return "always ";
			case Level_Severe:
				return "severe ";
			case Level_Error:
				return "error  ";
			case Level_Warning:
				return "warning";
			case Level_Notify:
				return "notify ";
			case Level_Info:
				return "info   ";
			case Level_Debug:
				return "debug  ";
			case Level_Debug2:
				return "debug_2";
			case Level_Trace:
				return "trace  ";
			case Level_Trace2:
				return "trace_2";
			default:
				return "unknown";
			}
		}
		
		int32_t sprintf_header(Level const level, const char *const functionNamePtr, const int32_t lineNumber, TFG::Log::PerFileData & perFileData, char *const in_buffer, size_t const in_buffer_count)
		{
			uint32_t this_thread_id = TFG_GetCurrentThreadId();
			
			int32_t const rv = TFG_sprintf_s(in_buffer, in_buffer_count, "T0x%08lx (%s) \"%s\":%-3d:%s: ", this_thread_id, LevelToHeaderString(level), perFileData.GetTruncatedFileName().c_str(), lineNumber, functionNamePtr);
			return rv;
		}

		void PrintF_vargs(Level const level, const char *const functionNamePtr, const int32_t lineNumber, TFG::Log::PerFileData & perFileData, const char *const format, va_list argList)
		{
			char buf[1024];
			int32_t const headerLen = sprintf_header(level, functionNamePtr, lineNumber, perFileData, buf, sizeof(buf));
			TFG_vsprintf_s(buf + headerLen, sizeof(buf) - headerLen, format, argList);
			TFG_strcat_s(buf, sizeof(buf), "\n");
			_log_str(buf);
		}

		void PrintF(Level const level, const char *const functionNamePtr, const int32_t lineNumber, TFG::Log::PerFileData & perFileData, const char *const format, ...)
		{
			va_list argList;
			va_start(argList, format);
			PrintF_vargs(level, functionNamePtr, lineNumber, perFileData, format, argList);
			va_end(argList);
		}
	}
}
