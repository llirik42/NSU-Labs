package ru.nsu.kondrenko.model.context;

public interface ContextListener {
    void onContextActionChange(Context context);

    void onContextToolChange(Context context);
}
