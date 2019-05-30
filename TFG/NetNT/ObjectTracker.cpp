#include "NetNT.h"
#include "ObjectTracker.h"
#include "Packet.h"
#include "UDPSocket.h"

TFG_FILE_SETUP()

#define TFG_NETNT_OBJECTTRACKER_IMPL_1(TYPENAME) \
ObjectTracker<TYPENAME>::ObjectTracker() : m_TypeName(#TYPENAME) {\
\
}\
\
ObjectTracker<TYPENAME>::~ObjectTracker() {\
\
}\
\
void ObjectTracker<TYPENAME>::Add(TYPENAME *const in_ObjectPtr)\
{\
	TFG_FUNC_ENTER();\
	std::unique_lock<std::mutex> _unique_lock(m_Mutex);\
	auto const prit = std::equal_range(m_Objects.begin(), m_Objects.end(), in_ObjectPtr);\
	TFG_ASSERT(prit.first == prit.second);\
	m_Objects.insert(prit.first, in_ObjectPtr);\
}\
\
void ObjectTracker<TYPENAME>::Remove(TYPENAME *const in_ObjectPtr)\
{\
	TFG_FUNC_ENTER();\
	std::unique_lock<std::mutex> _unique_lock(m_Mutex); \
	auto const prit = std::equal_range(m_Objects.begin(), m_Objects.end(), in_ObjectPtr);\
	TFG_ASSERT(std::distance(prit.first, prit.second) == 1);\
	m_Objects.erase(prit.first);\
}\
\
void ObjectTracker<TYPENAME>::DestroyContents()\
{\
	TFG_FUNC_ENTER();\
	std::vector<TYPENAME*> objects;\
	{\
		std::unique_lock<std::mutex> _unique_lock(m_Mutex); \
		objects.insert(objects.begin(), m_Objects.begin(), m_Objects.end());\
	}\
	std::for_each(objects.begin(), objects.end(), [this](TYPENAME* in_ObjectPtr) {\
		TFG_WARNING("%s 0x%0*" PRIxPTR" still alive during shutdown, destroying that object", m_TypeName, PRIxPTR_WIDTH, in_ObjectPtr);\
		DestroyObject(in_ObjectPtr);\
	});\
}

#define TFG_NETNT_OBJECTTRACKER_IMPL_2(TYPENAME) \
template <typename TYPENAME> void ObjectTracker<TYPENAME>::DestroyObject(TYPENAME * const in_ObjectPtr)\
{\
	TFG_FUNC_ENTER();\
	in_ObjectPtr->Destroy();\
}

#define TFG_NETNT_OBJECTTRACKER_IMPL(TYPENAME) \
TFG_NETNT_OBJECTTRACKER_IMPL_1(TYPENAME) \
TFG_NETNT_OBJECTTRACKER_IMPL_2(TYPENAME)


namespace TFG
{

namespace NetNT
{
	
TFG_NETNT_OBJECTTRACKER_IMPL(UDPSocket);


} // namespace NetNT

} // namespace TFG
