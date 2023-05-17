package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.controller.Controller;
import ru.nsu.kondrenko.model.Model;
import ru.nsu.kondrenko.model.ModelListener;
import ru.nsu.kondrenko.model.ModelState;
import ru.nsu.kondrenko.model.game.Context;
import ru.nsu.kondrenko.model.game.Game;
import ru.nsu.kondrenko.model.game.GameState;
import ru.nsu.kondrenko.model.game.PieceContainer;

import javax.swing.*;

public class GUIViewer implements ModelListener {
    private final Controller controller;

    private FixedSizeFrame currentActiveFrame;

    public GUIViewer(final Controller controller) {
        this.controller = controller;
        currentActiveFrame = new MainMenuFrame(controller);
    }

    @Override
    public void notifyModelStateChange(final ModelState modelState, final Model model) {
        if (modelState == ModelState.SHOWING_SCORE_TABLE) {
            changeActiveFrame(new StatisticsFrame(model.getStatisticStorage(), controller));
        }

        if (modelState == ModelState.MAIN_MENU_CHOICE) {
            changeActiveFrame(new MainMenuFrame(controller));
        }

        if (modelState == ModelState.STARTED_SINGLEPLAYER) {
            changeActiveFrame(new SingleplayerFrame(controller));
        }

        if (modelState == ModelState.STARTED_MULTIPLAYER) {
            changeActiveFrame(new MultiplayerFrame(controller));
        }

        if (modelState == ModelState.INCORRECT_PLAYER_NAME) {
            JOptionPane.showMessageDialog(currentActiveFrame, "Incorrect player(s) name!", "Player(s) name", JOptionPane.ERROR_MESSAGE);
        }

        if (modelState == ModelState.COLOR_NOT_SELECTED) {
            JOptionPane.showMessageDialog(currentActiveFrame, "Color is not selected!", "Color", JOptionPane.ERROR_MESSAGE);
        }

        if (modelState == ModelState.START_OF_GAME) {
            changeActiveFrame(new GameFrame(controller));
        }

        if (modelState == ModelState.DRAW) {
            changeActiveFrame(new MainMenuFrame(controller));
        }

        if (modelState == ModelState.MATE) {
            changeActiveFrame(new MainMenuFrame(controller));
        }

        if (modelState == ModelState.RESTARTING_GAME) {
            final Game game = model.getGame();
            currentActiveFrame.notifyBoardChange(game.getPieceContainer(), game.getContext());
        }

        if (modelState == ModelState.SAME_NAMES) {
            JOptionPane.showMessageDialog(currentActiveFrame, "The names of the players cannot match!", "Names of players", JOptionPane.ERROR_MESSAGE);
        }

        if (modelState == ModelState.INCORRECT_MOVE) {
            JOptionPane.showMessageDialog(currentActiveFrame, "Incorrect move!", "Move", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void changeActiveFrame(final FixedSizeFrame newActiveFrame) {
        currentActiveFrame.setVisible(false);
        currentActiveFrame.dispose();
        currentActiveFrame = newActiveFrame;
    }

    @Override
    public void notifyBoardChange(final PieceContainer pieceContainer, final Context context) {
        currentActiveFrame.notifyBoardChange(pieceContainer, context);
    }

    @Override
    public void notifyGameStateChange(final GameState state, final Context context) {
        currentActiveFrame.notifyGameStateChange(state, context);
    }
}
