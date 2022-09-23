// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#include "ThinkingAnalytics.h"
#include "Core.h"
#include "Json.h"
#include <string>
#include "ISettingsModule.h"
#include "AnalyticsEventAttribute.h"

#include "ThinkingAnalyticsSettings.h"
#include "Interfaces/IPluginManager.h"
#include "Common/TAConstants.h"

#if PLATFORM_ANDROID
#include "./Android/ThinkingAnalyticsJNI.h"
#elif PLATFORM_IOS
#include "./IOS/ThinkingAnalyticsCpp.h"
#elif PLATFORM_MAC || PLATFORM_WINDOWS
#include "./PC/ThinkingAnalyticsPC.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(ThinkingAnalytics, Display, All);

UThinkingAnalytics::UThinkingAnalytics(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UThinkingAnalytics::Initialize() 
{
    auto  &manager = IPluginManager::Get();
    auto  plugin = manager.FindPlugin("ThinkingAnalytics");

    if ( !plugin.IsValid() )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("ThinkingAnalytics is not correctly loaded"));
        return;
    }

    const UThinkingAnalyticsSettings *defaultSettings = GetDefault<UThinkingAnalyticsSettings>();
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_initialize(defaultSettings, plugin->GetDescriptor().VersionName);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_initialize(defaultSettings, plugin->GetDescriptor().VersionName);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC::Initialize(defaultSettings, plugin->GetDescriptor().VersionName);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling Initialize... The platform is not supported. %d"), defaultSettings->Mode);
#endif
}

void UThinkingAnalytics::InitializeInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone) 
{

    auto  &manager = IPluginManager::Get();
    auto  plugin = manager.FindPlugin("ThinkingAnalytics");

    if ( !plugin.IsValid() )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("ThinkingAnalytics is not correctly loaded"));
        return;
    }
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_initializeInstance(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_initialize(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC::Initialize(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling Initialize... The platform is not supported. %d"), mode);
#endif
}

void UThinkingAnalytics::InitializeEncryptInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone, bool bEnableEncrypt, const FString& EncryptPublicKey, int EncryptVersion, const FString& SymmetricEncryption, const FString& AsymmetricEncryption)
{
    auto  &manager = IPluginManager::Get();
    auto  plugin = manager.FindPlugin("ThinkingAnalytics");

    if ( !plugin.IsValid() )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("ThinkingAnalytics is not correctly loaded"));
        return;
    }
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_initializeEncryptInstance(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName, bEnableEncrypt, EncryptPublicKey, EncryptVersion, SymmetricEncryption, AsymmetricEncryption);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_initializeEncryptInstance(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName, EncryptPublicKey, EncryptVersion, SymmetricEncryption, AsymmetricEncryption);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC::Initialize(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling Initialize... The platform is not supported. %d"), mode);
#endif
}

void UThinkingAnalytics::EnableAutoTrack(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enable_autotrack(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling UThinkingAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UThinkingAnalytics::EnableAutoTrackWithType(const TArray<FString>& EventTypeList, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack_with_type(AppId, EventTypeList);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enable_autotrack_with_type(AppId, EventTypeList);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling UThinkingAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UThinkingAnalytics::EnableAutoTrackWithTypeAndProperties(const TArray<FString>& EventTypeList, const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack_with_type_and_prop(AppId, EventTypeList, Properties);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enable_autotrack_with_type_and_prop(AppId, EventTypeList, Properties);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling UThinkingAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UThinkingAnalytics::EnableAutoTrackWithTypeAndProperties(const TArray<FString>& EventTypeList, TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack_with_type_and_prop(AppId, EventTypeList, PropertiesStr);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enable_autotrack_with_type_and_prop(AppId, EventTypeList, PropertiesStr);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling UThinkingAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UThinkingAnalytics::Track(const FString& EventName, const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->Track(EventName, Properties, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Track"));
#endif
}

void UThinkingAnalytics::Track(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track(EventName, PropertiesStr, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track(EventName, PropertiesStr, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->Track(EventName, PropertiesStr, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Track"));
#endif
}

void UThinkingAnalytics::TrackFirst(const FString& EventName, const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_first(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackFirst(EventName, Properties, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUnique"));
#endif
}

void UThinkingAnalytics::TrackFirst(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first(EventName, PropertiesStr, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_first(EventName, PropertiesStr, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackFirst(EventName, PropertiesStr, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUnique"));
#endif
}

void UThinkingAnalytics::TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first_withId(EventName, Properties, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_first_withId(EventName, Properties, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackFirstWithId(EventName, Properties, FirstCheckId, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUniqueWithId"));
#endif
}

void UThinkingAnalytics::TrackFirstWithId(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& FirstCheckId, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first_withId(EventName, PropertiesStr, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_first_withId(EventName, PropertiesStr, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackFirstWithId(EventName, PropertiesStr, FirstCheckId, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUniqueWithId"));
#endif
}

void UThinkingAnalytics::TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_update(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_update(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackUpdate(EventName, Properties, EventId, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUpdate"));
#endif
}

void UThinkingAnalytics::TrackUpdate(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& EventId, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_update(EventName, PropertiesStr, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_update(EventName, PropertiesStr, EventId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackUpdate(EventName, PropertiesStr, EventId, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUpdate"));
#endif
}

void UThinkingAnalytics::TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_overwrite(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_overwrite(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackOverwrite(EventName, Properties, EventId, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackOverwrite"));
#endif
}

void UThinkingAnalytics::TrackOverwrite(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& EventId, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_overwrite(EventName, PropertiesStr, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_overwrite(EventName, PropertiesStr, EventId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UThinkingAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackOverwrite(EventName, PropertiesStr, EventId, Dyldproperties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackOverwrite"));
#endif
}

void UThinkingAnalytics::TimeEvent(const FString& EventName, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_time_event(EventName, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_time_event(EventName, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TimeEvent"));
#endif
}

void UThinkingAnalytics::UserSet(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set(Properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_set(Properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserSet(Properties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserSet"));
#endif
}

void UThinkingAnalytics::UserSet(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set(PropertiesStr, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_set(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserSet(PropertiesStr); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserSet"));
#endif
}

void UThinkingAnalytics::UserSetOnce(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set_once(Properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_set_once(Properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserSetOnce(Properties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserSetOnce"));
#endif
}

void UThinkingAnalytics::UserSetOnce(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set_once(PropertiesStr, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_set_once(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserSetOnce(PropertiesStr); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserSetOnce"));
#endif
}

void UThinkingAnalytics::UserAdd(const FString& Property, const float Value, const FString& AppId)
{
    FString outStr;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&outStr);
    /** Write JSON message */
    JsonWriter->WriteObjectStart();
    JsonWriter->WriteValue(Property, Value);
    JsonWriter->WriteObjectEnd();
    JsonWriter->Close();
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_add(outStr, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_add(outStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserAdd(outStr); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserAdd"));
#endif
}

void UThinkingAnalytics::UserUnset(const FString& Property, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_unset(Property, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_unset(Property, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserUnset(Property); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserUnset"));
#endif
}

void UThinkingAnalytics::UserAppend(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_append(Properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_append(Properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserAppend(Properties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserAppend"));
#endif
}

void UThinkingAnalytics::UserAppend(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_append(PropertiesStr, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_append(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserAppend(PropertiesStr); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserAppend"));
#endif
}

void UThinkingAnalytics::UserUniqueAppend(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_unique_append(Properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_unique_append(Properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserUniqueAppend(Properties); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserUniqueAppend"));
#endif
}

void UThinkingAnalytics::UserUniqueAppend(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_unique_append(PropertiesStr, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_unique_append(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserUniqueAppend(PropertiesStr); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserUniqueAppend"));
#endif
}

void UThinkingAnalytics::UserDelete(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_delete(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_delete(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserDelete(); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserDelete"));
#endif
}

void UThinkingAnalytics::Identify(const FString& DistinctId, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_identify(DistinctId, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_identify(DistinctId, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_Identify(DistinctId); 
    }    
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Identify"));
#endif
}

void UThinkingAnalytics::Login(const FString& AccountId, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_login(AccountId, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_login(AccountId, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {  
        Instance->ta_Login(AccountId); 
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Login"));
#endif
}

void UThinkingAnalytics::Logout(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_logout(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_logout(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_Logout(); 
    }    
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Logout"));
#endif
}

void UThinkingAnalytics::Flush(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_flush(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_flush(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_Flush();
    }
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Flush"));
#endif
}

void UThinkingAnalytics::CalibrateTime(int64 timestamp)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_calibrate_time(timestamp);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_calibrate_time(timestamp);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::CalibrateTime"));
#endif
}

void UThinkingAnalytics::CalibrateTimeWithNtp(const FString& urlString)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_calibrate_time_ntp(urlString);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_calibrate_time_ntp(urlString);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::calibrateTimeWithNtp"));
#endif
}

void UThinkingAnalytics::OptOutTracking(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_opt_out_tracking(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_opt_out_tracking(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::OptOutTracking"));
#endif
}

void UThinkingAnalytics::OptInTracking(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_opt_in_tracking(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_opt_in_tracking(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::OptInTracking"));
#endif
}

void UThinkingAnalytics::EnableTracking(bool bIsEnable, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_tracking(bIsEnable, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enable_tracking(bIsEnable, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->EnableTracking(bIsEnable); 
    }    
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::EnableTracking"));
#endif
}

FString UThinkingAnalytics::GetDeviceId(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_device_id(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_get_device_id(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
        return FString(""); 
    }
    else
    {
        return Instance->ta_GetDeviceID(); 
    }    
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::GetDeviceId"));
    return FString("");
 #endif
    
}

FString UThinkingAnalytics::GetDistinctId(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_distinct_id(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_get_distinct_id(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
        return FString(""); 
    }
    else
    {
        return Instance->ta_GetDistinctID(); 
    }     
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::GetDistinctId"));
    return FString("");
 #endif
}

void UThinkingAnalytics::SetSuperProperties(const FString& properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_set_superProperties(properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_set_superProperties(properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_SetSuperProperties(properties); 
    }
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::SetSuperProperties"));

 #endif
}

void UThinkingAnalytics::SetSuperProperties(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_set_superProperties(PropertiesStr, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_set_superProperties(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_SetSuperProperties(PropertiesStr); 
    }
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::SetSuperProperties"));

 #endif
}

void UThinkingAnalytics::SetTrackStatus(const FString& Status, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_set_trackStatus(Status, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_set_trackStatus(Status, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_SetTrackState(Status);
    }
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::SetTrackStatus"));

 #endif
}

FString UThinkingAnalytics::GetSuperProperties(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_superProperties(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_get_superProperties(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
        return FString("");
    }
    else
    {
        return Instance->ta_GetSuperProperties();
    }       
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::GetSuperProperties"));
    return FString("");
 #endif
}

FString UThinkingAnalytics::GetPresetProperties(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_presetProperties(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_get_presetProperties(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UThinkingAnalyticsPC* Instance = UThinkingAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("There is no Instance!"));
        return FString("");
    }
    else
    {
        return Instance->ta_GetPresetProperties(); 
    }       
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::GetPresetProperties"));
    return FString("");
 #endif
}

FString UThinkingAnalytics::CreateLightInstance(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_createLightInstance(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_createLightInstance(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::CreateLightInstance"));
    return FString("");
#endif
}

void UThinkingAnalytics::EnableThirdPartySharing(const TArray<FString>& EventTypeList, const FString& AppId)
{
    int typeList = 0;
    for (int i = 0; i < EventTypeList.Num(); i++)
    {
        if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeAPPSFLYER))
        {
            typeList = typeList | (1 << 0);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeIRONSOURCE))
        {
            typeList = typeList | (1 << 1);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeADJUST))
        {
            typeList = typeList | (1 << 2);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeBRANCH))
        {
            typeList = typeList | (1 << 3);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTOPON))
        {
            typeList = typeList | (1 << 4);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTRACKING))
        {
            typeList = typeList | (1 << 5);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTRADPLUS))
        {
            typeList = typeList | (1 << 6);
        }
    }
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enableThirdPartySharing(typeList, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enableThirdPartySharing(typeList, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::EnableThirdPartySharing"));
#endif 
}

void UThinkingAnalytics::EnableThirdPartySharingWithCustomProperties(const TArray<FString>& EventTypeList, TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
int typeList = 0;
    for (int i = 0; i < EventTypeList.Num(); i++)
    {
        if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeAPPSFLYER))
        {
            typeList = typeList | (1 << 0);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeIRONSOURCE))
        {
            typeList = typeList | (1 << 1);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeADJUST))
        {
            typeList = typeList | (1 << 2);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeBRANCH))
        {
            typeList = typeList | (1 << 3);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTOPON))
        {
            typeList = typeList | (1 << 4);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTRACKING))
        {
            typeList = typeList | (1 << 5);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTRADPLUS))
        {
            typeList = typeList | (1 << 6);
        }
    }

    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enableThirdPartySharingWithCustomProperties(typeList, PropertiesStr, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enableThirdPartySharingWithCustomProperties(typeList, PropertiesStr, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::EnableThirdPartySharing"));
#endif
}

FString UThinkingAnalytics::GetDynamicProperties(const FString& AppId)
{
    FString jsonString = "";
    TADynamicSuperPropRetValDelegate* delegate = TADynamicSuperPropMethods.Find(*AppId);
    if(delegate != nullptr)
    {
        jsonString = delegate->Execute();
    }
    return jsonString;
}

void UThinkingAnalytics::SetDynamicProperties(TADynamicSuperPropRetValDelegate Del, const FString& AppId)
{
    TADynamicSuperPropMethods.Emplace(*AppId, Del);
}

void UThinkingAnalytics::TASetAutoTrackEventListener(TAAutoTrackEventRetValDelegate Del, const TArray<FString>& EventTypeList, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_setAutoTrackEventListener(Del, EventTypeList, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_setAutoTrackEventListener(Del, EventTypeList, AppId);
#else
    UE_LOG(LogTemp, Warning, TEXT("Calling UThinkingAnalytics::EnableAutoTrack... The platform is not supported."));
#endif   
}