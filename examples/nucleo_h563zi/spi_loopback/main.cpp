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

// SPI4 is a limited instance (8 byte FIFO, TSIZE < 0x3FF). The buffer below
// is larger than that, so the driver splits it into multiple transfers.
//
// Connect MOSI (PE14) to MISO (PE13).
using Spi = SpiMaster4;
using Sck = GpioE12;
using Miso = GpioE13;
using Mosi = GpioE14;

std::array<uint8_t, 1500> tx;
std::array<uint8_t, 1500> rx;

int
main()
{
	Board::initialize();
	Leds::setOutput();

	Spi::connect<Sck::Sck, Mosi::Mosi, Miso::Miso>();
	Spi::initialize<SystemClock, 15.625_MHz>();

	MODM_LOG_INFO << "SPI loopback test" << modm::endl;

	uint32_t counter{0};
	while (true)
	{
		for (std::size_t i = 0; i < tx.size(); ++i) {
			tx[i] = uint8_t(i + counter);
		}
		rx.fill(0);

		Spi::transfer(tx.data(), rx.data(), tx.size());
		const bool bufferOk = std::ranges::equal(tx, rx);

		const uint8_t value = uint8_t(counter);
		const bool byteOk = (Spi::transfer(value) == value);

		// rx only: the driver transmits zeros
		rx.fill(0xff);
		Spi::transfer(nullptr, rx.data(), rx.size());
		const bool rxOnlyOk = std::ranges::all_of(rx, [](uint8_t b) { return b == 0; });

		const bool ok = bufferOk and byteOk and rxOnlyOk;
		MODM_LOG_INFO << "loop " << counter << ": buffer " << bufferOk << ", byte " << byteOk
					  << ", rx only " << rxOnlyOk << modm::endl;

		Leds::write(ok ? 0b001 : 0b100);
		++counter;
		modm::delay(500ms);
	}

	return 0;
}
