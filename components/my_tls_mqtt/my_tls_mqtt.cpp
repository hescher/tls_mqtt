#include "my_tls_mqtt.h"

namespace my_tls_mqtt {

void MyTLSMQTTClient::set_host(const std::string &host, int port) {
  this->broker_host = host;
  this->broker_port = port;
}

void MyTLSMQTTClient::set_username(const std::string &username) {
  this->username = username;
}

void MyTLSMQTTClient::set_password(const std::string &password) {
  this->password = password;
}

void MyTLSMQTTClient::setup() {
  static BearSSL::X509List cert(root_ca);
  wifi_client.setTrustAnchors(&cert);

  mqtt_client.setClient(wifi_client);
  mqtt_client.setServer(broker_host.c_str(), broker_port);
}

void MyTLSMQTTClient::loop() {
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop();
}

void MyTLSMQTTClient::reconnect() {
  while (!mqtt_client.connected()) {
    if (mqtt_client.connect("ESP_TLS_Client", username.c_str(), password.c_str())) {
      esphome::ESP_LOGD("my_tls_mqtt", "Connected to MQTT Broker: %s", broker_host.c_str());
    } else {
      delay(5000);
    }
  }
}

}  // namespace my_tls_mqtt
