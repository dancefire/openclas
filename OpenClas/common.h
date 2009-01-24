#pragma once
#ifndef _OPENCLAS_COMMON_H_
#define _OPENCLAS_COMMON_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace openclas {
	typedef wchar_t			char_type;
	typedef std::wstring	string_type;
	using boost::shared_ptr;
}

#endif