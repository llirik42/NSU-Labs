package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.modelobjects.Session;

public interface NonLoginRequest extends Request {
    Session getSession();
}
