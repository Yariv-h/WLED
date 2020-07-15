# 1 "C:\\Users\\Yariv\\AppData\\Local\\Temp\\tmpmv1v_jja"
#include <Arduino.h>
# 1 "C:/Git/opensource/yariv-fork/WLED/wled00/wled00.ino"
# 13 "C:/Git/opensource/yariv-fork/WLED/wled00/wled00.ino"
#include "wled.h"
void setup();
void loop();
#line 15 "C:/Git/opensource/yariv-fork/WLED/wled00/wled00.ino"
void setup() {
  WLED::instance().setup();
}

void loop() {
  WLED::instance().loop();
}