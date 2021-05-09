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

#ifndef __BM_BUS_H_
#define __BM_BUS_H_


#pragma once

#include "bm_defs.hpp"

enum class BusMode{
	BLOCKING,
	IT,
	DMA,
};

template<typename T>
class BusInterface{

public:

	using derived_ptr = T*;
	static constexpr BusMode mode = T::mode;

	BusInterface(){}

	void send (const char arr[], const index_t num) noexcept {

		if constexpr (BusMode::BLOCKING == mode){

			derived()->send_block(arr, num);

		} else if constexpr (BusMode::IT == mode){

			derived()->send_it(arr, num);

		} else if constexpr (BusMode::DMA == mode){

			derived()->send_dma(arr, num);
		}
	}


private:

	derived_ptr derived(void) noexcept{
		return static_cast<derived_ptr>(this);
	}

	void send_block (const char arr[], const index_t num) noexcept {}

	void send_it (const char arr[], const index_t num) noexcept {}

	void send_dma (const char arr[], const index_t num) noexcept {}
};

#endif /*__BM_BUS_H_*/
