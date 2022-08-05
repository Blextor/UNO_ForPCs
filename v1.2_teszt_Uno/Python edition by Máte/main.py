from Uno_Network import *

import sys
import socket
from typing import Callable

if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP) as s:
        CONMSG = connect_msg()
        tcp_connect(s)
        tcp_send(s, CONMSG)
        connect_response = tcp_recv(s)
        """
        if connect_response.startswith("OK"):
            print(connect_response)
            input("Press a key to exit...")
            sys.exit(1)
        """
        print("CONNECTED!")

        ########################################

        command = ""
        on = True

        while on:
            tcp_send(s, "READY")

            data = tcp_recv(s)

            ########################################

            # MODIFIABLE # PER-GAME SETUP #

            ########################################

            tcp_send(s, "OK")

            print("READY FOR NEW GAME!")

            command = ""

            while command != "END":
                recv_list = tcp_recv(s).split()
                command, recv_list = recv_list[0], recv_list[1:]
                if command == "YT!":
                    command, recv_list = recv_list[0], recv_list[1:]
                    if command == "PLUS":
                        ########################################
                        # MODIFIABLE # PLUS -> ACTION #
                        DUMMY_canPutPlus: Callable[[],bool] = lambda: False
                        if not DUMMY_canPutPlus():
                            tcp_send(s, "DRAW")
                            card_list = tcp_recv(s).split()
                            DUMMY_processDraw: Callable[[list],None] = lambda ls: None
                            DUMMY_processDraw(card_list)
                            tcp_send(s, "OK")
                        else:
                            tcp_send(s, "PUT 1 GP") # MODIFIABLE # DOESN'T MAKE SENSE #
                        ########################################
                    elif command == "OFF":
                        ########################################
                        # MODIFIABLE # OFF -> ACTION #
                        tcp_send(s, "OK")
                        ########################################
                    elif command == "NONE":
                        ########################################
                        # MODIFIABLE # NONE -> ACTION #
                        DUMMY_canPutCard: Callable[[],bool] = lambda: False
                        if not DUMMY_canPutCard():
                            tcp_send(s, "DRAW")
                            card_list = tcp_recv(s).split()
                            DUMMY_processDraw: Callable[[list],None] = lambda ls: None
                            DUMMY_processDraw(card_list)
                            tcp_send(s, "OK")
                        else:
                            tcp_send(s, "PUT 1 GP") # MODIFIABLE # DOESN'T MAKE SENSE #
                        ########################################
                    else:
                        tcp_send(s, "ERROR")
                elif command == "WIN":
                    print(f"Helyezett: {recv_list[0]}")
                    tcp_send(s, "OK")
                elif command == "END":
                    if recv_list[0] == "0":
                        on = False
                else:
                    on = False
                    break
        
        ########################################

        # END #

        ########################################

        tcp_send(s, "BYE")
        print(tcp_recv(s))

        input("Press a key to exit...")
        sys.exit(0)