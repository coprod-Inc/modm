/*
 * Copyright (c) 2026, coprod LLC
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32H5_DMA_BASE_HPP
#define MODM_STM32H5_DMA_BASE_HPP

#include <modm/platform/core/peripherals.hpp>
#include <modm/platform/gpdma/gpdma_base.hpp>

namespace modm::platform
{

/**
 * Classic DMA names on top of GPDMA.
 *
 * @ingroup	modm_platform_dma
 */
class DmaBase : public GpdmaBase
{
public:
	using MemoryDataSize = DataWidth;
	using PeripheralDataSize = DataWidth;
	using MemoryIncrementMode = Increment;
	using PeripheralIncrementMode = Increment;

	enum class CircularMode : uint8_t
	{
		Disabled = 0,
		Enabled = 1,  ///< One-node GPDMA chain with @ref ChainLoop::Loop
	};

	enum class DataTransferDirection : uint8_t
	{
		PeripheralToMemory = 0,
		MemoryToPeripheral = 1,
		MemoryToMemory = 2,
	};

	enum class Signal : uint8_t
	{
		NoSignal,
		Rx,
		Tx,
	};

protected:
	static constexpr Transfer
	toTransfer(DataTransferDirection direction)
	{
		if (direction == DataTransferDirection::MemoryToMemory)
			return Transfer::SoftwareRequest;
		if (direction == DataTransferDirection::MemoryToPeripheral)
			return Transfer::DestinationRequest;
		return Transfer::SourceRequest;
	}

	template<Peripheral peripheral, Signal signal>
	static constexpr Request
	mappedRequest()
	{
		if constexpr (peripheral == Peripheral::Adc1 and signal == Signal::NoSignal)
			return Request::Adc1;
		else if constexpr (peripheral == Peripheral::Adc2 and signal == Signal::NoSignal)
			return Request::Adc2;
		else if constexpr (peripheral == Peripheral::Spi1 and signal == Signal::Rx)
			return Request::Spi1Rx;
		else if constexpr (peripheral == Peripheral::Spi1 and signal == Signal::Tx)
			return Request::Spi1Tx;
		else if constexpr (peripheral == Peripheral::Spi2 and signal == Signal::Rx)
			return Request::Spi2Rx;
		else if constexpr (peripheral == Peripheral::Spi2 and signal == Signal::Tx)
			return Request::Spi2Tx;
		else if constexpr (peripheral == Peripheral::Spi3 and signal == Signal::Rx)
			return Request::Spi3Rx;
		else if constexpr (peripheral == Peripheral::Spi3 and signal == Signal::Tx)
			return Request::Spi3Tx;
		else if constexpr (peripheral == Peripheral::Usart1 and signal == Signal::Rx)
			return Request::Usart1Rx;
		else if constexpr (peripheral == Peripheral::Usart1 and signal == Signal::Tx)
			return Request::Usart1Tx;
		else if constexpr (peripheral == Peripheral::Usart2 and signal == Signal::Rx)
			return Request::Usart2Rx;
		else if constexpr (peripheral == Peripheral::Usart2 and signal == Signal::Tx)
			return Request::Usart2Tx;
		else if constexpr (peripheral == Peripheral::Usart3 and signal == Signal::Rx)
			return Request::Usart3Rx;
		else if constexpr (peripheral == Peripheral::Usart3 and signal == Signal::Tx)
			return Request::Usart3Tx;
		else if constexpr (peripheral == Peripheral::I2c1 and signal == Signal::Rx)
			return Request::I2c1Rx;
		else if constexpr (peripheral == Peripheral::I2c1 and signal == Signal::Tx)
			return Request::I2c1Tx;
		else if constexpr (peripheral == Peripheral::I2c2 and signal == Signal::Rx)
			return Request::I2c2Rx;
		else if constexpr (peripheral == Peripheral::I2c2 and signal == Signal::Tx)
			return Request::I2c2Tx;
		else if constexpr (peripheral == Peripheral::Lpuart1 and signal == Signal::Rx)
			return Request::Lpuart1Rx;
		else if constexpr (peripheral == Peripheral::Lpuart1 and signal == Signal::Tx)
			return Request::Lpuart1Tx;
		else
		{
			static_assert(peripheral != peripheral,
					"no GPDMA request for this peripheral/signal");
			return Request::Adc1;
		}
	}
};

}  // namespace modm::platform

#endif  // MODM_STM32H5_DMA_BASE_HPP
