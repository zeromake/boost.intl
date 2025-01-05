
#include <boost/locale/facet_id.hpp>
#include <boost/locale/message.hpp>
#include "info.hpp"

namespace boost {
namespace locale {

namespace detail {
template <class Derived> std::locale::id facet_id<Derived>::id;
} // namespace detail
template struct detail::facet_id<info>;
template struct detail::facet_id<message_format<char>>;
template struct detail::facet_id<message_format<wchar_t>>;
} // namespace locale
} // namespace boost
