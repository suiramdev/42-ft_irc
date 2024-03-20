import os
import requests
import socket
import signal
import sys

# Constants for configuration
SOCKET_PORT = 8080
SOCKET_HOST = "localhost"
BEARER_KEY = os.getenv("MISTRAL_API_KEY")  # Use getenv for better readability


def send_request_to_mistral(prompt):
    """Send a request to the Mistral API and return the response."""
    headers = {
        "Authorization": f"Bearer {BEARER_KEY}",
        "Content-Type": "application/json",
        "Accept": "application/json",
    }
    data = {
        "model": "open-mistral-8x7b",  # Corrected typo in model name
        "max_tokens": 10,
        "messages": [{"role": "user", "content": prompt}],
    }
    response = requests.post(
        "https://api.mistral.ai/v1/chat/completions",
        headers=headers,
        json=data,
    )
    return response.json()["choices"][0]["message"]["content"]


def signal_handler(sig, frame):
    """Signal handler for graceful shutdown."""
    print("\nShutting down gracefully...")
    # Perform any necessary cleanup here
    sys.exit(0)


def main():
    """Main function to start the server and handle connections."""
    if not BEARER_KEY:
        print("Please set the MISTRAL_API_KEY environment variable.")
        exit(1)

    signal.signal(signal.SIGINT, signal_handler)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((SOCKET_HOST, SOCKET_PORT))
        s.listen(1)
        print(f"Listening on {SOCKET_HOST}:{SOCKET_PORT}...")

        while True:
            conn, _ = s.accept()
            with conn:
                prompt = conn.recv(4096).decode("utf-8")

                if prompt:
                    try:
                        response = send_request_to_mistral(prompt)
                        conn.sendall(response.encode("utf-8"))
                    except Exception as e:
                        print(f"Error: {e}")
                        conn.sendall("I'm sorry, I don't know.".encode("utf-8"))


if __name__ == "__main__":
    main()
