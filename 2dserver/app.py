from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO
from datetime import datetime

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

# Grid dimensions
GRID_COLS = 114
GRID_ROWS = 92

# Current state
current_pos = {"x": 0, "y": 0}
position_log = []

@app.route('/')
def index():
    return render_template('map.html', 
                         grid_cols=GRID_COLS,
                         grid_rows=GRID_ROWS)

@app.route('/update_position', methods=['POST'])
def update_position():
    global current_pos
    data = request.get_json()
    
    if not data or not isinstance(data, list):
        return jsonify({"status": "error"}), 400
    
    for pos in data:
        if 'x' in pos and 'y' in pos:
            x = max(0, min(int(pos['x']), GRID_COLS-1))
            y = max(0, min(int(pos['y']), GRID_ROWS-1))
            current_pos = {"x": x, "y": y}
            log_entry = f"{datetime.utcnow().isoformat()} - Position: ({x}, {y})"
            position_log.append(log_entry)
            socketio.emit('position_update', current_pos)
            socketio.emit('log_update', {"log": log_entry})
    
    return jsonify({"status": "success", "position": current_pos})

@app.route('/get_logs')
def get_logs():
    return jsonify({"logs": position_log[-20:]})

@socketio.on('connect')
def handle_connect():
    socketio.emit('position_update', current_pos)
    socketio.emit('initial_logs', {"logs": position_log[-20:]})

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000, debug=True)
