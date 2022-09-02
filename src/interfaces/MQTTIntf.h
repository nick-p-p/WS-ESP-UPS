#pragma once


#include <PageProperties.h>

#include <MyNetwork.h>

#include <PubSubClient.h>

//#include "Globals.h"
// #include "core/messagebus/Bus.h"
// #include "MQTTBufferEntry.h"
#include "ArduinoJson.h"
#include <TZManager.h>

// #include "SerialMessage.h"

// file for config
#define MQTT_FILENAME "/mqttconfig.json"

#define SERVER_NAME_LEN 20
#define TOPIC_NAME_LEN 60
#define MAX_MESSAGE_COUNT 10

#define MQTT_INTERFACE_UID  303

#define PHRASE_TOPICS_REQUIRED  0
#define PHRASE_SERVER_REQUIRED  1

class MQTTIntf :   PageHandler
{

public:
    enum MQTTMode
    {
        // basically not in use
        inactive,
        // send from myHome to mqtt server
        sendOnly,
        // receive from mqtt server into myhome
        receiveOnly,
        // send and receive
        sendReceive
    };

    MQTTIntf(PubSubClient *mqttClient);

    void begin();
    void beginReceive();


    /**
     * @brief consumers should wait for this call to start publishing so all consumers can be connected first
     */
    virtual void startPublishing();

    /**
     * @brief override to respond to a system message that is a broadcast
     */
    // virtual bool onBroadcastMessage(EventMessage *eventMessage);


        /**
         * send a message (async)  to the broker if mode is sendOnly or sendReceive otherwise ignore
         */
        void send(char  *eventMessage);

    /**
     * check the connection state, reconnect if necessary and loop the connection for new messages
     */
    void loop();

    /**
     * suspend activities - fro closedown
     */
    void pause();

    /**
     * Get a pointer to the singleton instance
     */
    static MQTTIntf *Instance();

    /**
     * @brief unique ID for this handler. it is used to match against the "UID" parameter
     * of any incoming request
     */
    // virtual int uniqueID();

    /**
     * @brief optional, return data to update the page
     * called when action = "data"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */
    virtual void webPageData(AsyncWebServerRequest *request);

    /**
     * @brief optional, save data sent from the page
     * called when action = "save"
     * return a json array of name value pairs, where name is an id of an object and value is what to set it to
     */
    virtual void savePageData(AsyncWebServerRequest *request);

private:
    /**
     * the client we use
     */
    PubSubClient *mqttClient;

    /**
     * Mode the handler is in now
     */
    static MQTTMode mode;

    /**
     * pointer to singleton instance
     */
    static MQTTIntf *instance;

    /**
     * callback for when a message is received on the inbound topic
     */
    static void callback(char *topic, byte *message, unsigned int length);

    // /**
    //  * build a web page for editing configuration
    //  */
    // bool buildEditPage(WebServer *server, PageBuilder *pageBuilder, const char *errorMsg);
    // /**
    //  * respond to the edit page by validating and saving config
    //  */
    // bool buildSavePage(WebServer *server, PageBuilder *pageBuilder);
    // /**
    //  * helper for the web pages to add options into a select
    //  */
    // void appendListOption(PageBuilder *pageBuilder, const char *option, MQTTMode optMode);
    // static EventMessageHandler *handler;

    /**
     * try and connect if disconnected
     */
    void reconnect();

    /**
     * ring buffer of messsages to send
     */
    // static RingBuffer<MQTTBufferEntry, MAX_MESSAGE_COUNT> ringBuffer;
    // MQTTBufferEntry ringBuffer[MAX_MESSAGE_COUNT];

    /**
     * send a message to mqtt from the ring buffer
     */
    void sendMessage(char *msg);

    /**
     * send the next waiting message (if any) otehrwise sleep a bit
     */
    // void processOutboundOrSleep();

    /**
     * configure the mqtt server, topics and callback
     */
    void mqttBegin();

    /**
     * load configuration data
     */
    bool load();
    /**
     * save configuration data
     */
    void save();

    /**
     * textual mode
     */
    const char *modeText();

    bool hasConfig = false;
    char mqttServer[SERVER_NAME_LEN + 1] = "";
    char inputTopic[TOPIC_NAME_LEN + 1] = "";
    char outputTopic[TOPIC_NAME_LEN + 1] = "";
    char clientId[30];

    bool paused = false;
};
// #endif