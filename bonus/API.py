import os
import requests
import socket
import signal
import sys
import threading

SOCKET_PORT = 8080
SOCKET_HOST = "localhost"
BEARER_KEY = os.getenv("MISTRAL_API_KEY")


def signal_handler(sig, frame):
    print("\nShutting down gracefully...")
    sys.exit(0)


def send_request_to_mistral(prompt):
    """Send a request to the Mistral API and return the response."""
    headers = {
        "Authorization": f"Bearer {BEARER_KEY}",
        "Content-Type": "application/json",
        "Accept": "application/json",
    }
    data = {
        "model": "open-mistral-7b",
        "max_tokens": 10,
        "messages": [{"role": "user", "content": prompt}],
    }
    response = requests.post(
        "https://api.mistral.ai/v1/chat/completions",
        headers=headers,
        json=data,
    )
    print(response.json())
    return response.json()["choices"][0]["message"]["content"]


def handle_client(conn):
    with conn:
        while True:
            prompt = conn.recv(4096).decode("utf-8").strip()
            if not prompt or prompt.lower() == "quit":
                break
            try:
                response = send_request_to_mistral(prompt)
                conn.sendall(response.encode("utf-8"))
            except Exception as e:
                print(f"Error: {e}")
                conn.sendall("I'm sorry, I don't know.".encode("utf-8"))


def main():
    if not BEARER_KEY:
        print("Please set the MISTRAL_API_KEY environment variable.")
        exit(1)

    signal.signal(signal.SIGINT, signal_handler)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((SOCKET_HOST, SOCKET_PORT))
        s.listen()
        print(f"Listening on {SOCKET_HOST}:{SOCKET_PORT}...")
        while True:
            conn, addr = s.accept()
            print(f"{addr} connected.")
            client_thread = threading.Thread(target=handle_client, args=(conn,))
            client_thread.start()


if __name__ == "__main__":
    main()
