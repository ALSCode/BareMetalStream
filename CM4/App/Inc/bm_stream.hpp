/*****************************************************************************
* Author	: Aleksandr Sazhin
* Date		: 2021
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

#ifndef __BASIC_STREAM_H_
#define __BASIC_STREAM_H_

#include <span>
#include <ranges>
#include <algorithm>

#include "usart.hpp"
#include "conv.hpp"


class StreamStorage{
protected:
	static char storage[256];
};

inline char StreamStorage::storage[256] __attribute__((section(".D3_DMA_Section")));

template <class Bus, char Delim>
class StreamBase final: public StreamStorage
{

public:

	using bus_t = Bus;
	using stream_t = StreamBase<Bus, Delim>;

	static constexpr BusMode mode = Bus::mode;


	StreamBase() = default;
	~StreamBase(){ if constexpr (BusMode::BLOCKING != mode) flush(); }

	stream_t& operator << (const char_type auto c){

		if constexpr (BusMode::BLOCKING == mode){

			bus.send(&c, index_t{1});

		} else {

			*it = c;
			it = std::next(it);
		}

		return *this;
	}

	stream_t& operator << (const std::floating_point auto f){

		if constexpr (BusMode::BLOCKING == mode){

			auto [ptr, cnt] = NumConvert::to_string_float(f, buffer.data());

			bus.send(ptr, cnt);

		} else {

			auto [ptr, cnt] = NumConvert::to_string_float(f, buffer.data() + std::distance(buffer.begin(), it));

			it = std::next(it, cnt);
		}

		return *this;
	}

	stream_t& operator << (const num_type auto n){

		auto [ptr, cnt] = NumConvert::to_string_integer( n, &buffer.back() );

		if constexpr (BusMode::BLOCKING == mode){

			bus.send(ptr, cnt);

		} else {

			auto src = std::prev(buffer.end(), cnt + 1);

			it = std::copy(src, buffer.end(), it);
		}

		return *this;
	}

	stream_t& operator << (const std::ranges::range auto& r){

		using arg_type = std::ranges::range_value_t<decltype(r)>;

		if constexpr (char_type<arg_type>){

			std::ranges::for_each(r, [this](const auto val){ *this << val; } );

		} else if (num_type<arg_type> || std::floating_point<arg_type>) {

			std::ranges::for_each(r, [this](const auto val){ *this << val << Delim; } );
		}

		return *this;
	}


private:

	void flush (void) {

		bus.send(buffer.data(), std::distance(buffer.begin(), it));

		it = buffer.begin();
	}

	std::span<char> buffer{storage};
	std::span<char>::iterator it{buffer.begin()};

	bus_t bus;
}; 

using StreamUartBlocking = StreamBase<UartBlocking, ' '>;
using StreamUartIt = StreamBase<UartIt, ' '>;
using StreamUartDma = StreamBase<UartDma, ' '>;


#endif
