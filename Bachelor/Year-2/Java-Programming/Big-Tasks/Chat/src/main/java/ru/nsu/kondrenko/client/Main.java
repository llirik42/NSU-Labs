package ru.nsu.kondrenko.client;

import ru.nsu.kondrenko.client.controller.Controller;
import ru.nsu.kondrenko.client.model.DefaultClient;
import ru.nsu.kondrenko.client.view.SwingView;
import ru.nsu.kondrenko.common.connections.factories.ConnectionsFactory;
import ru.nsu.kondrenko.common.connections.factories.DefaultConnectionsFactory;

public class Main {
    public static void main(String[] args) {
        final Controller controller = new Controller();
        final SwingView view = new SwingView(controller, controller);
        final ConnectionsFactory connectionsFactory = new DefaultConnectionsFactory();
        final DefaultClient defaultClient = new DefaultClient(view.getMessageDocument(), view, connectionsFactory);
        view.setClientContext(defaultClient);
        controller.setView(view);
        controller.setClient(defaultClient);
        controller.start();
    }
}
