//This will decode proprietary stuff and turn it into json. At the time send() is called, everything we need is in buff

void send() {
    doc.clear();
    byte i;
    switch (buff[0]) {
        case OP_BEGIN:
            doc["op"] = "begin";
            teamType = buff[1];
            aruco = ((int) buff[2] << 8) + buff[3];

            strcpy(teamName, (char *) &buff[4]);

            doc["teamName"] = teamName;
            doc["aruco"] = aruco;
            doc["teamType"] = teamType;
            break;

        case OP_MISSION:
            doc["op"] = "mission";
            doc["teamName"] = teamName;
            doc["type"] = (int) buff[1];
            strcpy(buff, (char *) &buff[2]);
            doc["message"] = buff;
            break;

        case OP_PRINT:
            doc["op"] = "print";
            doc["teamName"] = teamName;
            strcpy(buff, (char *) &buff[1]);
            doc["message"] = buff;
            break;
    }
    //        Ok, now we need to send this baddy out on the websocket.
    serializeJson(doc, buff);
    client.send(buff);
}

void sendAruco() {
    if (aruco_visible) {
        float_converter_t f;
#ifdef DEBUG
        psl("Its visible.");
        put(aruco_x); put(aruco_y); putl(aruco_theta);
        f.f = aruco_x;
        for (int i = 0; i < 4; i++) {
            Serial.print(f.b[i], HEX); Serial.print(" ");
        }
        f.f = aruco_y;
        for (int i = 0; i < 4; i++) {
            Serial.print(f.b[i], HEX); Serial.print(" ");
        }
        f.f = aruco_theta;
        for (int i = 0; i < 4; i++) {
            Serial.print(f.b[i], HEX); Serial.print(" ");
        }
        pl();
#endif
        arduinoSerial.write(0x05);
        arduinoSerial.flush();
        f.f = aruco_x;
        arduinoSerial.write(f.b, 4);
        arduinoSerial.flush();
        f.f = aruco_y;
        arduinoSerial.write(f.b, 4);
        arduinoSerial.flush();
        f.f = aruco_theta;
        arduinoSerial.write(f.b, 4);
        arduinoSerial.flush();
    }
    else {
#ifdef DEBUG
        psl("Its not visible...");
#endif
        arduinoSerial.write(0x09);
    }
}

void onMessageCallback(WebsocketsMessage message) {
#ifdef DEBUG
    //    psl("Got message!");
    //    putl(message.data());
#endif
    StaticJsonDocument<300> doc;
    deserializeJson(doc, message.data());
    if (strcmp(doc["op"], "aruco") == 0) {
        aruco_visible = doc["aruco"]["visible"];
        aruco_x = doc["aruco"]["x"];
        aruco_y = doc["aruco"]["y"];
        aruco_theta = doc["aruco"]["theta"];
        newData = true;
    }
    else if (strcmp(doc["op"], "info") == 0) {
            const float x = 0.55, theta = 0;
            float y;
            if (strcmp(doc["mission_loc"], "bottom") == 0) //mission location is on bottom
                y = 0.55;
            else
                y = 1.45;
            float_converter_t f;
            arduinoSerial.write(0x05);
            arduinoSerial.flush();
            f.f = 0.55; //x
            arduinoSerial.write(f.b, 4);
            arduinoSerial.flush();
            f.f = strcmp(doc["mission_loc"], "bottom") == 0 ? 0.55 : 1.45; //y
            arduinoSerial.write(f.b, 4);
            arduinoSerial.flush();
            f.f = 0; //theta is 0
            arduinoSerial.write(f.b, 4);
            arduinoSerial.flush();
    }
    else if (strcmp(doc["op"], "aruco_confirm") == 0) {
        arucoConfirmed = true;
#ifdef DEBUG
        psl("Aruco Confirmed...");
#endif
    }
}
