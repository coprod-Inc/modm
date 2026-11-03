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

#include <modm/platform/gpdma/gpdma_base.hpp>

namespace modm::platform
{

/**
 * DMA names of the other STM32 DMA drivers on top of the GPDMA.
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
		/// Source: peripheral address; Sink: memory address
		PeripheralToMemory = 0,
		/// Source: memory address; Sink: peripheral address
		MemoryToPeripheral = 1,
		/// Source: peripheral address; Sink: memory address
		MemoryToMemory = 2,
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
};

}  // namespace modm::platform

#endif  // MODM_STM32H5_DMA_BASE_HPP
