const char* ssid_1     = "123";
const char* password_1 = "abc";
const char* ssid_2     = "123";
const char* password_2 = "abc";

const char* BrokerMQTT = "123.123.123.123";
const int PuertoMQTT = 1883;
const char* NombreESP = "AlertaOBS_Paty";
const char* TopicControl = "alsw/control";

#define CantidadTopic 2
const char* TopicSolisitud[CantidadTopic] = {"alsw/audio_obs/#", "alsw/monitor_esp/obs"};

const char* MensajeEstado = "{\"host\":\"ryuk\",\"nombre\":\"esp32_estado_obs\",\"accion\":\"obs_estado\"}";
const char* MensajeGrabacion = "{\"host\":\"ryuk\",\"nombre\":\"cambiar_grabar\",\"accion\":\"obs_grabar\"}";
const char* MensajeGrabacionVertical = "{\"host\":\"ryuk\",\"nombre\":\"cambiar_grabar_vertical\",\"accion\":\"obs_grabar_vertical\"}";

const char* MensajeEnvivo = "{\"host\":\"ryuk\",\"nombre\":\"cambiar_envivo\",\"accion\":\"obs_envivo\"}";
const char* MensajePausar = "{\"host\":\"ryuk\",\"nombre\":\"pausar_grabar\",\"accion\":\"obs_pausar\"}";
const char* MensajeOBS = "{\"host\":\"ryuk\",\"nombre\":\"abir_OBS\", \"accion\":\"alias\", \"opciones\": {\"nombre\": \"obs\"}}";
const char* MensajeConectar = "{\"host\":\"ryuk\",\"nombre\":\"cambiar_envivo\",\"accion\":\"obs_conectar\"}";
