/*
 * JAWSWebUI:
 *    Implements the WebUI additions for JAWS
 *
 */

#ifndef JAWSWebUI_h
#define JAWSWebUI_h

#include <WebUI.h>

namespace JAWSWebUI {
  // ----- Setup functions
  void init();

  namespace Handlers {
    void takeReadings();
    void displayHomePage();
    void handleUpdateJAWSConfig();
    void handleJAWSConfigure();
  }
}

#endif  // JAWSWebUI_h