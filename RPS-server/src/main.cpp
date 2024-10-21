#include <Arduino.h>
#include <../.pio/libdeps/d1_wroom_02/tinyxml2/tinyxml2.h>

using namespace tinyxml2;

void setup() {
  Serial.begin(9600);
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

    // find necessary tag and modify its value
    XMLElement* configsElement = doc.FirstChildElement("user");
    configsElement = configsElement->FirstChildElement("configs");
    if (configsElement) {
      String currentText = configsElement->GetText();
      currentText += " is modified";
      configsElement->SetText(currentText.c_str());
    }

    // convert xml file to string
    XMLPrinter printer;
    doc.Print(&printer);
    String modifiedXml = String(printer.CStr());
    modifiedXml.replace("\n", "");
    //modifiedXml.replace("\t", "");

    Serial.println(modifiedXml);
  }
}







  // //while (Serial.available()) {
  // String xmlInput = Serial.readString();

  // // parse "xml file"
  // XMLDocument doc;
  // XMLError result = doc.Parse(xmlInput.c_str());
  // if (result != XML_SUCCESS) {
  //   return;
  // }

  // // find necessary tag and modify its value
  // XMLElement* configsElement = doc.FirstChildElement("user");
  // configsElement = configsElement->FirstChildElement("configs");
  // if (configsElement) {
  //   String currentText = configsElement->GetText();
  //   currentText += " is modified";
  //   configsElement->SetText(currentText.c_str());
  // }

  // // convert xml file to string
  // XMLPrinter printer;
  // doc.Print(&printer);
  // String modifiedXml = String(printer.CStr());

  // Serial.println(xmlInput);
  // xmlInput = "";


  //}
  // get full message
  // while (Serial.available()) {
  //   char c = Serial.read();
  //   xmlInput += c;
  //   if (c == '\0')
  //     break;
  // }

  // // parse "xml file"
  // XMLDocument doc;
  // XMLError result = doc.Parse(xmlInput.c_str());
  // if (result != XML_SUCCESS) {
  //   return;
  // }

  // // find necessary tag and modify its value
  // XMLElement* configsElement = doc.FirstChildElement("user");
  // configsElement = configsElement->FirstChildElement("configs");
  // if (configsElement) {
  //   String currentText = configsElement->GetText();
  //   currentText += " is modified";
  //   configsElement->SetText(currentText.c_str());
  // }

  // // convert xml file to string
  // XMLPrinter printer;
  // doc.Print(&printer);
  // String modifiedXml = String(printer.CStr());

  // send and clear buffer for new message

  // Serial.println("<user><configs>ki404</configs></user>");
  // xmlInput = "";
  
  // Serial.println(modifiedXml); // "<user><configs>shabaka</configs></user>"
  // delay(1);
