from flask import Flask, render_template
from flask_socketio import SocketIO
import time
import math
import threading

app = Flask(__name__)
socketio = SocketIO(app)

beacons = [
    {"id": 1, "x": 0, "y": 0, "z": 0, "ip": "192.168.50.76", "mac": "e8:6b:ea:2b:ce:78", "battery": 100, "rssi": None, "voltage": None},
    {"id": 2, "x": 5, "y": 0, "z": 0, "ip": "192.168.50.34", "mac": "e8:6b:ea:2b:02:30", "battery": 100, "rssi": None, "voltage": None},
    {"id": 3, "x": 0, "y": 5, "z": 3, "ip": "192.168.50.93", "mac": "e8:6b:ea:2b:02:08", "battery": 100, "rssi": None, "voltage": None}
]

client_pos = {"x": 2.5, "y": 2.5, "z": 0.5}
angle = 0
logs = []

def update_client_position():
    global angle
    while True:
        client_pos["x"] = 2.5 + 0.3 * math.cos(angle)
        client_pos["y"] = 2.5 + 0.3 * math.sin(angle)
        client_pos["z"] = 0.5
        angle += 0.1
        if angle >= 2 * math.pi:
            angle = 0
        log_message = f"Client position updated: x={client_pos['x']:.2f}, y={client_pos['y']:.2f}, z={client_pos['z']:.2f}"
        logs.append(log_message)
        print(log_message)
        socketio.emit('update', {'client': client_pos, 'beacons': beacons, 'logs': logs[-10:]})
        time.sleep(2)

@socketio.on('beacon_update')
def handle_beacon_update(data):
    beacon_id = data.get('id')
    battery = data.get('battery')
    wifi_rssi = data.get('wifi_rssi')
    voltage = data.get('voltage')
    station_mac = data.get('station_mac')
    for beacon in beacons:
        if beacon['id'] == beacon_id and beacon['mac'].lower() == station_mac.lower():
            beacon['battery'] = battery
            beacon['rssi'] = wifi_rssi
            beacon['voltage'] = voltage
            break
    log_message = f"Updated beacon {beacon_id}: battery={battery}%, wifi_rssi={wifi_rssi}, voltage={voltage}V"
    logs.append(log_message)
    print(log_message)
    socketio.emit('update', {'client': client_pos, 'beacons': beacons, 'logs': logs[-10:]})

@app.route('/')
def index():
    return render_template('index.html', beacons=beacons)

if __name__ == '__main__':
    threading.Thread(target=update_client_position, daemon=True).start()
    socketio.run(app, host='0.0.0.0', port=5000)