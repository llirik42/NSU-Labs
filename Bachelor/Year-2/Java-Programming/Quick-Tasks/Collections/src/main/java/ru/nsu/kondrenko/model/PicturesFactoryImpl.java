package ru.nsu.kondrenko.model;

public class PicturesFactoryImpl implements PicturesFactory {

    @Override
    public Picture getPicture(int id) {
        return new Picture(id);
    }
}
