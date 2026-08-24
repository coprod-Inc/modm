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

#ifndef MODM_STM32H5_GPDMA_HAL_HPP
#define MODM_STM32H5_GPDMA_HAL_HPP

#include <modm/architecture/interface/assert.hpp>

#include "../device.hpp"
#include "gpdma_base.hpp"

namespace modm::platform
{

/**
 * GPDMA channel register access.
 *
 * @tparam Base Address of @c DMA_Channel_TypeDef (e.g. GPDMA1_Channel0_BASE).
 *
 * @ingroup	modm_platform_gpdma
 */
template<uintptr_t Base>
class GpdmaChannelHal : public GpdmaBase
{
	static DMA_Channel_TypeDef*
	reg()
	{
		return reinterpret_cast<DMA_Channel_TypeDef*>(Base);
	}

	static constexpr uint32_t kMaxBlockBytes = DMA_CBR1_BNDT_Msk;

public:
	static constexpr uint32_t ChannelStride = ChannelRegisterStride;

	static void
	stop()
	{
		auto* channel = reg();
		if (channel->CCR & DMA_CCR_EN)
		{
			channel->CCR |= DMA_CCR_SUSP;
			while ((channel->CSR & (DMA_CSR_SUSPF | DMA_CSR_IDLEF)) == 0)
				;
		}
		channel->CCR |= DMA_CCR_RESET;
		clearLinkedList();
	}

	static void
	start()
	{
		reg()->CCR |= DMA_CCR_EN;
	}

	/**
	 * Direct-programming setup. Clears @c CLLR and the interrupt enables.
	 *
	 * Leaves the request selection untouched, so call @ref setHardwareRequest
	 * afterwards, not before. Defaults: burst length 1, @c BREQ = 0.
	 */
	static void
	configure(Transfer transfer,
			DataWidth sourceWidth,
			DataWidth destinationWidth,
			Increment sourceIncrement,
			Increment destinationIncrement,
			Priority priority = Priority::Medium,
			HardwareRequestMode hardwareRequestMode = HardwareRequestMode::Burst,
			BurstLength sourceBurstLength = BurstLength::Beats1,
			BurstLength destinationBurstLength = BurstLength::Beats1)
	{
		stop();

		reg()->CTR1 = transferRegister1(transfer, sourceWidth, destinationWidth,
				sourceIncrement, destinationIncrement,
				sourceBurstLength, destinationBurstLength);
		reg()->CTR2 = transferBits(transfer) | requestModeBit(hardwareRequestMode);
		clearLinkedList();

		reg()->CCR = uint32_t(priority);
	}

	static void
	setPriority(Priority priority)
	{
		auto* channel = reg();
		channel->CCR = (channel->CCR & ~DMA_CCR_PRIO) | uint32_t(priority);
	}

	static Transfer
	getTransfer()
	{
		const uint32_t ctr2 = reg()->CTR2;
		if (ctr2 & DMA_CTR2_SWREQ)
			return Transfer::SoftwareRequest;
		if (ctr2 & DMA_CTR2_DREQ)
			return Transfer::DestinationRequest;
		return Transfer::SourceRequest;
	}

	static void
	setSourceAddress(uintptr_t address)
	{
		reg()->CSAR = address;
	}

	static void
	setDestinationAddress(uintptr_t address)
	{
		reg()->CDAR = address;
	}

	static void
	setSourceIncrement(bool increment)
	{
		if (increment)
			reg()->CTR1 |= DMA_CTR1_SINC;
		else
			reg()->CTR1 &= ~DMA_CTR1_SINC;
	}

	static void
	setDestinationIncrement(bool increment)
	{
		if (increment)
			reg()->CTR1 |= DMA_CTR1_DINC;
		else
			reg()->CTR1 &= ~DMA_CTR1_DINC;
	}

	static void
	setHardwareRequestMode(HardwareRequestMode mode)
	{
		if (mode == HardwareRequestMode::Block)
			reg()->CTR2 |= DMA_CTR2_BREQ;
		else
			reg()->CTR2 &= ~DMA_CTR2_BREQ;
	}

	static HardwareRequestMode
	hardwareRequestMode()
	{
		return (reg()->CTR2 & DMA_CTR2_BREQ) ? HardwareRequestMode::Block
				: HardwareRequestMode::Burst;
	}

	static void
	setSourceBurstLength(BurstLength length)
	{
		auto* channel = reg();
		channel->CTR1 = (channel->CTR1 & ~DMA_CTR1_SBL_1) |
				burstLengthField(length, DMA_CTR1_SBL_1_Pos);
	}

	static void
	setDestinationBurstLength(BurstLength length)
	{
		auto* channel = reg();
		channel->CTR1 = (channel->CTR1 & ~DMA_CTR1_DBL_1) |
				burstLengthField(length, DMA_CTR1_DBL_1_Pos);
	}

	static BurstLength
	sourceBurstLength()
	{
		return burstLengthFromField(reg()->CTR1, DMA_CTR1_SBL_1_Pos);
	}

	static BurstLength
	destinationBurstLength()
	{
		return burstLengthFromField(reg()->CTR1, DMA_CTR1_DBL_1_Pos);
	}

	static DataWidth
	sourceDataWidth()
	{
		const uint32_t log2 =
				(reg()->CTR1 & DMA_CTR1_SDW_LOG2) >> DMA_CTR1_SDW_LOG2_Pos;
		return DataWidth(log2);
	}

	static uintptr_t
	sourceAddress()
	{
		return reg()->CSAR;
	}

	static uintptr_t
	destinationAddress()
	{
		return reg()->CDAR;
	}

	static void
	setBlockLengthBytes(std::size_t bytes)
	{
		modm_assert(bytes > 0 and bytes <= kMaxBlockBytes, "gpdma", "bndt");
		reg()->CBR1 = uint32_t(bytes) & DMA_CBR1_BNDT;
	}

	static uint32_t
	blockLengthBytes()
	{
		return reg()->CBR1 & DMA_CBR1_BNDT;
	}

	static void
	setHardwareRequest(Request request)
	{
		auto* channel = reg();
		channel->CTR2 = (channel->CTR2 & ~(DMA_CTR2_REQSEL | DMA_CTR2_SWREQ)) |
				requestBits(request);
	}

	static void
	enableInterrupt(InterruptEnable_t irq)
	{
		reg()->CCR |= irq.value;
	}

	static void
	disableInterrupt(InterruptEnable_t irq)
	{
		reg()->CCR &= ~irq.value;
	}

	static InterruptFlags_t
	getInterruptFlags()
	{
		return InterruptFlags_t{reg()->CSR & uint32_t(InterruptFlags::All)};
	}

	static void
	clearInterruptFlags(InterruptFlags_t flags = InterruptFlags::All)
	{
		reg()->CFCR = flags.value;
	}

protected:
	static void
	clearLinkedList()
	{
		reg()->CLLR = 0;
	}

	/// Load @p node into the live registers and arm the link it points to.
	static void
	applyNode(const Node& node)
	{
		auto* channel = reg();
		channel->CTR1 = node.ctr1;
		channel->CTR2 = node.ctr2;
		channel->CBR1 = node.cbr1;
		channel->CSAR = node.csar;
		channel->CDAR = node.cdar;
		channel->CLBAR = nodeWindow(node);
		channel->CLLR = node.cllr;
	}
};

}  // namespace modm::platform

#endif  // MODM_STM32H5_GPDMA_HAL_HPP
