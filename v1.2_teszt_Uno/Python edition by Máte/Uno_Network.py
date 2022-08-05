__all__ = ["connect_msg", "tcp_connect", "tcp_send", "tcp_recv"]

import socket

PRINT_COMM = True # MODIFIABLE #

HOST = "localhost" #"medvekilatas.ddns.net"
PORT = 2020

ADDRESS = (HOST, PORT)

ID = "almafa0000"

def connect_msg() -> str:
    ans = int( input("Botok ellen (1), vagy valódi játékosok ellen mennél(2)?  (1/2): ") )
    opt = ""
    if ans == 1:
        opt = "TEST " + input("Hany bot legyen ellened? (1-7): ").strip()
    elif ans == 2:
        opt = "ROOM " + input("Melyik szobaba csatlakoznal? (20-86): ").strip()

    return f"CONNECT {opt} {ID}"

def tcp_connect(s: socket.socket):
    s.connect(ADDRESS)

def tcp_send(s: socket.socket, msg: str):
    if s.send(msg.encode("ascii")) == 0:
        print("Client: send() error.")
    else:
        if PRINT_COMM: print(f"Client: {msg}")

def tcp_recv(s: socket.socket) -> str:
    content = s.recv(1024).decode("ascii")
    if len(content) == 0:
        print("Server: recv() error.")
    else:
        if PRINT_COMM: print(f"Server: {content}")
    return content