#include "asio.h"

#include "endian.h"
#include "string.h"

#include <cstring>
#include <utility>

#include <common\asio.h>

namespace flexasio {
	// Somewhat surprisingly, ASIO 64-bit integer data types store the most significant 32 bits half *first*, even on little endian architectures. See the ASIO SDK documentation.
	// On x86, which is little endian, that means we can't simply represent the value as an int64_t - we need to swap the two halves first.

	template <typename ASIOInt64> int64_t ASIOToInt64(ASIOInt64 asioInt64) {
		if constexpr (endianness == Endianness::LITTLE) std::swap(asioInt64.hi, asioInt64.lo);
		int64_t result;
		static_assert(sizeof asioInt64 == sizeof result);
		memcpy(&result, &asioInt64, sizeof result);
		return result;
	}
	template int64_t ASIOToInt64<ASIOTimeStamp>(ASIOTimeStamp);
	template int64_t ASIOToInt64<ASIOSamples>(ASIOSamples);

	template <typename ASIOInt64> ASIOInt64 Int64ToASIO(int64_t int64) {
		ASIOInt64 result;
		static_assert(sizeof int64 == sizeof result);
		memcpy(&result, &int64, sizeof result);
		if constexpr (endianness == Endianness::LITTLE) std::swap(result.hi, result.lo);
		return result;
	}
	template ASIOTimeStamp Int64ToASIO<ASIOTimeStamp>(int64_t);
	template ASIOSamples Int64ToASIO<ASIOSamples>(int64_t);

	std::string GetASIOErrorString(ASIOError error) {
		return EnumToString(error, {
			{ASE_OK, "ASE_OK"},
			{ASE_SUCCESS, "ASE_SUCCESS"},
			{ASE_NotPresent, "ASE_NotPresent"},
			{ASE_HWMalfunction, "ASE_HWMalfunction"},
			{ASE_InvalidParameter, "ASE_InvalidParameter"},
			{ASE_InvalidMode, "ASE_InvalidMode"},
			{ASE_SPNotAdvancing, "ASE_SPNotAdvancing"},
			{ASE_NoClock, "ASE_NoClock"},
			{ASE_NoMemory, "ASE_NoMemory"},
			});
	}

	std::string GetASIOSampleTypeString(ASIOSampleType sampleType) {
		return EnumToString(sampleType, {
			{ASIOSTInt16MSB, "ASIOSTInt16MSB"},
			{ASIOSTInt24MSB, "ASIOSTInt24MSB"},
			{ASIOSTInt32MSB, "ASIOSTInt32MSB"},
			{ASIOSTFloat32MSB, "ASIOSTFloat32MSB"},
			{ASIOSTFloat64MSB, "ASIOSTFloat64MSB"},
			{ASIOSTInt32MSB16, "ASIOSTInt32MSB16"},
			{ASIOSTInt32MSB18, "ASIOSTInt32MSB18"},
			{ASIOSTInt32MSB20, "ASIOSTInt32MSB20"},
			{ASIOSTInt32MSB24, "ASIOSTInt32MSB24"},
			{ASIOSTInt16LSB, "ASIOSTInt16LSB"},
			{ASIOSTInt24LSB, "ASIOSTInt24LSB"},
			{ASIOSTInt32LSB, "ASIOSTInt32LSB"},
			{ASIOSTFloat32LSB, "ASIOSTFloat32LSB"},
			{ASIOSTFloat64LSB, "ASIOSTFloat64LSB"},
			{ASIOSTInt32LSB16, "ASIOSTInt32LSB16"},
			{ASIOSTInt32LSB18, "ASIOSTInt32LSB18"},
			{ASIOSTInt32LSB20, "ASIOSTInt32LSB20"},
			{ASIOSTInt32LSB24, "ASIOSTInt32LSB24"},
			{ASIOSTDSDInt8LSB1, "ASIOSTDSDInt8LSB1"},
			{ASIOSTDSDInt8MSB1, "ASIOSTDSDInt8MSB1"},
			{ASIOSTDSDInt8NER8, "ASIOSTDSDInt8NER8"},
			});
	}

	std::string GetASIOFutureSelectorString(long selector) {
		return EnumToString(selector, {
			{kAsioEnableTimeCodeRead, "EnableTimeCodeRead"},
			{kAsioDisableTimeCodeRead, "DisableTimeCodeRead"},
			{kAsioSetInputMonitor, "SetInputMonitor"},
			{kAsioTransport, "Transport"},
			{kAsioSetInputGain, "SetInputGain"},
			{kAsioGetInputMeter, "GetInputMeter"},
			{kAsioSetOutputGain, "SetOutputGain"},
			{kAsioGetOutputMeter, "GetOutputMeter"},
			{kAsioCanInputMonitor, "CanInputMonitor"},
			{kAsioCanTimeInfo, "CanTimeInfo"},
			{kAsioCanTimeCode, "CanTimeCode"},
			{kAsioCanTransport, "CanTransport"},
			{kAsioCanInputGain, "CanInputGain"},
			{kAsioCanInputMeter, "CanInputMeter"},
			{kAsioCanOutputGain, "CanOutputGain"},
			{kAsioCanOutputMeter, "CanOutputMeter"},
			{kAsioOptionalOne, "OptionalOne"},
			{kAsioSetIoFormat, "SetIoFormat"},
			{kAsioGetIoFormat, "GetIoFormat"},
			{kAsioCanDoIoFormat, "CanDoIoFormat"},
			{kAsioCanReportOverload, "CanReportOverload"},
			{kAsioGetInternalBufferSamples, "GetInternalBufferSamples"},
			});
	}

	std::string GetASIOMessageSelectorString(long selector) {
		return EnumToString(selector, {
			{kAsioSelectorSupported, "kAsioSelectorSupported"},
			{kAsioEngineVersion, "kAsioEngineVersion"},
			{kAsioResetRequest, "kAsioResetRequest"},
			{kAsioBufferSizeChange, "kAsioBufferSizeChange"},
			{kAsioResyncRequest, "kAsioResyncRequest"},
			{kAsioLatenciesChanged, "kAsioLatenciesChanged"},
			{kAsioSupportsTimeInfo, "kAsioSupportsTimeInfo"},
			{kAsioSupportsTimeCode, "kAsioSupportsTimeCode"},
			{kAsioMMCCommand, "kAsioMMCCommand"},
			{kAsioSupportsInputMonitor, "kAsioSupportsInputMonitor"},
			{kAsioSupportsInputGain, "kAsioSupportsInputGain"},
			{kAsioSupportsInputMeter, "kAsioSupportsInputMeter"},
			{kAsioSupportsOutputGain, "kAsioSupportsOutputGain"},
			{kAsioSupportsOutputMeter, "kAsioSupportsOutputMeter"},
			{kAsioOverload, "kAsioOverload"},
			});
	}

	std::string GetAsioTimeInfoFlagsString(unsigned long timeInfoFlags) {
		return BitfieldToString(timeInfoFlags, {
			{kSystemTimeValid, "kSystemTimeValid"},
			{kSamplePositionValid, "kSamplePositionValid"},
			{kSampleRateValid, "kSampleRateValid"},
			{kSpeedValid, "kSpeedValid"},
			{kSampleRateChanged, "kSampleRateChanged"},
			{kClockSourceChanged, "kClockSourceChanged"},
			});
	}

	std::string GetASIOTimeCodeFlagsString(unsigned long timeCodeFlags) {
		return BitfieldToString(timeCodeFlags, {
			{kTcValid, "kTcValid"},
			{kTcRunning, "kTcRunning"},
			{kTcReverse, "kTcReverse"},
			{kTcOnspeed, "kTcOnspeed"},
			{kTcStill, "kTcStill"},
			{kTcSpeedValid, "kTcSpeedValid"},
			});
	}

	std::string DescribeASIOTimeInfo(const AsioTimeInfo& asioTimeInfo) {
		std::stringstream result;
		result << "ASIO time info with speed " << asioTimeInfo.speed << ", system time "
			<< ASIOToInt64(asioTimeInfo.systemTime) << ", sample position "
			<< ASIOToInt64(asioTimeInfo.samplePosition) << ", sample rate "
			<< asioTimeInfo.sampleRate << " Hz, flags "
			<< GetAsioTimeInfoFlagsString(asioTimeInfo.flags) << ", reserved "
			<< Join(asioTimeInfo.reserved, " ", CharAsNumber());
		return result.str();
	}

	std::string DescribeASIOTimeCode(const ASIOTimeCode& asioTimeCode) {
		std::stringstream result;
		result << "ASIO time code with speed " << asioTimeCode.speed << ", samples "
			<< ASIOToInt64(asioTimeCode.timeCodeSamples) << ", flags "
			<< GetASIOTimeCodeFlagsString(asioTimeCode.flags) << ", future "
			<< Join(asioTimeCode.future, " ", CharAsNumber());
		return result.str();
	}

	std::string DescribeASIOTime(const ASIOTime& asioTime) {
		std::stringstream result;
		result << "ASIO time with reserved " << Join(asioTime.reserved, " ") << ", time info ("
			<< DescribeASIOTimeInfo(asioTime.timeInfo) << "), time code ("
			<< DescribeASIOTimeCode(asioTime.timeCode) << ")";
		return result.str();
	}

}