/**
 * Provides the basic_nullbuf and basic_onullstream needed
 * to create an ostream that will output to nothing.
 * The class is added to the std namespace for compatibility reasons.
 * This class is used by the Logger class.
 *
 * @file src/onullstream.h
 * @author Ryan Lindeman
 * @date 20110524 - Initial Release
 * @date 20120829 - Fix obscure gcc compiler bug when calling basic_ios::init
 *
 * @date 20211018 - Included to lib-DXApp_common project
 */

#ifndef STD_ONULLSTREAM
#define STD_ONULLSTREAM


namespace std
{
	template <class cT, class traits = std::char_traits<cT> >
	class basic_nullbuf : public std::basic_streambuf<cT, traits> {
		typename traits::int_type overflow(typename traits::int_type c)
		{
			return traits::not_eof(c); // indicate success
		}
	};

	template <class cT, class traits = std::char_traits<cT> >
	class basic_onullstream : public std::basic_ostream<cT, traits> {
	public:
		basic_onullstream() :
			std::basic_ios<cT, traits>(&m_sbuf),
			std::basic_ostream<cT, traits>(&m_sbuf)
		{
			this->init(&m_sbuf);
		}

	private:
		basic_nullbuf<cT, traits> m_sbuf;
	};

	typedef basic_onullstream<char> onullstream;
	typedef basic_onullstream<wchar_t> wonullstream;
} // namespace std


#endif // !STD_ONULLSTREAM

/**
 * @class std::onullstream
 * @class std::wonullstream
 * The std::onullstream and std::wonullstream provide a useful way to replace
 * the ostream class with something that will output nothing.
 *
 * Copyright (c) 2010-2011 Ryan Lindeman
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */