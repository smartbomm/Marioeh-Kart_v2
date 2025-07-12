package CarraraV;

import com.hivemq.client.mqtt.MqttClient;
import com.hivemq.client.mqtt.datatypes.MqttQos;
import com.hivemq.client.mqtt.mqtt3.Mqtt3BlockingClient;
import com.hivemq.client.mqtt.mqtt3.message.publish.Mqtt3Publish;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class MQTTBridge {

    private static Mqtt3BlockingClient mqttClient;

    public static void main(String[] args) throws Exception {
        mqttClient = MqttClient.builder()
                .useMqttVersion3()
                .identifier("udpBridge")
                .serverHost("127.0.0.1")
                .serverPort(1883)
                .buildBlocking();

        mqttClient.connectWith()
        .cleanSession(true)
        .send();

        System.out.println("Starting UDP-to-MQTT bridge");

        try (DatagramSocket socket = new DatagramSocket(4210)) {
            byte[] buffer = new byte[256];
            
            while (true) {
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                socket.receive(packet);
                
                String message = new String(packet.getData(), 0, packet.getLength());
                processMessage(message);
            }
        }
    }

    private static void processMessage(String rawMessage) {
        try {
            String[] parts = rawMessage.split(";");

            if (parts.length >= 2) {
                String carId = parts[0].trim();
                List<Mqtt3Publish> messages = new ArrayList<>();

                for (int i = 1; i < parts.length; i++) {
                    String[] keyValue = parts[i].split("=", 2);
                    if (keyValue.length == 2) {
                        String topic = carId + "/" + keyValue[0].trim();
                        String payload = keyValue[1].trim();

                        messages.add(
                            Mqtt3Publish.builder()
                                .topic(topic)
                                .payload(payload.getBytes(StandardCharsets.UTF_8))
                                .qos(MqttQos.AT_LEAST_ONCE)
                                .retain(false)
                                .build()
                        );
                    }
                }

                if (!messages.isEmpty()) {
                    synchronized (mqttClient) {
                        for (Mqtt3Publish message : messages) {
                            mqttClient.publish(message);
                            //System.out.print(message + "/n");
                        }
                    }
                }
            }
        } catch (Exception e) {
            System.err.println("Error processing message: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
