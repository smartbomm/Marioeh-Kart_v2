package crash_warn;

import java.util.Map;
import java.util.Set;
import java.util.concurrent.*;

import com.hivemq.client.mqtt.MqttClient;
import com.hivemq.client.mqtt.MqttGlobalPublishFilter;
import com.hivemq.client.mqtt.datatypes.MqttQos;
import com.hivemq.client.mqtt.mqtt3.Mqtt3BlockingClient;
import com.hivemq.client.mqtt.mqtt3.Mqtt3BlockingClient.Mqtt3Publishes;
import com.hivemq.client.mqtt.mqtt3.message.publish.Mqtt3Publish;

public class crash {

    public static void main(String[] args) throws Exception {
        //System.out.println("Starte Client");

        Mqtt3BlockingClient client = MqttClient.builder()
                .useMqttVersion3()
                .identifier("crash_warning")
                .serverHost("127.0.0.1") //localhost
				.serverPort(1883)
				.buildBlocking();

        client.connectWith().cleanSession(true).send();

        Mqtt3Publishes incomingMsg = client.publishes(MqttGlobalPublishFilter.SUBSCRIBED);
        client.subscribeWith().topicFilter("C/+/p/#").qos(MqttQos.AT_MOST_ONCE).send();
        client.subscribeWith().topicFilter("C/+/ts").qos(MqttQos.AT_MOST_ONCE).send();

        ConcurrentMap<String, String> xValues = new ConcurrentHashMap<>();
        ConcurrentMap<String, String> yValues = new ConcurrentHashMap<>();
        ConcurrentMap<String, String> xSnapshot = new ConcurrentHashMap<>();
        ConcurrentMap<String, String> ySnapshot = new ConcurrentHashMap<>();
        ConcurrentMap<String, Long> lastXYTimestamp = new ConcurrentHashMap<>();
        ConcurrentMap<String, ScheduledFuture<?>> activeSenders = new ConcurrentHashMap<>();
        ConcurrentMap<String, String> tsStatus = new ConcurrentHashMap<>();
        Set<String> blockedIDs = ConcurrentHashMap.newKeySet();
        Set<String> sentOnce = ConcurrentHashMap.newKeySet();

        ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();

        // Prüft regelmäßig ob eine ID lange genug inaktiv war, um zu senden
        scheduler.scheduleAtFixedRate(() -> {
            long now = System.currentTimeMillis();

            for (String id : lastXYTimestamp.keySet()) {
                long last = lastXYTimestamp.getOrDefault(id, 0L);

                if ((now - last) <= 1000 ||
                    activeSenders.containsKey(id) ||
                    sentOnce.contains(id) ||
                    "0".equals(tsStatus.getOrDefault(id, "1"))) {
                    continue;
                }

                String x = xValues.get(id);
                String y = yValues.get(id);
                String oldX = xSnapshot.get(id);
                String oldY = ySnapshot.get(id);

                if (x == null || y == null) continue;

                // Nur senden, wenn x UND y unverändert sind
                if ((oldX == null || !x.equals(oldX)) || (oldY == null || !y.equals(oldY))) {
                    xSnapshot.put(id, x);
                    ySnapshot.put(id, y);
                    continue;
                }

                // Sende x und y alle 500ms für 10s
                ScheduledFuture<?> senderTask = scheduler.scheduleAtFixedRate(() -> {
                    if ("0".equals(tsStatus.getOrDefault(id, "1"))) {
                        return; // nicht senden bei ts=0
                    }

                    try {
                        client.publishWith()
                                .topic("K/" + id + "/x")
                                .payload(x.getBytes())
                                .qos(MqttQos.AT_MOST_ONCE)
                                .retain(false)
                                .send();

                        client.publishWith()
                                .topic("K/" + id + "/y")
                                .payload(y.getBytes())
                                .qos(MqttQos.AT_MOST_ONCE)
                                .retain(false)
                                .send();

                        //System.out.println("[Send] K/" + id + "/x=" + x + ", y=" + y);
                    } catch (Exception e) {
                        //System.err.println("[Publish error] " + e.getMessage());
                    }
                }, 0, 500, TimeUnit.MILLISECONDS);

                scheduler.schedule(() -> {
                    senderTask.cancel(false);
                    activeSenders.remove(id);
                    sentOnce.add(id);
                    //System.out.println("[Send stopped after 10s] ID: " + id);
                }, 10, TimeUnit.SECONDS);

                activeSenders.put(id, senderTask);
            }
        }, 0, 500, TimeUnit.MILLISECONDS);

        // Nachrichtenempfang
        while (true) {
            Mqtt3Publish rcvd = incomingMsg.receive();
            String topicStr = rcvd.getTopic().toString();
            String[] parts = topicStr.split("/");
            if (parts.length < 3) continue;

            String id = parts[1];
            String payload = new String(rcvd.getPayloadAsBytes()).trim();

            // ts == 0 C/{id}/ts
            if (parts.length == 3 && parts[2].equals("ts")) {
                tsStatus.put(id, payload);

                if (payload.equals("0")) {
                    blockedIDs.add(id);
                    ////System.out.println("[ts=0] Blocked ID: " + id);
                    ScheduledFuture<?> task = activeSenders.remove(id);
                    if (task != null) task.cancel(false);
                } else {
                    blockedIDs.remove(id);
                }
                continue;
            }

            // x/y unter C/{id}/p/x oder /p/y
            if (parts.length == 4 && parts[2].equals("p")) {
                String field = parts[3];
                if (field.equals("x")) xValues.put(id, payload);
                if (field.equals("y")) yValues.put(id, payload);

                if (field.equals("x") || field.equals("y")) {
                    lastXYTimestamp.put(id, System.currentTimeMillis());
                    sentOnce.remove(id);

                    ScheduledFuture<?> running = activeSenders.remove(id);
                    if (running != null) {
                        running.cancel(false);
                        //System.out.println("[New message] Aborted sender for ID: " + id);
                    }
                }
            }
        }
    }
}
