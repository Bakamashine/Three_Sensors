#pragma once

// delta (burner hysteresis) bounds
#define MIN_DELTA 0
#define MAX_DELTA 50

#define DEFAULT_USER_TEMP 50
#define DEFAULT_HYSTERESIS 10

#define MAX_PERMITTED_TEMP 110
#define MIN_PERMITTED_TEMP -10


/// MAX_VALUES
#define MAX_PERMITTED_TEMP 110
#define MIN_PERMITTED_TEMP -10

/// periods
#define ERROR_PERIOD 10000
#define SETTINGS_CLICK_PERIOD 1000 // 1 sec
#define BUTTON_PERIOD 400          // 0.4 sec
#define DEBUG_PERIOD 500           // ms between debug log lines
