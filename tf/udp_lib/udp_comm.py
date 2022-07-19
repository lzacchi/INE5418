import socket
import string
from tokenize import String

# from config import n_process

packet: dict[list[int], bytes] = {}


def initialize_vector_clock(n_processes: int) -> list[int]:
    vector_clock = [0] * n_processes
    print("Initialized vector clock {}".format(vector_clock))
    return vector_clock


def open_socket() -> socket.socket:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return sock


def close_socket(socket: socket.socket) -> None:
    socket.close()


def bind_socket(socket: socket.socket, src: tuple[str, int]) -> None:
    socket.bind(src)


""" def update_clock(p_id: int) -> None:
    print("Updating vector clock at inted {}".format(p_id))
    vector_clock[p_id] += 1
    print("Vector clock updated: {}".format(vector_clock))
"""


def send(
    socket: socket.socket, dest: tuple[str, int], p_id: int, message: bytes
) -> None:
    # TODO: Sync logic clock
    # TODO: Handle source/dest
    socket.sendto(message, dest)
    # update_clock(p_id)


def receive(socket: socket.socket, p_id: int) -> bytes:
    # TODO: Sync logic clock
    # TODO: Handle source/dest
    data, _ = socket.recvfrom(1024)
    # update_clock(p_id)
    return data


def broadcast(
    socket: socket.socket,
    targets: dict[tuple[str, int], int],
    p_id: int,
    message: bytes,
) -> None:
    for target in targets:
        socket.sendto(message, target[0])
