package MarioConverter;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.hivemq.client.mqtt.MqttClient;
import com.hivemq.client.mqtt.MqttGlobalPublishFilter;
import com.hivemq.client.mqtt.datatypes.MqttQos;
import com.hivemq.client.mqtt.datatypes.MqttTopic;
import com.hivemq.client.mqtt.mqtt3.Mqtt3BlockingClient;
import com.hivemq.client.mqtt.mqtt3.Mqtt3BlockingClient.Mqtt3Publishes;
import com.hivemq.client.mqtt.mqtt3.message.publish.Mqtt3Publish;
import de.marioehkart.marioehkarte.Map;
import de.marioehkart.marioehkarte.Coordinates;

public class MarioeTranslator {

	public static void main(String[] args) throws Exception {
		System.out.println("Starte Client");

		Mqtt3BlockingClient client = MqttClient.builder()
				.useMqttVersion3()
				.identifier("XYPublisher")
				.serverHost("127.0.0.1")
				.serverPort(1883)
				.buildBlocking();
		
        client.connectWith()
        .cleanSession(true)
        .send();
		
		Mqtt3Publishes incomingMsg =  client.publishes(MqttGlobalPublishFilter.SUBSCRIBED);
		
		client.subscribeWith()
		.topicFilter("C/+/p")
		.qos(MqttQos.AT_MOST_ONCE)
		.send();
		
		Gson myGson = new GsonBuilder().create();
		Message myMessage = new Message();
		Coordinates coords = new Coordinates();
		
	    Map carreraMap;
        carreraMap = new Map("logs");
        carreraMap.feed("BahnKarte.csv", ";");
		
		while(true){
			Mqtt3Publish rcvd = incomingMsg.receive();
			MqttTopic topic = rcvd.getTopic();
			byte[]    payload = rcvd.getPayloadAsBytes();
			String msg = new String(payload);
			//System.out.println(msg);
			myMessage =  myGson.fromJson(msg,Message.class);
			try {
				double pos_linear = myMessage.p;
				int section = myMessage.ts;
				coords = carreraMap.getCoordinates(section, pos_linear);//
				String St_topic = topic.toString();
				String St_x = String.valueOf(coords.x);
				String St_y = String.valueOf(coords.y);
				String St_a = String.valueOf(coords.angle_x);
				
				client.publishWith()
				.topic(St_topic + "/x")
				.payload(St_x.getBytes())
				.qos(MqttQos.AT_MOST_ONCE)
				.retain(false)
				.send();
				client.publishWith()
				.topic(St_topic + "/y")
				.payload(St_y.getBytes())
				.qos(MqttQos.AT_MOST_ONCE)
				.retain(false)
				.send();
				client.publishWith()
				.topic(St_topic + "/a")
				.payload(St_a.getBytes())
				.qos(MqttQos.AT_MOST_ONCE)
				.retain(false)
				.send();
			}
			catch (IllegalArgumentException  | IndexOutOfBoundsException | NullPointerException e) {}
				//do nothing becourse the last value is the best thing to assume
		}
	}
}
