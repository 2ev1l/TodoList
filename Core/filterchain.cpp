#include "filterchain.h"

FilterChain::FilterChain(std::initializer_list<IFilter *> filters) : m_filters(filters)
{
}
