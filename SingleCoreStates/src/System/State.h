#pragma once

enum MissionState {
  INIT,
  WAIT_FOR_LAUNCH,
  ASCENT,
  DESCENT,
  LANDED,
  ERROR_STATE
};

extern MissionState currentState;