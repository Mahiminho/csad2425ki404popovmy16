#include <Arduino.h>
#include "../.pio/libdeps/d1_wroom_02/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

String chooseWinner(String p1, String p2);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');

    // parse "xml file"
    XMLDocument doc;
    XMLError result = doc.Parse(message.c_str());
    if (result != XML_SUCCESS) {
      return;
    }

    // find necessary tags
    XMLElement* user = doc.FirstChildElement("user");

    // get mode
    XMLElement* mode = user->FirstChildElement("mode");
    String modeVal = mode->GetText();

    // get p1
    XMLElement* p1 = user->FirstChildElement("p1");
    String p1Val = p1->GetText();

    // get p2
    XMLElement* p2 = user->FirstChildElement("p2");
    String p2Val = p2->GetText();

    // tags for setting
    XMLElement* ai1 = user->FirstChildElement("ai1");
    XMLElement* ai2 = user->FirstChildElement("ai2");
    XMLElement* win = user->FirstChildElement("win");

    // process
    if (modeVal == "1") {
      String ai1Choice = String(random(1, 4));

      String winner = chooseWinner(p1Val, ai1Choice);

      ai1->SetText(ai1Choice.c_str());
      win->SetText(winner.c_str());
    } 
    else if (modeVal == "2") {
      String winner = chooseWinner(p1Val, p2Val);
      win->SetText(winner.c_str());
    } 
    else if (modeVal == "3") {
      String ai1Choice = String(random(1, 4));
      String ai2Choice = String(random(1, 4));

      String winner = chooseWinner(ai1Choice, ai2Choice);

      ai1->SetText(ai1Choice.c_str());
      ai2->SetText(ai2Choice.c_str());
      win->SetText(winner.c_str());
    } 
    else if (modeVal == "4") {
      String ai1Choice = String(random(1, 4));
      String ai2Choice = "";
      if (ai1Choice == "1")
        ai2Choice = "2";
      else if (ai1Choice == "2")
        ai2Choice = "3";
      else if (ai1Choice == "3")
        ai2Choice = "1";

      String winner = chooseWinner(ai1Choice, ai2Choice);

      ai1->SetText(ai1Choice.c_str());
      ai2->SetText(ai2Choice.c_str());
      win->SetText(winner.c_str());
    }

    // convert xml file to string
    XMLPrinter printer;
    doc.Print(&printer);
    String modifiedXml = String(printer.CStr());
    modifiedXml.replace("\n", "");

    Serial.println(modifiedXml);
  }
}

String chooseWinner(String p1, String p2) {
  if (p1 == p2) {
    return "0";
  } else if ((p1 == "1" && p2 == "2") ||
              (p1 == "2" && p2 == "3") ||
              (p1 == "3" && p2 == "1")) {
    return "2";
  } else if ((p2 == "1" && p1 == "2") ||
              (p2 == "2" && p1 == "3") ||
              (p2 == "3" && p1 == "1")) {
    return "1";
  }
  return "5";
}
