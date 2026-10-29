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

#include <modm/board.hpp>
#include <algorithm>
#include <array>

using namespace Board;

/*
 * Copies a buffer with the DMA API of the other STM32 families, then copies
 * it again in two halves with a GPDMA linked list.
 */

std::array<uint32_t, 64> source;
std::array<uint32_t, 64> destination;
volatile uint32_t transfersComplete{0};

using Gpdma = GpdmaBase;
constexpr std::size_t HalfBytes = sizeof(source) / 2;

Gpdma::Chain<2> chain{{
	Gpdma::NodeConfig{
		.transfer = Gpdma::Transfer::SoftwareRequest,
		.sourceAddress = uintptr_t(source.data()),
		.destinationAddress = uintptr_t(destination.data()),
		.blockBytes = HalfBytes,
		.sourceWidth = Gpdma::DataWidth::Word,
		.destinationWidth = Gpdma::DataWidth::Word,
		.sourceIncrement = Gpdma::Increment::Increment,
		.destinationIncrement = Gpdma::Increment::Increment,
	},
	Gpdma::NodeConfig{
		.transfer = Gpdma::Transfer::SoftwareRequest,
		.sourceAddress = uintptr_t(source.data()) + HalfBytes,
		.destinationAddress = uintptr_t(destination.data()) + HalfBytes,
		.blockBytes = HalfBytes,
		.sourceWidth = Gpdma::DataWidth::Word,
		.destinationWidth = Gpdma::DataWidth::Word,
		.sourceIncrement = Gpdma::Increment::Increment,
		.destinationIncrement = Gpdma::Increment::Increment,
	},
}};

void
waitAndCheck(const char* name, uint32_t transfers)
{
	while (transfersComplete < transfers) {}
	transfersComplete = 0;
	MODM_LOG_INFO << name << (source == destination ? ": ok" : ": failed") << modm::endl;
}

int
main()
{
	Board::initialize();
	Leds::setOutput();
	Dma1::enable();

	for (uint32_t i = 0; i < source.size(); ++i) source[i] = i * 0x01010101;

	// Memory-to-memory: the source is the peripheral address
	using Dma = Dma1::Channel0;
	Dma::configure(Dma::DataTransferDirection::MemoryToMemory,
				   Dma::MemoryDataSize::Word, Dma::PeripheralDataSize::Word,
				   Dma::MemoryIncrementMode::Increment, Dma::PeripheralIncrementMode::Increment);
	Dma::setPeripheralAddress(uintptr_t(source.data()));
	Dma::setMemoryAddress(uintptr_t(destination.data()));
	Dma::setDataLength(source.size());
	Dma::setTransferCompleteIrqHandler([] { transfersComplete = transfersComplete + 1; });
	Dma::enableInterrupt(Dma::InterruptEnable::TransferComplete);
	Dma::enableInterruptVector();
	Dma::start();
	waitAndCheck("dma", 1);

	// The same copy in two linked-list nodes, each node completes a transfer
	std::ranges::fill(destination, 0);
	using Channel = Gpdma1::Channel1;
	Channel::configureChain<chain>(Gpdma::Priority::High);
	Channel::setTransferCompleteIrqHandler([] { transfersComplete = transfersComplete + 1; });
	Channel::enableInterrupt(Gpdma::InterruptEnable::TransferComplete);
	Channel::enableInterruptVector();
	Channel::start();
	waitAndCheck("gpdma chain", 2);

	while (true)
	{
		Leds::toggle();
		modm::delay(500ms);
	}

	return 0;
}
