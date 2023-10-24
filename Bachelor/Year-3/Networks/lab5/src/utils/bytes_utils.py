from typing import List


def int_to_network_bytes(i: int) -> bytes:
    return b'\x00' if i == 0 else i.to_bytes(length=(i.bit_length() + 7) // 8, byteorder="big")


def network_bytes_to_int(b: bytes) -> int:
    return int.from_bytes(bytes=b, byteorder="big")


def concat_bytes_list(bytes_list: List[bytes]) -> bytes:
    ret: bytes = bytes_list[0]
    for x in bytes_list[1:]:
        ret = ret + x

    return ret


def get_byte(data: bytes, i: int) -> bytes:
    return data[i:i + 1]
