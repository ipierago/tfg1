#pragma once
#include "NetNT.h"

namespace TFG
{

namespace NetNT
{

template <typename TYPENAME>
class ObjectTracker
{
public:
    ObjectTracker();
    ~ObjectTracker();
	void Add(TYPENAME *const);
    void Remove(TYPENAME *const);
    void DestroyContents();
private:
	LPCSTR const m_TypeName;
	std::mutex m_Mutex;
    std::vector<TYPENAME *> m_Objects;
	void DestroyObject(TYPENAME * const);
};

} // namespace NetNT

} // namespace TFG
