package ru.nsu.kondrenko.common;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;

public final class ProtocolUtils {
    public static final Charset MAIN_CHARSET = StandardCharsets.UTF_8;

    private static final ByteBuffer longBuffer = ByteBuffer.allocate(Long.BYTES);
    private static final ByteBuffer intBuffer = ByteBuffer.allocate(Integer.BYTES);

    public static byte[] longToBytes(long x) {
        longBuffer.putLong(0, x);
        return longBuffer.array();
    }

    public static long bytesToLong(byte[] bytes) {
        return ByteBuffer.wrap(bytes).getLong();
    }

    public static byte[] intToBytes(int x) {
        intBuffer.putInt(0, x);
        return intBuffer.array();
    }

    public static int bytesToInt(byte[] bytes) {
        return ByteBuffer.wrap(bytes).getInt();
    }
}
