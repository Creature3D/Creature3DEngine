// ControlRecord.cpp

#include <Plugin flt\flt.h>
#include <Plugin flt\Registry.h>
#include <Plugin flt\ControlRecord.h>

using namespace flt;

RegisterRecordProxy<PushLevelRecord>    g_PushLevelProxy;
RegisterRecordProxy<PopLevelRecord>     g_PopLevelProxy;

RegisterRecordProxy<PushSubfaceRecord>  g_PushSubfaceProxy;
RegisterRecordProxy<PopSubfaceRecord>   g_PopSubfaceProxy;

RegisterRecordProxy<PushExtensionRecord> g_PushExtensionProxy;
RegisterRecordProxy<PopExtensionRecord>  g_PopExtensionProxy;
