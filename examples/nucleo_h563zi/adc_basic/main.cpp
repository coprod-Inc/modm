/*
 * Copyright (c) 2026, coprod LLC
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <modm/board.hpp>

using namespace Board;

// Arduino A0
using A0 = GpioA6;

int
main()
{
	Board::initialize();

	MODM_LOG_INFO << "STM32H5 ADC basic example" << modm::endl;

	// max. ADC clock for STM32H563: 75 MHz
	// 250 MHz AHB clock / 4 = 62.5 MHz
	Adc1::connect<A0::Inp3>();
	Adc1::initialize(Adc1::ClockMode::SynchronousPrescaler4,
					 Adc1::ClockSource::NoClock,
					 Adc1::Prescaler::Disabled,
					 Adc1::CalibrationMode::SingleEndedInputsMode);
	Adc1::setPinChannel<A0>(Adc1::SampleTime::Cycles48);

	while (true)
	{
		Adc1::startConversion();
		while (!Adc1::isConversionFinished());
		const uint32_t value = Adc1::getValue();
		MODM_LOG_INFO << "A0: " << value << " (" << (value * 3300 / 0xfff) << " mV)" << modm::endl;
		modm::delay(500ms);
	}

	return 0;
}
