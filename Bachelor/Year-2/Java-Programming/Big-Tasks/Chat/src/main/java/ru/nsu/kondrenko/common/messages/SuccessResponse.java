package ru.nsu.kondrenko.common.messages;

import ru.nsu.kondrenko.common.ClientContext;

public record SuccessResponse() implements Response {
    @Override
    public void handle(ClientContext clientContext, ServerMessagesListener serverMessagesListener) {
    }

    @Override
    public String toXML() {
        return """
                <success>
                </success>
                """;
    }
}
