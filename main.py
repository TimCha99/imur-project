from flask import Flask, request, abort, render_template, Response
import serial
import time
import io
import picamera2

app = Flask(__name__)

picam2 = picamera2.Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"size": (1280, 720)}))
picam2.start()

def generate_frames():
    while True:
        stream = io.BytesIO()
        picam2.capture_file(stream, format='jpeg')
        stream.seek(0)
        frame = stream.read()
        
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

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
    return Response(generate_frames(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='10.42.0.1', port=8000, debug=False)
