const char* ssid_1 = "ALSW_ESTUDIO";
const char* password_1 = "Fullpower7##";
const char* ssid_2 = "Subcribanse";
const char* password_2 = "alswnet";

const char* BrokerMQTT = "192.168.50.200";
#define CantidadTopic 4
const char* TopicMQTT[CantidadTopic] = {
  "alsw/audio_obs/#", "alsw/monitor_esp/obs", "alsw/control/ryuk", "bmo/#"
};
const char* nombre = "BMO_OBS";
