import requests

send_command = requests.post("localhost:3000/send", 
    json={"text": "left"}
)

last_command = requests.get("localhost:3000/last")