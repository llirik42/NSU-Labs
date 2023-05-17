package ru.nsu.kondrenko.model;

import java.util.LinkedHashMap;
import java.util.Map;

public class PicturesFactoryProxy implements PicturesFactory {
    private final LinkedHashMap<Integer, Picture> map;

    private final PicturesFactory factory;

    public PicturesFactoryProxy(int N, PicturesFactory factory){
        map = new LinkedHashMap<>() {
            protected boolean removeEldestEntry(Map.Entry eldest) {
                return size() > N;
            }
        };

        this.factory = factory;
    }

    public Picture getPicture(int id){
        if (!map.containsKey(id)){
            map.put(id, factory.getPicture(id));
        }

        return map.get(id);
    }
}
