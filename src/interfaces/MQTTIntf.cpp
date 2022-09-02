#include "MQTTIntf.h"

#define TOPICS_REQUIRED_MSG "If a server is specified, input and output topics cannot be blank"
#define SERVER_REQUIRED_MSG "Server is required for active modes"

MQTTIntf::MQTTMode MQTTIntf::mode = inactive;
MQTTIntf *MQTTIntf::instance = nullptr;
// EventMessageHandler *MQTTIntf::handler = nullptr;

MQTTIntf::MQTTIntf(PubSubClient *mqttClient) :  PageHandler(MQTT_INTERFACE_UID)
{
    this->mqttClient = mqttClient;
    instance = this;
}

void MQTTIntf::begin()
{

    hasConfig = load();

    if (!hasConfig)
    {
        this->mode = inactive;
        sprintf(inputTopic, "/mys/%s/in", MyNetwork::MacId());
        sprintf(outputTopic, "/mys/%s/out", MyNetwork::MacId());
        save();
    }
    sprintf(clientId, "MyS%s", MyNetwork::MacId());
    mqttBegin();

    BDWebServer::subscribe(this);
}

void MQTTIntf::beginReceive()
{
    if (mode == receiveOnly || mode == sendReceive)
    {
        mqttClient->setCallback(MQTTIntf::callback);
    }
}

void MQTTIntf::startPublishing()
{
    beginReceive();
}



void MQTTIntf::send(char  *eventMessage)
{
    if (mode == sendOnly || mode == sendReceive)
    {
        // char buffer[61];
        // snprintf(buffer, 60, "%d;%d;%d;%d;%d;%s",
        //          eventMessage->getUID(),
        //          eventMessage->getSubUID(),
        //          eventMessage->getCmd(),
        //          0,
        //          eventMessage->getSubType(),
        //          eventMessage->getString());

        sendMessage(eventMessage);
    }
}

MQTTIntf *MQTTIntf::Instance()
{
    return instance;
}

void MQTTIntf::callback(char *topic, byte *message, unsigned int length)
{
    char messageTemp[141];
    if (length > 140)
    {
        ILogger::log(l_error, "Message from topic %s, too long @ %d", topic, length);
        return;
    }

    for (int i = 0; i < length; i++)
    {
        messageTemp[i] = (char)message[i];
    }
    messageTemp[length] = '\0';

    ILogger::log(l_diagnostics, "Message arrived on topic: %s, Message %s ", topic, messageTemp);

    if (length)
    {
        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, messageTemp);
        if (error)
        {
            ILogger::log(l_error, "Failed to read/parse MQTT message %s", messageTemp);
            return;
        }

        if (mode == receiveOnly || mode == sendReceive)
        {
            // messageType msgType = (messageType)doc["msgType"].as<int>();
            // mysensors_data_t subType = (mysensors_data_t)doc["subType"].as<int>();
            // String payload = doc["payload"];
            // char buffer[81];
            // snprintf(buffer, 80, "%s", payload.c_str());
            // ILogger::log(l_debug, "<= %s", buffer);
            // EventMessage eventMessage(buffer, TZManager::now());
            // if (eventMessage.isValid())
            // {
            //     handler->handleMessage(&eventMessage);
            // }
            // MySensorGateway::decodeAndSendMySensors(payload.c_str(), TZManager::now(), msMQTT);
            // Bus::publish(msgType, msMQTT, payload.c_str(), subType, MQTT_INTERFACE_UID, 0, C_SET);
        }
    }
}

void MQTTIntf::loop()
{
    if ((mode != inactive) && !paused)
    {
        if (!mqttClient->connected())
        {
            reconnect();
        }

        if (mqttClient->connected())
        {
            mqttClient->loop();
            if (!mqttClient->connected())
            {
                ILogger::log(l_error, "Whoa mqtt disconnected after loop");
                delay(5000);
                return;
            }
        }
    }
    else
    {
        delay(500);
    }
}

void MQTTIntf::pause()
{
    paused = true;
}

void MQTTIntf::reconnect()
{
    // Loop until we're reconnected
    if (!mqttClient->connected())
    {
        ILogger::log(l_info, "Attempting MQTT connection for client %s", clientId);
        // Attempt to connect
        if (mqttClient->connect(clientId))
        {
            if (!mqttClient->connected())
            {
                ILogger::log(l_error, "Whoa mqtt not connected after succesful connext!");
                delay(5000);
                return;
            }

            ILogger::log(l_info, "connected");
            // Subscribe
            if (mode == receiveOnly || mode == sendReceive)
            {
                mqttClient->subscribe(inputTopic);
            }

            if (!mqttClient->connected())
            {
                ILogger::log(l_error, "Whoa mqtt disconnected after subscribe to %s", inputTopic);
                delay(5000);
                return;
            }
        }
        else
        {
            ILogger::log(l_error, "failed, rc = %d try again in 5 seconds", mqttClient->state());
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTTIntf::sendMessage(char *msg)
{
    mqttClient->publish(outputTopic, msg);
}

void MQTTIntf::mqttBegin()
{
    if (mode != inactive)
    {
        mqttClient->setServer(mqttServer, 1883);
    }
}

bool MQTTIntf::load()
{
    ILogger::log(l_info, "loading: %s", MQTT_FILENAME);

    File mqttFile = SPIFFS.open(MQTT_FILENAME, "r");
    if (!mqttFile)
    {
        ILogger::log(l_error, "Failed open to open MQTT file: %s", MQTT_FILENAME);
        return false;
    }

    StaticJsonDocument<256> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, mqttFile);
    if (error)
    {
        ILogger::log(l_error, "Failed to read file, using default configuration: %s", MQTT_FILENAME);
        return false;
    }

    mode = (MQTTMode)(doc["mode"].as<byte>());
    sprintf(mqttServer, "%s", doc["server"].as<String>().c_str());
    sprintf(inputTopic, "%s", doc["inputTopic"].as<String>().c_str());
    sprintf(outputTopic, "%s", doc["outputTopic"].as<String>().c_str());

    ILogger::log(l_info, "MQTT mode: %s", modeText());
    ILogger::log(l_info, "server: %s", mqttServer);
    ILogger::log(l_info, "inputTopic: %s", inputTopic);
    ILogger::log(l_info, "outputTopic: %s", outputTopic);

    mqttFile.close();

    return (strlen(mqttServer) > 0) && (strlen(inputTopic) > 0) && (strlen(outputTopic) > 0);
}

void MQTTIntf::save()
{
    // Delete existing file, otherwise the configuration is appended to the file
    SPIFFS.remove(MQTT_FILENAME);

    File mqttFile = SPIFFS.open(MQTT_FILENAME, "w");
    if (!mqttFile)
    {
        ILogger::log(l_error, "Failed to open mqtt config file for writing: %s", MQTT_FILENAME);
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<256> doc;

    // Set the values in the document
    doc["mode"] = mode;
    doc["server"] = mqttServer;
    doc["inputTopic"] = inputTopic;
    doc["outputTopic"] = outputTopic;

    // Serialize JSON to file
    if (serializeJson(doc, mqttFile) == 0)
    {
        ILogger::log(l_error, "Failed to write to mqtt config file");
    }

    mqttFile.close();
}


/**
 * @brief optional, return data to update the page
 * called when action = "data"
 * return a json array of name value pairs, where name is an id of an object and value is what to set it to
 */
void MQTTIntf::webPageData(AsyncWebServerRequest *request)
{
    PageProperties properties(200);
    properties.add("mode", mode);
    properties.add("server", mqttServer);
    properties.add("inputTopic", inputTopic);
    properties.add("outputTopic", outputTopic);
    properties.send(request);
}

/**
 * @brief optional, save data sent from the page
 * called when action = "save"
 * return a json array of name value pairs, where name is an id of an object and value is what to set it to
 */
void MQTTIntf::savePageData(AsyncWebServerRequest *request)
{
    PageProperties pageProperties(64);

    if (!request->hasArg("server") || !request->hasArg("inputTopic") || !request->hasArg("outputTopic") || !request->hasArg("mode"))
    {
        pageProperties.sendConfirmation(request, BDLanguage::phrase(SYS_PHRASE_BAD_ARGS));
        return;
    }

    String serverName = request->arg("server");
    String input = request->arg("inputTopic");
    String output = request->arg("outputTopic");
    int modeNum = request->arg("mode").toInt();

    if ((modeNum > 0) && (serverName.length() == 0))
    {
        pageProperties.sendConfirmation(request, BDLanguage::phrase(PHRASE_SERVER_REQUIRED));
        return;
    }

    if ((serverName.length() > 0) && ((input.length() == 0) || (output.length() == 0)))
    {
        pageProperties.sendConfirmation(request, BDLanguage::phrase(PHRASE_TOPICS_REQUIRED));
        return;
    }

    mode = (MQTTMode)modeNum;
    sprintf(mqttServer, serverName.c_str());
    sprintf(inputTopic, input.c_str());
    sprintf(outputTopic, output.c_str());

    save();

    mqttClient->disconnect();
    mqttBegin();
    beginReceive();

    pageProperties.sendConfirmation(request, BDLanguage::phrase(SYS_PHRASE_SAVED_SUCCESS));
}

const char *MQTTIntf::modeText()
{
    switch (mode)
    {
    case inactive:
        return "inactive";

    case sendOnly:
        return "send only";

    case receiveOnly:
        return "receive only";

    case sendReceive:
        return "send and receive";
    }
    return "invalid mode";
}