/*****************************************************************************
* Author	: neiver, http://we.easyelectronics.ru/blog/Soft/2400.html
* Date		: 2012
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/
#ifndef __CONV_H_
#define __CONV_H_


#pragma once

struct Ret{
	char *ptr = nullptr;
	std::uint32_t cnt{0};
};

constexpr std::uint32_t table2[] =
{
    0xF0BDC21A,
    0x3DA137D5,
    0x9DC5ADA8,
    0x2863C1F5,
    0x6765C793,
    0x1A784379,
    0x43C33C19,
    0xAD78EBC5,
    0x2C68AF0B,
    0x71AFD498,
    0x1D1A94A2,
    0x4A817C80,
    0xBEBC2000,
    0x30D40000,
    0x7D000000,
    0x20000000,
    0x51EB851E,
    0xD1B71758,
    0x35AFE535,
    0x89705F41,
    0x232F3302,
    0x5A126E1A,
    0xE69594BE,
    0x3B07929F,
    0x971DA050,
    0x26AF8533,
    0x63090312,
    0xFD87B5F2,
    0x40E75996,
    0xA6274BBD,
    0x2A890926,
    0x6CE3EE76
};


template<class T>
struct divmod10_t
{
    T quot{0};
    std::uint8_t rem{0};
};

class NumConvert{
public:

	template<typename Num>
	static Ret to_string_integer (const Num value, char* buffer){

		char *out_ptr = buffer;

		*out_ptr = 0;

		divmod10_t<Num> res;

		res.quot = value;

		// convert to char's array
		do{
			res = divmodu10(res.quot);
			*--out_ptr = res.rem + '0';

		}while (res.quot);

		return {out_ptr, strlen(out_ptr)};
	}

	static Ret to_string_float(const float value, char* result)
	{
		char *in_ptr = result;
		char *out_ptr = in_ptr;

		std::uint8_t precision = 6;
		const int bufferSize = 10;
		char buffer[bufferSize+1];

		int exp10 = ftoaEngine(value, buffer, precision);

		if(exp10 == 0xff)
		{
			std::uint32_t digits = strlen(buffer);
			std::uint32_t i = 0;
			if(buffer[0] == '+')
				i = 1;

			for(i = 0; i < digits; i++)
				*out_ptr++ = buffer[i];
			*out_ptr = 0;
			return {result, strlen(in_ptr)};
		}

		char *str_begin = &buffer[2];
		if(buffer[1] != '0')
		{
			exp10++;
			str_begin--;
		}

		std::uint_fast8_t digits = (std::uint_fast8_t)strlen(str_begin);

		std::uint_fast8_t intDigits=0, leadingZeros = 0;
		if(abs(exp10) >= precision)
		{
			intDigits = 1;
		}else if(exp10 >= 0)
		{
			intDigits = exp10+1;
			exp10 = 0;
		}else
		{
			intDigits = 0;
			leadingZeros = -exp10 - 1;
			exp10 = 0;
		}
		std::uint_fast8_t fractDigits = digits > intDigits ? digits - intDigits : 0;
		//FieldFill(fractPartSize + intPartSize, IOS::right);
		if(intDigits)
		{
			std::uint_fast8_t count = intDigits > digits ? digits : intDigits;
			while(count--)
				*out_ptr++ = *str_begin++;
			std::int_fast8_t tralingZeros = intDigits - digits;
			while(tralingZeros-- > 0)
				*out_ptr++ ='0';
		}
		else
			*out_ptr++ = '0';

		if(fractDigits)
		{
			*out_ptr++ = '.';
			while(leadingZeros--)
				*out_ptr++ = '0';
			while(fractDigits--)
				*out_ptr++ = *str_begin++;
		}

		if(exp10 != 0)
		{
			*out_ptr++ = 'e';
			std::uint32_t upow10;
			if(exp10 < 0)
			{
				*out_ptr++ = '-';
				upow10 = -exp10;
			}
			else
			{
				*out_ptr++ = '+';
				upow10 = exp10;
			}

			char* powPtr = utoa_fast_div(upow10, buffer + bufferSize);

			while(powPtr < buffer + bufferSize)
			{
				*out_ptr++ = *powPtr++;
			}
		}
		*out_ptr = 0;

		return {result, strlen(in_ptr)};
	}

private:
	template<typename N>
	static divmod10_t<N> divmodu10(const N& n){
		divmod10_t<N> res;
		res.quot = n >> 1;
		res.quot += res.quot >> 1;
		res.quot += res.quot >> 4;
		if(sizeof(N) > 1)
			res.quot += res.quot >> 8;
		if(sizeof(N) > 2)
			res.quot += res.quot >> 16;
		N qq = res.quot;
		res.quot >>= 3;
		res.rem = std::uint8_t(n - ((res.quot << 1) + (qq & ~std::uint32_t{7})));
		if(res.rem > 9)
		{
			res.rem -= 10;
			res.quot++;
		}
		return res;
	}

	static std::uint32_t MantissaMul(std::uint32_t mantissa, std::uint32_t multiplier)
	{
	    mantissa <<= 8;
		return (std::uint64_t(mantissa) * multiplier) >> 32;
	}

	static int ftoaEngine(float value, char *buffer, int presc)
	{
//		std::uint32_t uvalue = *reinterpret_cast<std::uint32_t*>(&value);
		std::uint32_t uvalue = *bit_cast<std::uint32_t*>(&value);
		std::uint8_t exponent = static_cast<std::uint8_t>(uvalue >> 23);
		std::uint32_t fraction = (uvalue & 0x00ffffff) | 0x00800000;
		char *ptr = buffer;

		if(uvalue & 0x80000000){
	        *ptr++ = '-';
		} else {
	        *ptr++ = '+';
		}

		if(exponent == 0) // don't care about a subnormals
		{
			ptr[0] = '0';
			ptr[1] = 0;
			return 0xff;
		}

		if(exponent == 0xff)
		{
		    if(fraction & 0x007fffff)
		    {
		    	ptr[0] = 'n';
		    	ptr[1] = 'a';
		    	ptr[2] = 'n';
				ptr[3] = 0;
		    }
		    else
			{
		    	ptr[0] = 'i';
		    	ptr[1] = 'n';
		    	ptr[2] = 'f';
				ptr[3] = 0;
		    }
			return 0xff;
		}

		*ptr++ = '0';

	    int exp10 = ((((exponent>>3))*77+63)>>5) - 38;
	    std::uint32_t t = MantissaMul(fraction, table2[exponent / 8]) + 1;
	    std::uint_fast8_t shift = 7 - (exponent & 7);
	    t >>= shift;

	    std::uint8_t digit = t >> 24;
		digit >>= 4;
	    while(digit == 0)
	    {
	        t &= 0x0fffffff;
			//t <<= 1;
			//t += t << 2;
			t *= 10;
	        digit = (uint8_t)(t >> 28);
	        exp10--;
	    }

	    for(std::uint_fast8_t i = presc+1; i > 0; i--)
	    {
			digit = (uint8_t)(t >> 28);
	        *ptr++ = digit + '0';
	        t &= 0x0fffffff;
			t *= 10;
			//t <<= 1;
			//t += t << 2;
	    }
	    // roundup
		if(buffer[presc+2] >= '5')
	        buffer[presc+1]++;
	    ptr[-1] = 0;
	    ptr-=2;
		for(std::uint_fast8_t i = presc + 1; i > 1; i--)
		{
			if(buffer[i] > '9')
			{
			    buffer[i]-=10;
				buffer[i-1]++;
			}
		}
		while(ptr[0] == '0')
		{
		    *ptr-- = 0;
		}
	    return exp10;
	}

	static char *utoa_fast_div(std::uint32_t value, char *bufferEnd)
	{
	    *bufferEnd = 0;
		divmod10_t<std::uint32_t> res;
		res.quot = value;
	    do
	    {
	        res = divmodu10(res.quot);
	        *--bufferEnd = res.rem + '0';
	    }
	    while (res.quot);

	    return bufferEnd;
	}
};

#endif
