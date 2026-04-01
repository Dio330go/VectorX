#include "Mission.h"
#include "State.h"
#include "Data.h"

MissionState currentState = INIT;

void setupMission() {
  mission.bootTime = millis();
  currentState = INIT;
  initDevices();
}

void runMissionState() {
  switch (currentState) {
    case INIT:
      handleInit();
      break;
    case WAIT_FOR_LAUNCH:
      handleWaitForLaunch();
      break;
    case ASCENT:
      handleAscent();
      break;
    case DESCENT:
      handleDescent();
      break;
    case LANDED:
      handleLanded();
      break;
    case ERROR_STATE:
      handleError();
      break;
  }
}