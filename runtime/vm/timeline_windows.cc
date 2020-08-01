// Copyright (c) 2017, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#include "vm/globals.h"
#if defined(HOST_OS_WINDOWS) && defined(SUPPORT_TIMELINE)

#include "vm/timeline.h"

#include <TraceLoggingProvider.h>

#include <string>

TRACELOGGING_DEFINE_PROVIDER(g_hMyComponentProvider,
                            "FlutterEngineTraceLoggingProvider",
                            (0xea107295,
                            0x1edb,
                            0x45e8,
                            0xb8,
                            0xa5,
                            0x7c,
                            0xb7,
                            0xca,
                            0xb6,
                            0x50,
                            0xe1));

namespace dart {

TimelineEventWindowsRecorder::TimelineEventWindowsRecorder()
    : TimelineEventPlatformRecorder() {
  TraceLoggingRegister(g_hMyComponentProvider);
}

TimelineEventWindowsRecorder::~TimelineEventWindowsRecorder() {
  TraceLoggingUnregister(g_hMyComponentProvider);
}

void TimelineEventWindowsRecorder::OnEvent(TimelineEvent* event) {
  if (event == NULL) {
    return;
  }

   const char* label = event->label();

   switch (event->event_type()) {
    case TimelineEvent::kInstant: {
       TraceLoggingWrite(
           g_hMyComponentProvider,
           "Dart Instanct",
           TraceLoggingString(label)
       );
      break;
    }
    case TimelineEvent::kBegin: {
      TraceLoggingWrite(
          g_hMyComponentProvider,
          "Dart Begin",
                        TraceLoggingString(label)
      );
      break;
    }
    case TimelineEvent::kEnd: {
      TraceLoggingWrite(g_hMyComponentProvider, "Dart End",
                        TraceLoggingString(label));
      break;
    }
    case TimelineEvent::kAsyncBegin: {
      TraceLoggingWrite(g_hMyComponentProvider, "Dart Async Begin",
                        TraceLoggingString(label),
                        TraceLoggingInt64(event->AsyncId())
                          );
      break;
    }
    case TimelineEvent::kAsyncEnd: {
      TraceLoggingWrite(g_hMyComponentProvider, "Dart Async End",
                        TraceLoggingString(label),
                        TraceLoggingInt64(event->AsyncId()));
      break;
    }
    case TimelineEvent::kCounter: {
      char  buffer[64];
      buffer[0] = 0;
      const char* fmt = "%s";
      Utils::SNPrint(reinterpret_cast<char*>(buffer), sizeof(buffer), fmt,
                     event->arguments()[0].value);

      TraceLoggingWrite(g_hMyComponentProvider, "Dart Counter",
                        TraceLoggingInt64(event->AsyncId()),
                        TraceLoggingString(buffer)
      );
      break;
    }
    default:
      break;
  }

}

}  // namespace dart

#endif  // defined(HOST_OS_WINDOWS) && !defined(PRODUCT)
