package ru.nsu.kondrenko;

import java.io.Serializable;
import java.net.SocketAddress;

public record AppCopy(long pid, SocketAddress socketAddress) implements Serializable {
}
