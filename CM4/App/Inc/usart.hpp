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

#ifndef USART_HPP_
#define USART_HPP_

#pragma once

#include "usart.h"
#include "dma.h"
#include "bm_bus.hpp"


enum class IrqMask : base_t {
	transmit = 1,
	half_transmit = 2,
	receive = 4,
	half_receive = 8,
	/*--------------*/
	initial = 3,
};

struct UartIrq{

	static base_t test (IrqMask m);
};

template<BusMode Mode>
class Uart final : public BusInterface<Uart<Mode>> {

private:

	static constexpr BusMode mode = Mode;

	void send_block (const char arr[], const index_t num) noexcept {

		HAL_StatusTypeDef res{HAL_ERROR};

		res = HAL_UART_Transmit(
				&huart3,
				bit_cast<std::uint8_t*>(arr),
				std::uint16_t(num),
				base_t{5000}
		);

		Inspector::approve(HAL_OK == res);
	}

	void send_it (const char arr[], const index_t num) noexcept {

		HAL_StatusTypeDef res{HAL_ERROR};

		if ( UartIrq::test(IrqMask::transmit) ) {

			res = HAL_UART_Transmit_IT(
					&huart3,
					bit_cast<std::uint8_t*>(arr),
					std::uint16_t(num)
			);
		}

		Inspector::approve(HAL_OK == res);
	}

	void send_dma (const char arr[], const index_t num) noexcept {

		HAL_StatusTypeDef res{HAL_ERROR};

		if ( UartIrq::test(IrqMask::transmit) ) {

			res = HAL_UART_Transmit_DMA(
					&huart3,
					bit_cast<std::uint8_t*>(arr),
					std::uint16_t(num)
			);
		}

		Inspector::approve(HAL_OK == res);
	}

	friend class BusInterface<Uart<BusMode::BLOCKING>>;
	friend class BusInterface<Uart<BusMode::IT>>;
	friend class BusInterface<Uart<BusMode::DMA>>;
};

using UartBlocking = BusInterface<Uart<BusMode::BLOCKING>>;
using UartIt = BusInterface<Uart<BusMode::IT>>;
using UartDma = BusInterface<Uart<BusMode::DMA>>;


#endif /* USART_HPP_ */
