package ru.nsu.kondrenko.gui.controller;

import ru.nsu.kondrenko.gui.utils.GridPosition;
import ru.nsu.kondrenko.gui.utils.GridPositionUtils;
import ru.nsu.kondrenko.model.Model;
import ru.nsu.kondrenko.model.game.ChessPosition;
import ru.nsu.kondrenko.model.game.Move;

import javax.swing.*;
import java.awt.event.ActionEvent;

public class HumanPlayer extends Player {
    private ChessPosition previousTouchPosition = null;

    private final Model model;

    private final Controller controller;

    public HumanPlayer(final String name, final Model model, Controller controller) {
        super(name);
        this.model = model;
        this.controller = controller;
    }

    @Override
    public boolean isBot() {
        return false;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (!(e.getSource() instanceof JButton)) {
            throw new RuntimeException("Player received incorrect event!");
        }

        final String actionCommand = e.getActionCommand();

        GridPosition gridPosition = new GridPosition(Integer.parseInt(String.valueOf(actionCommand.charAt(1))), Integer.parseInt(String.valueOf(actionCommand.charAt(0))));
        handleTouch(GridPositionUtils.gridPositionToChessPosition(model.getCurrentSide(), gridPosition));
    }

    private void handleTouch(final ChessPosition touchPosition) {
        if (previousTouchPosition != null) {
            controller.notifyAboutGeneratedMove(new Move(previousTouchPosition, touchPosition));
            previousTouchPosition = null;
        } else {
            previousTouchPosition = touchPosition;
        }
    }
}
