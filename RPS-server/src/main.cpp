#include <Arduino.h>
#include <vector>
#include <algorithm>
#include <map>
#include "../.pio/libdeps/d1_wroom_02/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

// get result of game
String chooseWinner(String p1, String p2);
// array with most popular opponent choices (last 6 games)
std::vector<String> aiMemory = { "1", "2", "3", "1", "2", "3" }; //, "1", "2", "3" };
// function for AI choice based on its memory
String smartChoiceAI(const std::vector<String>& vec);

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
      // smart choice by AI
      String ai1Choice = smartChoiceAI(aiMemory);

      String winner = chooseWinner(p1Val, ai1Choice);

      // update AI memory after game
      aiMemory.pop_back();
      aiMemory.insert(aiMemory.begin(), p1Val);

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
      // random choice by AI1
      String ai1Choice = String(random(1, 4));
      // smart choice by AI2
      String ai2Choice = smartChoiceAI(aiMemory);

      String winner = chooseWinner(ai1Choice, ai2Choice);

      // update AI memory after game
      aiMemory.pop_back();
      aiMemory.insert(aiMemory.begin(), p1Val);

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

String smartChoiceAI(const std::vector<String>& vec) {
  std::map<String, int> frequency;
  for (String val : vec)
    frequency[val]++;
  
  String mostPopularChoice = std::max_element(frequency.begin(), frequency.end(),
    [](const auto& a, const auto& b) { return a.second < b.second; })->first;

  if (mostPopularChoice == "1")
    return "2";
  else if (mostPopularChoice == "2")
    return "3";
  else
    return "1";
}
