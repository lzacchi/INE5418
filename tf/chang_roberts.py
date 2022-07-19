from udp_lib import udp_comm
from node import Node


def create_node_list(n_nodes: int) -> list[Node]:
    base_address = "localhost"
    base_port = 9000

    nodes_list: list[Node] = []

    for i in range(n_nodes):
        nodes_list.append(Node(i, base_address, base_port + i))

    return nodes_list


def election(n_nodes: int) -> Node:
    nodes = create_node_list(n_nodes)

    # first loop:
    # compare and set ids

    # second loop
    # elect leader

    leader = nodes[0]
    print("Leader is {}".format(leader))
    return leader
