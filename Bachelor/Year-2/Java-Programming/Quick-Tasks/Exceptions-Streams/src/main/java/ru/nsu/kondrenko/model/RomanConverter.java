package ru.nsu.kondrenko.model;

import java.util.TreeMap;

public final class RomanConverter implements Converter{
    private final static TreeMap<Integer, String> map = new TreeMap<>();

    static {
        map.put(1000, "M");
        map.put(900, "CM");
        map.put(500, "D");
        map.put(400, "CD");
        map.put(100, "C");
        map.put(90, "XC");
        map.put(50, "L");
        map.put(40, "XL");
        map.put(10, "X");
        map.put(9, "IX");
        map.put(5, "V");
        map.put(4, "IV");
        map.put(1, "I");
    }

    public String convert(int x) {
        int l =  map.floorKey(x);
        if (x == l) {
            return map.get(x);
        }
        return map.get(l) + convert(x - l);
    }
}
