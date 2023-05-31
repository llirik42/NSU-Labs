package ru.nsu.kondrenko.common;

import java.net.InetAddress;

public final class Utils {
    private static final int MIN_PORT_NUMBER = 0;
    private static final int MAX_PORT_NUMBER = 65535;

    private Utils() {
    }

    public static boolean isPortValid(int port) {
        return MIN_PORT_NUMBER <= port && port <= MAX_PORT_NUMBER;
    }

    public static boolean isPortValid(String port) {
        try {
            return isPortValid(Integer.parseInt(port));
        } catch (Exception exception) {
            return false;
        }
    }

    public static boolean isAddressValid(String address) {
        try {
            InetAddress.getByName(address);
            return true;
        } catch (Exception exception) {
            return false;
        }
    }
}
