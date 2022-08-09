// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.210806.1

#pragma once
#ifndef WINRT_Windows_Devices_Sms_1_H
#define WINRT_Windows_Devices_Sms_1_H
#include "winrt/impl/Windows.Devices.Sms.0.h"
WINRT_EXPORT namespace winrt::Windows::Devices::Sms
{
    struct __declspec(empty_bases) ISmsAppMessage :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsAppMessage>,
        impl::require<winrt::Windows::Devices::Sms::ISmsAppMessage, winrt::Windows::Devices::Sms::ISmsMessageBase>
    {
        ISmsAppMessage(std::nullptr_t = nullptr) noexcept {}
        ISmsAppMessage(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsBroadcastMessage :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsBroadcastMessage>,
        impl::require<winrt::Windows::Devices::Sms::ISmsBroadcastMessage, winrt::Windows::Devices::Sms::ISmsMessageBase>
    {
        ISmsBroadcastMessage(std::nullptr_t = nullptr) noexcept {}
        ISmsBroadcastMessage(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsDevice2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsDevice2>
    {
        ISmsDevice2(std::nullptr_t = nullptr) noexcept {}
        ISmsDevice2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsDevice2Statics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsDevice2Statics>
    {
        ISmsDevice2Statics(std::nullptr_t = nullptr) noexcept {}
        ISmsDevice2Statics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsFilterRule :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsFilterRule>
    {
        ISmsFilterRule(std::nullptr_t = nullptr) noexcept {}
        ISmsFilterRule(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsFilterRuleFactory :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsFilterRuleFactory>
    {
        ISmsFilterRuleFactory(std::nullptr_t = nullptr) noexcept {}
        ISmsFilterRuleFactory(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsFilterRules :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsFilterRules>
    {
        ISmsFilterRules(std::nullptr_t = nullptr) noexcept {}
        ISmsFilterRules(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsFilterRulesFactory :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsFilterRulesFactory>
    {
        ISmsFilterRulesFactory(std::nullptr_t = nullptr) noexcept {}
        ISmsFilterRulesFactory(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsMessageBase :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsMessageBase>
    {
        ISmsMessageBase(std::nullptr_t = nullptr) noexcept {}
        ISmsMessageBase(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsMessageReceivedTriggerDetails :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsMessageReceivedTriggerDetails>
    {
        ISmsMessageReceivedTriggerDetails(std::nullptr_t = nullptr) noexcept {}
        ISmsMessageReceivedTriggerDetails(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsMessageRegistration :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsMessageRegistration>
    {
        ISmsMessageRegistration(std::nullptr_t = nullptr) noexcept {}
        ISmsMessageRegistration(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsMessageRegistrationStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsMessageRegistrationStatics>
    {
        ISmsMessageRegistrationStatics(std::nullptr_t = nullptr) noexcept {}
        ISmsMessageRegistrationStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsSendMessageResult :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsSendMessageResult>
    {
        ISmsSendMessageResult(std::nullptr_t = nullptr) noexcept {}
        ISmsSendMessageResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsStatusMessage :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsStatusMessage>,
        impl::require<winrt::Windows::Devices::Sms::ISmsStatusMessage, winrt::Windows::Devices::Sms::ISmsMessageBase>
    {
        ISmsStatusMessage(std::nullptr_t = nullptr) noexcept {}
        ISmsStatusMessage(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsTextMessage2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsTextMessage2>,
        impl::require<winrt::Windows::Devices::Sms::ISmsTextMessage2, winrt::Windows::Devices::Sms::ISmsMessageBase>
    {
        ISmsTextMessage2(std::nullptr_t = nullptr) noexcept {}
        ISmsTextMessage2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsVoicemailMessage :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsVoicemailMessage>,
        impl::require<winrt::Windows::Devices::Sms::ISmsVoicemailMessage, winrt::Windows::Devices::Sms::ISmsMessageBase>
    {
        ISmsVoicemailMessage(std::nullptr_t = nullptr) noexcept {}
        ISmsVoicemailMessage(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) ISmsWapMessage :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISmsWapMessage>,
        impl::require<winrt::Windows::Devices::Sms::ISmsWapMessage, winrt::Windows::Devices::Sms::ISmsMessageBase>
    {
        ISmsWapMessage(std::nullptr_t = nullptr) noexcept {}
        ISmsWapMessage(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif
