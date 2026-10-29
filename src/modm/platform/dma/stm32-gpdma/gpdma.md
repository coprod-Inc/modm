# General Purpose Direct Memory Access (GPDMA)

Driver for the STM32H5 GPDMA1 and GPDMA2 controllers with eight channels each.
Every channel copies from a source to a destination address, paced either by
software or by a hardware request of the source or the destination
(`GpdmaBase::Transfer`). Any channel can serve any of the hardware requests in
`GpdmaBase::Request`.

A channel can either be programmed directly with `GpdmaN::ChannelM::configure()`
or run a chain of linked-list nodes. A chain must have static storage:

```cpp
using Gpdma = modm::platform::GpdmaBase;
static Gpdma::Chain<2> chain{{
    Gpdma::NodeConfig{.transfer = Gpdma::Transfer::SoftwareRequest, /* ... */},
    Gpdma::NodeConfig{.transfer = Gpdma::Transfer::SoftwareRequest, /* ... */},
}};

Gpdma1::enable();
Gpdma1::Channel0::configureChain<chain>(Gpdma::Priority::High, Gpdma::ChainLoop::Stop);
Gpdma1::Channel0::start();
```

`start()` links the nodes and `stop()` unlinks them again, so a chain can be
modified with `reload()` while the channel is stopped. With `ChainLoop::Loop`
the last node points back to the first and the chain repeats.

The channels have different FIFO sizes and only channels 6 and 7 support 2D
addressing, see `GpdmaBase::ChannelHardware`.

The interrupts of all channels are handled by this module and dispatched to the
handlers set with `setTransferCompleteIrqHandler()` and friends. On an error,
the channel is stopped before the error handler is called.

!!! warning "STM32H5E/F not supported"
    The 12-channel GPDMA of the STM32H5E4/E5/F4/F5 is not supported yet.
