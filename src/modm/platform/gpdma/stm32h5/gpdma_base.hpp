/*
 * Copyright (c) 2026, coprod LLC
 *
 * @author  Adam Mesbahi (adam.mesbahi@coprod.ch)
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32H5_GPDMA_BASE_HPP
#define MODM_STM32H5_GPDMA_BASE_HPP

#include <array>
#include <cstddef>
#include <cstdint>

#include "../device.hpp"
#include <modm/architecture/interface/assert.hpp>
#include <modm/architecture/interface/register.hpp>

namespace modm::platform
{

/**
 * Common GPDMA definitions for STM32H5 (GPDMA1 and GPDMA2).
 *
 * Channel hardware parameters are taken from RM0481 (H52x/H56x/H573, also H503):
 *
 * | Channel x | dma_fifo_size[x] | dma_addressing[x] |
 * |-----------|------------------|-------------------|
 * | 0 to 3    | 2                | 0                 |
 * | 4 to 5    | 4                | 0                 |
 * | 6 to 7    | 4                | 1                 |
 *
 * FIFO depth in bytes is `2^(dma_fifo_size[x] + 1)`.
 *
 * @author	coprod
 * @ingroup	modm_platform_gpdma
 */
class GpdmaBase
{
public:
	static constexpr uint8_t ChannelCount = 8;
	static constexpr uint32_t ChannelRegisterStride = 0x80;

	enum class Channel : uint8_t
	{
		Channel0 = 0,
		Channel1,
		Channel2,
		Channel3,
		Channel4,
		Channel5,
		Channel6,
		Channel7,
	};

	/**
	 * Hardware request selection (`GPDMA_CxTR2.REQSEL[7:0]`, RM0481 Table 142).
	 *
	 * Same map for GPDMA1 and GPDMA2. Used only when `SWREQ = 0`.
	 * Some IDs exist only on parts that have that peripheral.
	 */
	enum class Request : uint8_t
	{
		Adc1 = 0,
		Adc2 = 1,
		Dac1Ch1 = 2,
		Dac1Ch2 = 3,
		Tim6Upd = 4,
		Tim7Upd = 5,
		Spi1Rx = 6,
		Spi1Tx = 7,
		Spi2Rx = 8,
		Spi2Tx = 9,
		Spi3Rx = 10,
		Spi3Tx = 11,
		I2c1Rx = 12,
		I2c1Tx = 13,
		// 14 reserved
		I2c2Rx = 15,
		I2c2Tx = 16,
		// 17 reserved
		I2c3Rx = 18,
		I2c3Tx = 19,
		// 20 reserved
		Usart1Rx = 21,
		Usart1Tx = 22,
		Usart2Rx = 23,
		Usart2Tx = 24,
		Usart3Rx = 25,
		Usart3Tx = 26,
		Uart4Rx = 27,
		Uart4Tx = 28,
		Uart5Rx = 29,
		Uart5Tx = 30,
		Usart6Rx = 31,
		Usart6Tx = 32,
		Uart7Rx = 33,
		Uart7Tx = 34,
		Uart8Rx = 35,
		Uart8Tx = 36,
		Uart9Rx = 37,
		Uart9Tx = 38,
		Uart10Rx = 39,
		Uart10Tx = 40,
		Uart11Rx = 41,
		Uart11Tx = 42,
		Uart12Rx = 43,
		Uart12Tx = 44,
		Lpuart1Rx = 45,
		Lpuart1Tx = 46,
		Spi4Rx = 47,
		Spi4Tx = 48,
		Spi5Rx = 49,
		Spi5Tx = 50,
		Spi6Rx = 51,
		Spi6Tx = 52,
		Sai1A = 53,
		Sai1B = 54,
		Sai2A = 55,
		Sai2B = 56,
		Ospi1 = 57,
		Tim1Cc1 = 58,
		Tim1Cc2 = 59,
		Tim1Cc3 = 60,
		Tim1Cc4 = 61,
		Tim1Upd = 62,
		Tim1Trg = 63,
		Tim1Com = 64,
		Tim8Cc1 = 65,
		Tim8Cc2 = 66,
		Tim8Cc3 = 67,
		Tim8Cc4 = 68,
		Tim8Upd = 69,
		Tim8Trg = 70,  ///< RM0481 lists this as tim8_tig_dma
		Tim8Com = 71,
		Tim2Cc1 = 72,
		Tim2Cc2 = 73,
		Tim2Cc3 = 74,
		Tim2Cc4 = 75,
		Tim2Upd = 76,
		Tim3Cc1 = 77,
		Tim3Cc2 = 78,
		Tim3Cc3 = 79,
		Tim3Cc4 = 80,
		Tim3Upd = 81,
		Tim3Trg = 82,
		Tim4Cc1 = 83,
		Tim4Cc2 = 84,
		Tim4Cc3 = 85,
		Tim4Cc4 = 86,
		Tim4Upd = 87,
		Tim5Cc1 = 88,
		Tim5Cc2 = 89,
		Tim5Cc3 = 90,
		Tim5Cc4 = 91,
		Tim5Upd = 92,
		Tim5Trg = 93,
		Tim15Cc1 = 94,
		Tim15Upd = 95,
		Tim15Trg = 96,
		Tim15Com = 97,
		Tim16Cc1 = 98,
		Tim16Upd = 99,
		Tim17Cc1 = 100,
		Tim17Upd = 101,
		Lptim1Ic1 = 102,
		Lptim1Ic2 = 103,
		Lptim1Ue = 104,
		Lptim2Ic1 = 105,
		Lptim2Ic2 = 106,
		Lptim2Ue = 107,
		Dcmi = 108,
		Pssi = Dcmi,
		AesOut = 109,
		AesIn = 110,
		HashIn = 111,
		Ucpd1Rx = 112,
		Ucpd1Tx = 113,
		CordicRead = 114,
		CordicWrite = 115,
		FmacRead = 116,
		FmacWrite = 117,
		SaesOut = 118,
		SaesIn = 119,
		I3c1Rx = 120,
		I3c1Tx = 121,
		I3c1Tc = 122,
		I3c1Rs = 123,
		I2c4Rx = 124,
		I2c4Tx = 125,
		// 126 reserved
		Lptim3Ic1 = 127,
		Lptim3Ic2 = 128,
		Lptim3Ue = 129,
		Lptim5Ic1 = 130,
		Lptim5Ic2 = 131,
		Lptim5Ue = 132,
		Lptim6Ic1 = 133,
		Lptim6Ic2 = 134,
		Lptim6Ue = 135,
		I3c2Rx = 136,
		I3c2Tx = 137,
		I3c2Tc = 138,
		I3c2Rs = 139,
		// 140, 141 reserved
		Adc3 = 142,
	};

	/// Channel priority (`GPDMA_CxCR.PRIO`).
	enum class Priority : uint32_t
	{
		Low = 0,
		Medium = DMA_CCR_PRIO_0,
		High = DMA_CCR_PRIO_1,
		VeryHigh = DMA_CCR_PRIO_1 | DMA_CCR_PRIO_0,
	};

	/// Binary logarithm of a beat width (`CTR1` `SDW_LOG2` / `DDW_LOG2`).
	enum class DataWidth : uint8_t
	{
		Byte = 0,
		Bit8 = Byte,
		HalfWord = 1,
		Bit16 = HalfWord,
		Word = 2,
		Bit32 = Word,
	};

	enum class Increment : uint8_t
	{
		Fixed = 0,
		Increment = 1,
	};

	/// How the channel is paced (`CTR2` `SWREQ` / `DREQ`). GPDMA has no DIR bit.
	enum class Transfer : uint8_t
	{
		SourceRequest = 0,       ///< Hardware request on the source (`DREQ` = 0, `SWREQ` = 0)
		DestinationRequest = 1,  ///< Hardware request on the destination (`DREQ` = 1)
		SoftwareRequest = 2,     ///< Software request (`SWREQ` = 1)
	};

	/// After the last node: idle (`cllr` = 0) or loop the chain (last → first).
	enum class ChainLoop : uint8_t
	{
		Stop = 0,  ///< Last `cllr` = 0, channel goes idle after the last block
		Loop = 1,  ///< Last `cllr` points at the first node
	};

	/// `CTR2.BREQ`: what one hardware request completes.
	enum class HardwareRequestMode : uint8_t
	{
		Burst = 0,  ///< One request transfers one burst (`BREQ` = 0)
		Block = 1,  ///< One request transfers the whole block (`BREQ` = 1)
	};

	/// Burst length in beats (`CTR1` `SBL_1` / `DBL_1` = beats − 1). Range 1–64.
	enum class BurstLength : uint8_t
	{
		Beats1 = 1,
		Beats2,
		Beats3,
		Beats4,
		Beats5,
		Beats6,
		Beats7,
		Beats8,
		Beats9,
		Beats10,
		Beats11,
		Beats12,
		Beats13,
		Beats14,
		Beats15,
		Beats16,
		Beats17,
		Beats18,
		Beats19,
		Beats20,
		Beats21,
		Beats22,
		Beats23,
		Beats24,
		Beats25,
		Beats26,
		Beats27,
		Beats28,
		Beats29,
		Beats30,
		Beats31,
		Beats32,
		Beats33,
		Beats34,
		Beats35,
		Beats36,
		Beats37,
		Beats38,
		Beats39,
		Beats40,
		Beats41,
		Beats42,
		Beats43,
		Beats44,
		Beats45,
		Beats46,
		Beats47,
		Beats48,
		Beats49,
		Beats50,
		Beats51,
		Beats52,
		Beats53,
		Beats54,
		Beats55,
		Beats56,
		Beats57,
		Beats58,
		Beats59,
		Beats60,
		Beats61,
		Beats62,
		Beats63,
		Beats64,
	};

	static_assert(uint8_t(BurstLength::Beats1) == 1);
	static_assert(uint8_t(BurstLength::Beats64) == 64);

	static constexpr uint8_t kMinBurstBeats = 1;
	static constexpr uint8_t kMaxBurstBeats = 64;

	static constexpr uint8_t
	burstLengthBeats(BurstLength length)
	{
		return uint8_t(length);
	}

	enum class InterruptEnable : uint32_t
	{
		TransferComplete = DMA_CCR_TCIE,
		HalfTransfer = DMA_CCR_HTIE,
		DataTransferError = DMA_CCR_DTEIE,
		UpdateLinkedListError = DMA_CCR_ULEIE,
		UserSettingError = DMA_CCR_USEIE,
		TriggerOverrun = DMA_CCR_TOIE,
	};
	MODM_FLAGS32(InterruptEnable);

	enum class InterruptFlags : uint32_t
	{
		TransferComplete = DMA_CSR_TCF,
		HalfTransferComplete = DMA_CSR_HTF,
		DataTransferError = DMA_CSR_DTEF,
		UpdateLinkedListError = DMA_CSR_ULEF,
		UserSettingError = DMA_CSR_USEF,
		TriggerOverrun = DMA_CSR_TOF,
		Error = DMA_CSR_DTEF | DMA_CSR_ULEF | DMA_CSR_USEF | DMA_CSR_TOF,
		All = DMA_CSR_TCF | DMA_CSR_HTF | DMA_CSR_DTEF | DMA_CSR_ULEF | DMA_CSR_USEF |
			  DMA_CSR_TOF,
	};
	MODM_FLAGS32(InterruptFlags);

	using IrqHandler = void (*)();

	static constexpr std::size_t
	dataWidthBytes(DataWidth width)
	{
		return std::size_t{1} << uint8_t(width);
	}

	// Register field encoding, shared by direct programming and linked-list nodes.

	static constexpr uint32_t
	burstLengthField(BurstLength length, uint32_t position)
	{
		return uint32_t(burstLengthBeats(length) - 1) << position;
	}

	static constexpr BurstLength
	burstLengthFromField(uint32_t ctr1, uint32_t position)
	{
		return BurstLength(((ctr1 >> position) & 0x3F) + 1);
	}

	/// Allocated ports (`CTR1` `SAP`/`DAP`): port 0 for the peripheral, port 1 for SRAM.
	static constexpr uint32_t
	portBits(Transfer transfer)
	{
		uint32_t bits = 0;
		if (transfer != Transfer::SourceRequest)
			bits |= DMA_CTR1_SAP;
		if (transfer != Transfer::DestinationRequest)
			bits |= DMA_CTR1_DAP;
		return bits;
	}

	/// Pacing bits (`CTR2` `SWREQ`/`DREQ`).
	static constexpr uint32_t
	transferBits(Transfer transfer)
	{
		if (transfer == Transfer::SoftwareRequest)
			return DMA_CTR2_SWREQ;
		if (transfer == Transfer::DestinationRequest)
			return DMA_CTR2_DREQ;
		return 0;
	}

	static constexpr uint32_t
	requestBits(Request request)
	{
		return uint32_t(request) << DMA_CTR2_REQSEL_Pos;
	}

	static constexpr uint32_t
	requestModeBit(HardwareRequestMode mode)
	{
		return (mode == HardwareRequestMode::Block) ? DMA_CTR2_BREQ : 0;
	}

	/// Complete `CTR1` value: data widths, increments, allocated ports, burst lengths.
	static constexpr uint32_t
	transferRegister1(Transfer transfer,
			DataWidth sourceWidth,
			DataWidth destinationWidth,
			Increment sourceIncrement,
			Increment destinationIncrement,
			BurstLength sourceBurstLength = BurstLength::Beats1,
			BurstLength destinationBurstLength = BurstLength::Beats1)
	{
		uint32_t ctr1 = (uint32_t(sourceWidth) << DMA_CTR1_SDW_LOG2_Pos) |
				(uint32_t(destinationWidth) << DMA_CTR1_DDW_LOG2_Pos) |
				burstLengthField(sourceBurstLength, DMA_CTR1_SBL_1_Pos) |
				burstLengthField(destinationBurstLength, DMA_CTR1_DBL_1_Pos) |
				portBits(transfer);
		if (sourceIncrement == Increment::Increment)
			ctr1 |= DMA_CTR1_SINC;
		if (destinationIncrement == Increment::Increment)
			ctr1 |= DMA_CTR1_DINC;
		return ctr1;
	}

	/// Complete `CTR2` value: request selection, pacing, burst/block request.
	static constexpr uint32_t
	transferRegister2(Transfer transfer,
			Request request,
			HardwareRequestMode hardwareRequestMode = HardwareRequestMode::Burst)
	{
		uint32_t ctr2 = transferBits(transfer) | requestModeBit(hardwareRequestMode);
		if (transfer != Transfer::SoftwareRequest)
			ctr2 |= requestBits(request);
		return ctr2;
	}

	/**
	 * One linked-list item (LLI) in SRAM.
	 *
	 * The GPDMA reads the registers named by @ref NodeUpdateMask back to back in
	 * ascending register order, so the word order below is fixed by hardware.
	 * Padding added by the alignment sits after @c cllr and is never read.
	 *
	 * Only 4-byte alignment is required; 32 bytes keeps a node inside one cache line.
	 */
	struct alignas(32) Node
	{
		uint32_t ctr1;
		uint32_t ctr2;
		uint32_t cbr1;
		uint32_t csar;
		uint32_t cdar;
		uint32_t cllr;
	};

	/// Update bits describing the @ref Node layout. Goes into the *pointing* `CLLR`.
	static constexpr uint32_t NodeUpdateMask = DMA_CLLR_UT1 | DMA_CLLR_UT2 |
			DMA_CLLR_UB1 | DMA_CLLR_USA | DMA_CLLR_UDA | DMA_CLLR_ULL;

	/// One block of a chain. Mirrors the arguments of a direct @c configure().
	struct NodeConfig
	{
		Transfer transfer{Transfer::SourceRequest};
		Request request{};
		uintptr_t sourceAddress{};
		uintptr_t destinationAddress{};
		std::size_t blockBytes{};
		DataWidth sourceWidth{DataWidth::Byte};
		DataWidth destinationWidth{DataWidth::Byte};
		Increment sourceIncrement{Increment::Fixed};
		Increment destinationIncrement{Increment::Increment};
		HardwareRequestMode hardwareRequestMode{HardwareRequestMode::Burst};
		BurstLength sourceBurstLength{BurstLength::Beats1};
		BurstLength destinationBurstLength{BurstLength::Beats1};
	};

	/// Build one node. @c cllr stays 0 until @c start() connects the chain.
	static constexpr Node
	makeNode(const NodeConfig& config)
	{
		return Node{
			transferRegister1(config.transfer,
					config.sourceWidth,
					config.destinationWidth,
					config.sourceIncrement,
					config.destinationIncrement,
					config.sourceBurstLength,
					config.destinationBurstLength),
			transferRegister2(config.transfer,
					config.request,
					config.hardwareRequestMode),
			uint32_t(config.blockBytes) & DMA_CBR1_BNDT,
			uint32_t(config.sourceAddress),
			uint32_t(config.destinationAddress),
			0,
		};
	}

	/// `CLBAR` value for @p node: the 64 KB window the node lives in.
	static uint32_t
	nodeWindow(const Node& node)
	{
		return uint32_t(uintptr_t(&node)) & DMA_CLBAR_LBA;
	}

	/// `CLLR` value pointing at @p node: its link address plus @ref NodeUpdateMask.
	static uint32_t
	nodeLink(const Node& node)
	{
		const auto address = uint32_t(uintptr_t(&node));
		modm_assert((address & 0x3) == 0, "gpdma", "lli");
		return (address & DMA_CLLR_LA) | NodeUpdateMask;
	}

	/**
	 * Ordered list of @ref Node items. Give it static storage and pass it to
	 * @c configureChain<chain>(). A local or heap chain cannot be used.
	 *
	 * Nodes are stored unlinked (`cllr` = 0). @c start() writes the next-node
	 * pointers; @c stop() clears them again. @ref ChainLoop::Loop points the
	 * last node at the first.
	 */
	template<std::size_t N>
	class Chain
	{
		static_assert(N >= 1, "chain must have at least one node");

	public:
		constexpr explicit Chain(const NodeConfig (&configs)[N])
		{
			for (std::size_t i = 0; i < N; ++i)
				nodes_[i] = makeNode(configs[i]);
		}

		constexpr explicit Chain(const std::array<NodeConfig, N>& configs)
		{
			for (std::size_t i = 0; i < N; ++i)
				nodes_[i] = makeNode(configs[i]);
		}

		Chain(const Chain&) = delete;
		Chain& operator=(const Chain&) = delete;

		/// Write `cllr` next-pointers. Last node stays 0 unless @ref ChainLoop::Loop.
		void
		connect(ChainLoop chainLoop = ChainLoop::Stop)
		{
			const uint32_t window = nodeWindow(nodes_[0]);
			for (std::size_t i = 0; i < N; ++i)
			{
				modm_assert(nodeWindow(nodes_[i]) == window, "gpdma", "lliwin");
				if (i + 1 < N)
					nodes_[i].cllr = nodeLink(nodes_[i + 1]);
				else if (chainLoop == ChainLoop::Loop)
					nodes_[i].cllr = nodeLink(nodes_[0]);
				else
					nodes_[i].cllr = 0;
			}
			cleanDataCache(nodes_, sizeof(nodes_));
		}

		/// Restore every node to `cllr` = 0.
		void
		disconnect()
		{
			for (std::size_t i = 0; i < N; ++i)
				nodes_[i].cllr = 0;
			cleanDataCache(nodes_, sizeof(nodes_));
		}

		const Node&
		front() const
		{
			return nodes_[0];
		}

		/// Replace node payloads. `cllr` stays 0 until @c connect().
		void
		reload(const NodeConfig (&configs)[N])
		{
			for (std::size_t i = 0; i < N; ++i)
				nodes_[i] = makeNode(configs[i]);
			disconnect();
		}

		void
		reload(const std::array<NodeConfig, N>& configs)
		{
			for (std::size_t i = 0; i < N; ++i)
				nodes_[i] = makeNode(configs[i]);
			disconnect();
		}

		void
		reload(const NodeConfig& config)
			requires (N == 1)
		{
			nodes_[0] = makeNode(config);
			disconnect();
		}

	private:
		Node nodes_[N]{};
	};

	/// Make CPU writes visible to the GPDMA. No-op unless the D-cache is on.
	static void
	cleanDataCache([[maybe_unused]] const void* address, [[maybe_unused]] std::size_t bytes)
	{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
		if (SCB->CCR & SCB_CCR_DC_Msk)
		{
			SCB_CleanDCache_by_Addr(
					reinterpret_cast<uint32_t*>(const_cast<void*>(address)), int32_t(bytes));
		}
#endif
	}

	/// RM0481 `dma_addressing[x]`.
	enum class Addressing : uint8_t
	{
		Linear = 0,          ///< Fixed / contiguously incremented addressing
		TwoDimensional = 1,  ///< 2D addressing
	};

	/**
	 * Hardware parameters of one GPDMA channel (RM0481).
	 *
	 * @c dmaFifoSize is the raw `dma_fifo_size[x]` value.
	 * FIFO depth in bytes is `2^(dmaFifoSize + 1)`.
	 */
	struct ChannelParameters
	{
		uint8_t dmaFifoSize;
		Addressing addressing;

		constexpr std::size_t
		fifoBytes() const
		{
			return std::size_t{1} << (dmaFifoSize + 1);
		}

		constexpr std::size_t
		fifoWords() const
		{
			return fifoBytes() / sizeof(uint32_t);
		}

		constexpr bool
		has2dAddressing() const
		{
			return addressing == Addressing::TwoDimensional;
		}
	};

	/**
	 * Per-channel hardware parameters, indexed by @ref Channel.
	 *
	 * - Channels 0–3: 8-byte / 2-word FIFO, linear addressing.
	 *   Typical: APB or AHB peripheral ↔ SRAM.
	 * - Channels 4–5: 32-byte / 8-word FIFO, linear addressing.
	 *   Typical: demanding AHB peripheral ↔ SRAM, or external memories.
	 * - Channels 6–7: 32-byte / 8-word FIFO, 2D addressing.
	 *   Typical: demanding AHB peripheral ↔ SRAM, or external memories.
	 */
	static constexpr std::array<ChannelParameters, ChannelCount> channelParameters{{
		{2, Addressing::Linear},
		{2, Addressing::Linear},
		{2, Addressing::Linear},
		{2, Addressing::Linear},
		{4, Addressing::Linear},
		{4, Addressing::Linear},
		{4, Addressing::TwoDimensional},
		{4, Addressing::TwoDimensional},
	}};

	static constexpr ChannelParameters
	parameters(Channel channel)
	{
		return channelParameters[uint8_t(channel)];
	}

	/// Compile-time traits for channel @tparam C.
	template<Channel C>
	struct ChannelHardware
	{
		static_assert(uint8_t(C) < ChannelCount, "invalid GPDMA channel");

		static constexpr ChannelParameters value{parameters(C)};
		static constexpr uint8_t dmaFifoSize = value.dmaFifoSize;
		static constexpr Addressing addressing = value.addressing;
		static constexpr std::size_t fifoBytes = value.fifoBytes();
		static constexpr std::size_t fifoWords = value.fifoWords();
		static constexpr bool has2dAddressing = value.has2dAddressing();
	};
};

}  // namespace modm::platform

#endif  // MODM_STM32H5_GPDMA_BASE_HPP
