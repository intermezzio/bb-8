# mock-api.py
#
# A simple mock Python HTTP server to test the display using dummy data.

from http.server import HTTPServer, BaseHTTPRequestHandler
import json

# port number for test server
LOCALHOST_PORT = 3000

# port for bluetooth
BLUETOOTH_PORT = "/dev/rfcomm1" # or "COM9", etc

# baud rate
BAUD_RATE = 9600

ser = serial.Serial()
ser.baudrate = BAUD_RATE
ser.port = BLUETOOTH_PORT

last_command = ""

class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        try:
            if self.path == "/last":
                with open("." + self.path, "r") as document:
                    
                    self.send_response(200)
                    self.send_header("Content-type", "text/html")
                    self.end_headers()
                    self.wfile.write(document.read().encode("utf-8"))

        except:
        	self.send_response(500)
        	self.end_headers()
        	self.wfile.write(b"500 internal server error")

    def do_POST(self):
        try:
            if self.path == "/send":
                length = int(self.headers.get("content-length"))
                request = json.loads(self.rfile.read(length))
                text = request["text"]
                
                if == "stop":
                	ser.close()
                    self.send_response(200)
                    self.end_headers()
                    self.wfile.write(b"The arduino was stopped")

                else:
                    ser.write(text.encode())
                    last_command = text
                    self.send_response(200)
                    self.end_headers()
                    self.wfile.write(b"The command was sent")

            else:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"404 not found")

        except:
            self.send_response(500)
            self.end_headers()
            self.wfile.write(b"500 internal server error")

    def do_OPTIONS(self):
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "X-PINGOTHER, Content-Type")
        self.send_header("Access-Control-Max-Age", "86400")
        self.send_header("Vary", "Accept-Encoding, Origin")
        self.end_headers()


if __name__ == "__main__":
    print("Initializing server")
    httpd = HTTPServer(("0.0.0.0", PORT), ModelRequestHandler)
    print("Serving on port %d" % PORT)
    httpd.serve_forever()