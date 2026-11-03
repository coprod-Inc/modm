# Direct Memory Access (DMA)

The STM32H5 has two General Purpose DMA controllers (GPDMA1 and GPDMA2) with
eight channels each. This module provides the same API as the DMA driver of the
other STM32 families on top of the `modm:platform:gpdma` module, so that
peripheral drivers using `DmaN::ChannelM` work unchanged.

```cpp
using Dma = Dma1::Channel0;
Dma1::enable();
Dma::configure(Dma::DataTransferDirection::PeripheralToMemory,
               Dma::MemoryDataSize::HalfWord, Dma::PeripheralDataSize::HalfWord,
               Dma::MemoryIncrementMode::Increment, Dma::PeripheralIncrementMode::Fixed,
               Dma::Priority::High, Dma::CircularMode::Enabled);
Dma::setPeripheralAddress(uintptr_t(&ADC1->DR));
Dma::setMemoryAddress(uintptr_t(buffer));
Dma::setDataLength(std::size(buffer));
Dma::setPeripheralRequest<Dma::RequestMapping<Peripheral::Adc1>::Request>();
Dma::start();
```

Differences to the other STM32 DMA drivers:

- Any channel can serve any peripheral request, there is no fixed mapping.
- `setDataLength()` takes the number of transfers, which is converted to the
  GPDMA block size in bytes using the source data size. The block size is
  limited to 65535 bytes.
- Circular mode is implemented as a single linked-list node that points to
  itself.
- In `DataTransferDirection::MemoryToMemory`, the source is the peripheral
  address and the destination is the memory address, as on the other STM32 DMAs.

For linked lists, burst transfers and other GPDMA specific features, use the
`modm:platform:gpdma` module directly.

!!! warning "STM32H5E/F not supported"
    The 12-channel GPDMA of the STM32H5E4/E5/F4/F5 is not supported yet.
