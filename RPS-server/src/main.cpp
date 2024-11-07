#include <Arduino.h>
#include <vector>
#include <algorithm>
#include <map>
#include "../.pio/libdeps/d1_wroom_02/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

// addition parameters from my neural network
std::vector<int> aiMemory = { 2, 1, 3 };
float inputWeights[3][8] = {
    { 0.237058, -0.290494, 0.274185, 0.002906, -0.130077, 0.604973, 0.687958, 0.113789 },
    { 0.486323, -0.628314, 0.194522, 0.172590, 0.763265, -0.167007, 1.251667, 0.067055 },
    { 0.120458, -0.396984, 1.712663, 1.075813, 0.168848, -0.612591, -1.263713, 1.474092 },
};
float hiddenWeights[8][3] = {
    { 0.469825, -0.215859, -0.548794 },
    { 0.285107, -0.448687, -0.552901 },
    { 0.656990, -1.276879, 0.272193 },
    { 1.293255, -1.585455, -1.156214 },
    { -0.498069, 0.174363, 0.186442 },
    { -0.631017, 1.110896, 0.074588 },
    { -0.578895, 0.978867, 0.364652 },
    { 1.034574, -1.109686, 0.851510 },
};
float hiddenBias[8] = {-0.376692, 0.000000, -0.370774, -0.863258, 0.907450, 0.770704, 1.312167, -0.110566};
float outputBias[3] = {-0.725252, 0.632030, 0.405089};

// addition functions for working
// with data from my neural network
float Relu(float x);
void SoftMax(float x[], int size);
String SmartChoiceAI();
// get result of game
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
      // smart choice by AI
      String ai1Choice = SmartChoiceAI();

      String winner = chooseWinner(p1Val, ai1Choice);

      // update AI memory after game
      aiMemory.pop_back();
      aiMemory.insert(aiMemory.begin(), p1Val.toInt());

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
      String ai2Choice = SmartChoiceAI();

      String winner = chooseWinner(ai1Choice, ai2Choice);

      // update AI memory after game
      aiMemory.pop_back();
      aiMemory.insert(aiMemory.begin(), ai1Choice.toInt());

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

float Relu(float x)
{
    return x > 0 ? x : 0;
}

void SoftMax(float x[], int size)
{
    float max_val = x[0];
    for (int i = 1; i < size; i++)
        if (x[i] > max_val)
            max_val = x[i];
    float sum = 0;
    for (int i = 0; i < size; i++)
        sum += exp(x[i] - max_val);
    for (int i = 0; i < size; i++)
        x[i] = exp(x[i] - max_val) / sum;
}

String SmartChoiceAI()
{
    float output[3];

    float hiddenLayer[8];
    for (int i = 0; i < 8; i++)
    {
        hiddenLayer[i] = hiddenBias[i];
        for (int j = 0; j < 3; j++)
        {
            hiddenLayer[i] += aiMemory[j] * inputWeights[j][i];
        }
        hiddenLayer[i] = Relu(hiddenLayer[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        output[i] = outputBias[i];
        for (int j = 0; j < 8; j++)
        {
            output[i] += hiddenLayer[j] * hiddenWeights[j][i];
        }
    }

    SoftMax(output, 3);

    int best_move = 0;
    if (output[1] > output[best_move])
        best_move = 1;
    if (output[2] > output[best_move])
        best_move = 2;

    return String(best_move + 1);
}