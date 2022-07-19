from fileinput import close
from multiprocessing import Process
import time
import socket
from typing import Any
from udp_lib import udp_comm


ADDRESS: str = "localhost"
p0_port: int = 9000
p1_port: int = 9001
p2_port: int = 9002


class Node:
    def __init__(self, id: int, address: str, port: int) -> None:
        self.id = id
        self.address = address
        self.port = port

    def socket_data(self, address: str, port: int) -> tuple[str, int]:
        return (address, port)

    def start_node(self, socket_data: tuple[str, int]) -> socket.socket:
        # There's no practical use for socket_data here
        # We'll use it as a safety measure
        # So we know exactly where we're creating a socket
        socket = udp_comm.open_socket()
        print("Node started at port {socket_data[1]}")
        return socket

    def bind_node(self, socket: socket.socket, src: tuple[str, int]) -> None:
        # server side
        udp_comm.bind_socket(socket, src)

    def close_node(self, socket: socket.socket) -> None:
        udp_comm.close_socket(socket)

    def send_message(
        self,
        src_socket: socket.socket,
        target_data: tuple[str, int],
        message: bytes,
    ) -> None:
        # treat message
        udp_comm.send(src_socket, target_data, self.id, message)

    def receive_message(self, src_socket: socket.socket) -> bytes:
        while True:
            message = udp_comm.receive(src_socket, self.id)
            print(
                "Received message: {!r} from address {}".format(
                    message, src_socket.getsockname()
                )
            )

    def listen(self, src_socket: socket.socket) -> Process:
        # Will be a running process
        listen_process = Process(target=self.receive_message, args=(src_socket,))
        listen_process.start()
        return listen_process

    def stop_listen(self, process: Process) -> None:
        process.terminate()


if __name__ == "__main__":
    # vector_clock is common to all processes
    vector_clock = udp_comm.initialize_vector_clock(3)

    node0 = Node(0, "localhost", 9000)
    node1 = Node(1, "localhost", 9001)
    node2 = Node(2, "localhost", 9002)

    node0_data = node0.socket_data(node0.address, node0.port)
    node1_data = node0.socket_data(node1.address, node1.port)
    node2_data = node0.socket_data(node2.address, node2.port)

    node0_socket = node0.start_node(node0_data)
    node1_socket = node1.start_node(node1_data)
    node2_socket = node2.start_node(node2_data)

    node0.bind_node(node0_socket, node0_data)
    node1.bind_node(node1_socket, node1_data)
    node2.bind_node(node2_socket, node2_data)

    # Node 0 to send message to Node 1 and Node 2
    message_1 = b"Message from Node 0 to Node 1"
    message_2 = b"Message from Node 0 to Node 2"
    message_3 = b"Message from Node 1 to Node 0"
    message_4 = b"Message from Node 2 to Node 1"
    message_5 = b"Message from node 2 to Node 0"

    node0_listener = node0.listen(node0_socket)
    node1_listener = node1.listen(node1_socket)
    node2_listener = node2.listen(node2_socket)

    node0.send_message(node1_socket, node1_data, message_1)
    node0.send_message(node2_socket, node2_data, message_2)

    node1.send_message(node0_socket, node0_data, message_3)

    node2.send_message(node1_socket, node1_data, message_4)
    node2.send_message(node0_socket, node0_data, message_5)

    time.sleep(2)  # wait a bit to guarantee everything is sent and received

    node0.stop_listen(node0_listener)
    node1.stop_listen(node1_listener)
    node2.stop_listen(node2_listener)

    node0.close_node(node0_socket)
    node1.close_node(node1_socket)
    node2.close_node(node2_socket)
