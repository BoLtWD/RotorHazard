#include <ArduinoUnitTests.h>
#include <Godmode.h>
#include "../rssi.h"
#include "util.h"

/**
 * Crossing that gradually raises and falls.
 */
unittest(gradualCrossing) {
  GodmodeState* nano = GODMODE();
  nano->reset();
  rssiInit();

  state.activatedFlag = true;

  // prime the state with some background signal
  sendSignal(nano, 50);
  sendSignal(nano, 50);
  assertTrue(rssiStateValid());

  // enter
  for(int signal = 50; signal<130; signal++) {
      sendSignal(nano, signal);
      int expected = signal;
      assertEqual(expected, (int)state.rssi);
      assertEqual(expected, (int)state.nodeRssiPeak);
  }
  sendSignal(nano, 130);
  assertEqual(130, (int)state.nodeRssiPeak);
  assertEqual(50, (int)state.nodeRssiNadir);
  assertTrue(state.crossing);
  assertEqual(130, (int)state.passPeak.rssi);

  // exit
  for(int signal = 130; signal>70; signal--) {
      sendSignal(nano, signal);
      int expected = signal;
      assertEqual(expected, (int)state.rssi);
  }
  sendSignal(nano, 70);
  assertEqual(70, (int)state.rssi);
  assertEqual(130, (int)state.nodeRssiPeak);

  assertFalse(state.crossing);
  assertEqual(time(2)-1, (int)state.passPeak.duration);
  assertEqual(70, (int)state.passRssiNadir);

  assertEqual(130, (int)history.peak.rssi);
  assertEqual(time(2)-1, (int)history.peak.duration);
  assertEqual(70, (int)history.nadir.rssi);
  assertEqual(0, (int)history.nadir.duration);

  assertTrue(isPeakValid(history.peakSend));
  assertEqual(130, (int)history.peakSend.rssi);
  assertEqual(time(2)-1, (int)history.peakSend.duration);
  assertEqual(0, (int)history.nadirSend.rssi);

  assertEqual(130, (int)state.nodeRssiPeak);
  assertEqual(50, (int)state.nodeRssiNadir);

  assertFalse(isPeakValid(state.passPeak));
  assertEqual(70, (int)state.passRssiNadir);

  assertEqual(130, (int)lastPass.rssiPeak);
  assertEqual(50, (int)lastPass.rssiNadir);
  assertEqual(1, (int)lastPass.lap);
}

unittest_main()
