package ru.nsu.kondrenko.common.modelobjects;

import java.io.Serializable;

public record ChatMessage(String content, ChatUser sender) implements Serializable {

}
