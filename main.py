from flask import Flask, request, abort, render_template, Response
import serial
import time
import io
import picamera2
import cv2
import numpy as np

app = Flask(__name__)

face_cascade = cv2.CascadeClassifier('/home/imur/website/opencv/haarcascade_frontalface_default.xml')

frame_w = 1280
frame_h = 720
CENTER_X = frame_w // 2
CENTER_Y = frame_h // 2
DEADZONE = 100
STOP_COMMAND = 'i\n'

picam2 = picamera2.Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"size": (frame_w, frame_h)}))
picam2.start()

isTracking = False
# isSafe = True

font = cv2.FONT_HERSHEY_SIMPLEX
font_scale = 1.5
font_thickness = 2

def send_serial(data):
    if ser:
        try:
            ser.write(data.encode('utf-8'))
        except serial.SerialException:
            return "serial error"
    return "serial disable"


def generate_frames():
    global isTracking

    while True:
        current_text = f"Tracking: {isTracking}"
        color = (0, 255, 0) if isTracking else(0, 0, 255)
        frame = picam2.capture_array()
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        coord = face_cascade.detectMultiScale(
            gray,
            scaleFactor=1.1,
            minNeighbors=5,
            minSize=(30, 30)
        )
        direction_to_send = ""
        face_found = False

        for (x, y, w, h) in coord:
            face_found = True
            center_x = int(x + w / 2)
            center_y = int(y + h / 2)
            center_point = (center_x, center_y)
            text_offset = 10
            text_position = (x, y - text_offset)

            cv2.rectangle(frame, (x, y), (x + w, y + h), color, 2)
            cv2.circle(frame, center_point, 5, color, -1)
            cv2.putText(
                frame,
                current_text,
                text_position,
                font,
                font_scale,
                color,
                font_thickness,
                cv2.LINE_AA
            )
            

            if(isTracking):
                if center_x < CENTER_X - DEADZONE:
                    direction_to_send += 'x'
                elif center_x > CENTER_X + DEADZONE:
                    direction_to_send += 'y'
                            
                if center_y < CENTER_Y - DEADZONE:
                    direction_to_send += 'u' 
                elif center_y > CENTER_Y + DEADZONE:
                    direction_to_send += 'd' 
            break

        if isTracking:
            if direction_to_send:
                send_serial(direction_to_send + '\n')
            elif face_found:
                send_serial(STOP_COMMAND)
            else:
                send_serial(STOP_COMMAND)
            

        ret_code, buffer = cv2.imencode('.jpg', frame)
        if not ret_code:
            continue
 
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + buffer.tobytes() + b'\r\n')

try:
    ser = serial.Serial('/dev/ttyACM0' or '/dev/ttyACM1', 9600, timeout=1)
    time.sleep(2)
except serial.SerialException as e:
    ser = None
    print(f"Serial Port Disconnect!!: {e}")

first_connected_ip = None

@app.route('/')
def home():
    global first_connected_ip
    current_ip = request.remote_addr

    if first_connected_ip is None:
        first_connected_ip = current_ip
        return render_template('index.html')
    elif first_connected_ip == current_ip:
        return render_template('index.html')
    else:
        abort(403)

@app.route('/button', methods=['POST'])
def button():
    if ser is None:
        return 'Serial not connected', 500

    state = request.form.get('state')
    if state:
        ser.write((state + '\n').encode()) 
    return 'ok'

@app.route('/video_feed')
def video_feed():
    return Response(
        generate_frames(),
        mimetype='multipart/x-mixed-replace; boundary=frame'
    )

@app.route('/tracking', methods=['POST'])
def update_tracking():
    global isTracking

    data = request.get_json()
    new_state = data.get('state')

    if new_state is not None:
        isTracking = (new_state.lower() == 'true')
        return {'status': 'success', 'isTracking': isTracking}, 200

"""
@app.route('/safe', methods=['POST'])
def safemode():
    global isSafe

    data = request.get_json()
    new_state = data.get('state')

    if new_state is not None:
        isSafe = (new_state.lower() == 'false')
        return {'status': 'success', 'isSafe': isSafe}, 200
"""

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000, debug=False)

