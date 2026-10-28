import time
import struct

try:
    import ujson as json
except ImportError:
    import json

try:
    import network
except ImportError:
    network = None

try:
    import usocket as socket
except ImportError:
    import socket

try:
    from machine import ADC
except ImportError:
    ADC = None


def load_config(path="config.json"):
    f = open(path, "r")
    try:
        return json.loads(f.read())
    finally:
        f.close()


CFG = load_config()

APN = CFG.get("apn", "internet")
APN_USER = CFG.get("apn_user", "")
APN_PASS = CFG.get("apn_pass", "")
BROKER = CFG["broker"]
PORT = CFG["port"]
TOPIC = CFG.get("topic", "a9g/test").encode()
CLIENT_ID = CFG.get("client_id", "a9g-01").encode()
PUBLISH_INTERVAL = CFG.get("publish_interval", 10)
SENSOR_ENABLED = CFG.get("sensor_enabled", False)
ADC_CHANNEL = CFG.get("adc_channel", 0)
KEEPALIVE = CFG.get("keepalive", 60)


def encode_len(size):
    out = b""
    while True:
        value = size & 0x7F
        size >>= 7
        if size:
            value |= 0x80
        out += bytes([value])
        if not size:
            return out


def mqtt_connect(sock, client_id, keepalive=60):
    variable_header = b"\x00\x04MQTT\x04\x02" + struct.pack("!H", keepalive)
    payload = struct.pack("!H", len(client_id)) + client_id
    packet = b"\x10" + encode_len(len(variable_header) + len(payload))
    packet += variable_header + payload
    sock.send(packet)

    response = sock.recv(4)
    if len(response) < 4 or response[0] != 0x20 or response[3] != 0x00:
        raise Exception("MQTT connect failed: %r" % (response,))


def mqtt_publish(sock, topic, message):
    variable_header = struct.pack("!H", len(topic)) + topic
    packet = b"\x30" + encode_len(len(variable_header) + len(message))
    packet += variable_header + message
    sock.send(packet)


def read_sensor():
    if not SENSOR_ENABLED or ADC is None:
        return None

    try:
        adc = ADC(ADC_CHANNEL)
        return adc.read()
    except Exception as exc:
        print("ADC read failed:", exc)
        return None


def build_payload(counter):
    analog = read_sensor()
    if analog is None:
        data = {"count": counter}
    else:
        data = {"count": counter, "analog": analog}
    return json.dumps(data).encode()


def connect_gprs():
    if network is None:
        raise Exception("network module not available")

    modem = network.WWAN()

    try:
        modem.active(True)
    except Exception as exc:
        print("WWAN active warning:", exc)

    try:
        if modem.isconnected():
            print("GPRS already connected:", modem.ifconfig())
            return modem
    except Exception:
        pass

    try:
        modem.connect(APN, APN_USER, APN_PASS)
    except Exception as exc:
        if "already on" not in str(exc):
            raise

    for _ in range(20):
        if modem.isconnected():
            print("GPRS connected:", modem.ifconfig())
            return modem
        print("Waiting for GPRS...")
        time.sleep(1)

    raise Exception("GPRS failed")


def open_socket():
    info = socket.getaddrinfo(BROKER, PORT)[0][-1]
    sock = socket.socket()
    sock.connect(info)
    print("TCP connected:", BROKER, PORT)
    return sock


def connect_mqtt():
    sock = open_socket()
    mqtt_connect(sock, CLIENT_ID, KEEPALIVE)
    print("MQTT connected")
    return sock


def main():
    connect_gprs()
    counter = 0

    while True:
        sock = None
        try:
            sock = connect_mqtt()
            while True:
                payload = build_payload(counter)
                mqtt_publish(sock, TOPIC, payload)
                print("Published:", payload)
                counter += 1
                time.sleep(PUBLISH_INTERVAL)
        except Exception as exc:
            print("Loop error:", exc)
            time.sleep(5)
        finally:
            try:
                if sock:
                    sock.close()
            except Exception:
                pass


if __name__ == "__main__":
    main()
