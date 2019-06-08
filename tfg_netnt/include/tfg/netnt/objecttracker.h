#pragma once
#include "netnt.h"

namespace TFG
{

namespace NetNT
{

template <typename TYPENAME>
class ObjectTracker
{
public:
    ObjectTracker(TFG_Level const in_Level = TFG_Level_Warning);
    ~ObjectTracker();
	void Add(TYPENAME *const);
    void Remove(TYPENAME *const);
    void DestroyContents();
private:
	TFG_Level m_Level;
	LPCSTR const m_TypeName;
	std::mutex m_Mutex;
    std::vector<TYPENAME *> m_Objects;
	void DestroyObject(TYPENAME * const);
};

} // namespace NetNT

} // namespace TFG
