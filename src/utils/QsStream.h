
//#pragma once

#include  "QsSdkConfig.h"

//#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <utils/QsAlloc.h>
#include <sstream>
#include <iostream>

typedef std::basic_stringbuf< char, std::char_traits< char >, std::allocator< char > > StringBuf;

namespace QingStor
{
	class  QsStream : public std::iostream
    {
    public:
		using Base = std::iostream;
		QsStream() :
			Base(QingStor::New<StringBuf>())
		{};
		virtual ~QsStream()
		{
			if (rdbuf())
			{
				QingStor::Delete(rdbuf());
			}
		};
    };


	


}
