#pragma once
#include <tfg/tfg.h>

namespace TFG
{
	namespace Log
	{
		enum Level
		{
			Level_Always,

			Level_Severe,
			Level_Error,
			Level_Warning,

			Level_Notify,
			Level_Info,

			Level_Debug,
			Level_Debug2,
			Level_Trace,
			Level_Trace2,

			Level_Never
		};


		class PerModuleData;
		class Globals;
		class PerModuleData;

		class Globals
		{
		public:
			static Globals & Get();
			void SetCurrentLevel(Level const);
			Level GetCurrentLevel() const { return m_CurrentLevel; }
			void OnPerModuleData(PerModuleData & perModuleData);
		private:
			Globals();
			std::mutex mutex;
			std::vector<PerModuleData *> m_PerModuleDataArray;
			Level m_CurrentLevel;
		};

		class PerFileData
		{
			std::string const m_FileName;
			std::string const m_TruncatedFileName;
		public:
			PerFileData(PerModuleData & perModuleData, const char *const fileNamePtr);
			void UpdateLevel();
			PerModuleData & m_PerModuleData;
			Level m_CurrentLevel;
			Level m_LocalLevel;
			std::string const & GetFileName() const { return m_FileName; }
			std::string const & GetTruncatedFileName() const { return m_TruncatedFileName; }
		};

		class PerModuleData
		{
		public:
			PerModuleData(Globals & globals, const char * const);
			void OnPerFileData(PerFileData & perFileData);
			void UpdateLevel();
			Globals & m_Globals;
			std::mutex m_Mutex;
			std::vector<PerFileData *> m_PerFileDataArray;
			Level m_LocalLevel;
			Level m_CurrentLevel;
			const char * const m_NamePtr;
		};


		// Get/Set minimum values across files
		void SetGlobalLevel(Level const level);

		void PrintF(Level const level, const char *const functionNamePtr, const int32_t lineNumber, TFG::Log::PerFileData &, const char *const, ...);
	}

} // namespace TFG

#define TFG_LOG_FILE_SETUP() \
	TFG::Log::PerFileData & GetTFGLogPerFileData() { static TFG::Log::PerFileData s_tfgLogPerFileData(GetTFGLogPerModuleData(), __FILE__); return s_tfgLogPerFileData;}

#define TFG_LOG_CHECK_LEVEL(_level) (GetTFGLogPerFileData().m_CurrentLevel >= _level)

#define TFG_LOG_PRINTF(LOG_LEVEL, ...) do {if (TFG_LOG_CHECK_LEVEL(LOG_LEVEL)) TFG::Log::PrintF(LOG_LEVEL, __FUNCTION__, __LINE__, GetTFGLogPerFileData(), __VA_ARGS__); } while (0);
#define TFG_LOG_PRINTF_DEBUG(...) TFG_LOG_PRINTF(TFG::Log::Level_Debug, __VA_ARGS__)
#define TFG_LOG_PRINTF_TRACE(...) TFG_LOG_PRINTF(TFG::Log::Level_Trace, __VA_ARGS__)

#define TFG_LOG_MODULE_SETUP(MODULE_NAME) \
	TFG::Log::PerModuleData & GetTFGLogPerModuleData() { static TFG::Log::PerModuleData s_Instance(TFG::Log::Globals::Get(), MODULE_NAME); return s_Instance;}

#define TFG_LOG_FUNCTION()
