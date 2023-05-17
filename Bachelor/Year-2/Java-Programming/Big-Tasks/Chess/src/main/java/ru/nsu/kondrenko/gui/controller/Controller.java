package ru.nsu.kondrenko.gui.controller;

import ru.nsu.kondrenko.model.Model;
import ru.nsu.kondrenko.model.ModelState;
import ru.nsu.kondrenko.model.game.Move;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.MenuEvent;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import java.awt.event.ActionEvent;
import java.awt.event.ItemEvent;
import java.util.Objects;
import java.util.Random;

public class Controller extends Adapter {
    private final Model model;

    public Controller(final Model model) {
        this.model = model;
    }

    private String singleplayerColorActionCommand = null;

    private String singlePlayerName = null;
    private String whitePlayerName = null;
    private String blackPlayerName = null;

    private Player whitePlayer = null;
    private Player blackPlayer = null;
    private Player currentPlayer = null;

    private final Random random = new Random();

    @Override
    public void actionPerformed(final ActionEvent event) {
        final Object source = event.getSource();

        if (!(source instanceof JButton)) {
            return;
        }

        final String actionCommand = ((JButton) source).getActionCommand();

        // TODO
        if (Objects.equals(actionCommand, Properties.SHOW_SCORE_BOARD)) {
            model.changeState(ModelState.SHOWING_SCORE_TABLE);
            return;
        }

        if (Objects.equals(actionCommand, Properties.START_SINGLEPLAYER)) {
            model.changeState(ModelState.STARTED_SINGLEPLAYER);
            return;
        }

        if (Objects.equals(actionCommand, Properties.START_MULTIPLAYER)) {
            model.changeState(ModelState.STARTED_MULTIPLAYER);
            return;
        }

        // TODO
        if (Objects.equals(actionCommand, Properties.SUBMIT_SINGLEPLAYER_ACTION_COMMAND)) {
            if (singlePlayerName == null || singlePlayerName.isEmpty()) {
                model.changeState(ModelState.INCORRECT_PLAYER_NAME);
            } else if (singleplayerColorActionCommand == null) {
                model.changeState(ModelState.COLOR_NOT_SELECTED);
            } else {
                final Player humanPlayer = new HumanPlayer(singlePlayerName, model, this);
                final Player botPlayer = new BotPlayer();

                if (Objects.equals(singleplayerColorActionCommand, Properties.PICKED_WHITE_COLOR)) {
                    whitePlayer = humanPlayer;
                    blackPlayer = botPlayer;
                } else if (Objects.equals(singleplayerColorActionCommand, Properties.PICKED_BLACK_COLOR)) {
                    whitePlayer = botPlayer;
                    blackPlayer = humanPlayer;
                } else {
                    final int randomNumberBetweenZeroAndOne = random.nextInt(0, 2);

                    if (randomNumberBetweenZeroAndOne == 0) {
                        whitePlayer = humanPlayer;
                        blackPlayer = botPlayer;
                    } else {
                        whitePlayer = botPlayer;
                        blackPlayer = humanPlayer;
                    }
                }

                model.restartGame();
                currentPlayer = whitePlayer;
                model.setWhitePlayer(whitePlayer);
                model.setBlackPlayer(blackPlayer);
                model.changeState(ModelState.START_OF_GAME);

                if (currentPlayer.isBot()) {
                    model.makeMove(currentPlayer.generateMove(model.getLegalMoves()));
                    currentPlayer = currentPlayer == whitePlayer ? blackPlayer : whitePlayer; // TODO
                }
                model.changeState(ModelState.WAITING_ON_MOVE);
            }

            return;
        }

        // TODO
        if (Objects.equals(actionCommand, Properties.SUBMIT_MULTIPLAYER_ACTION_COMMAND)) {
            if (whitePlayerName == null || blackPlayerName == null || whitePlayerName.isEmpty() || blackPlayerName.isEmpty()) {
                model.changeState(ModelState.INCORRECT_PLAYER_NAME);
            } else if (whitePlayerName.equals(blackPlayerName)) {
                model.changeState(ModelState.SAME_NAMES);
            } else {
                model.restartGame();
                whitePlayer = new HumanPlayer(whitePlayerName, model, this); // TODO
                blackPlayer = new HumanPlayer(blackPlayerName, model, this); // TODO
                currentPlayer = whitePlayer;
                model.setWhitePlayer(whitePlayer);
                model.setBlackPlayer(blackPlayer);
                model.changeState(ModelState.START_OF_GAME);
                model.changeState(ModelState.WAITING_ON_MOVE);
            }

            return;
        }

        currentPlayer.actionPerformed(event);
    }

    public void notifyAboutGeneratedMove(final Move move) {
        if (model.isMoveIllegal(move)) {
            model.changeState(ModelState.INCORRECT_MOVE);
            return;
        }

        model.makeMove(move);

        currentPlayer = currentPlayer == whitePlayer ? blackPlayer : whitePlayer; // TODO

        // TODO
        if (model.isGameOver()) {
            singlePlayerName = null;
            whitePlayerName = null;
            blackPlayerName = null;
            singleplayerColorActionCommand = null;
        }

        if (!currentPlayer.isBot()) {
            model.changeState(ModelState.WAITING_ON_MOVE);
        } else {
            model.makeMove(currentPlayer.generateMove(model.getLegalMoves()));
            model.changeState(ModelState.WAITING_ON_MOVE);
            currentPlayer = currentPlayer == whitePlayer ? blackPlayer : whitePlayer; // TODO
        }

        if (model.isGameOver()) {
            singlePlayerName = null;
            whitePlayerName = null;
            blackPlayerName = null;
            singleplayerColorActionCommand = null;
        }
    }

    @Override
    public void menuSelected(MenuEvent event) {
        final Object source = event.getSource();

        final String actionCommand = ((JMenu) source).getActionCommand();

        if (actionCommand.equals(Properties.BACK)) {
            model.changeState(ModelState.MAIN_MENU_CHOICE);
            singlePlayerName = null;
            whitePlayerName = null;
            blackPlayerName = null;
            singleplayerColorActionCommand = null;
            return;
        }

        if (actionCommand.equals(Properties.RESTART)) {
            model.restartGame();
            currentPlayer = whitePlayer;

            if (currentPlayer.isBot()) {
                model.makeMove(currentPlayer.generateMove(model.getLegalMoves()));
                currentPlayer = currentPlayer == whitePlayer ? blackPlayer : whitePlayer; // TODO
                model.changeState(ModelState.WAITING_ON_MOVE);
            }
        }
    }

    @Override
    public void itemStateChanged(ItemEvent event) {
        final JRadioButton source = (JRadioButton) event.getSource();

        if (source.getModel().isSelected()) {
            singleplayerColorActionCommand = source.getActionCommand();
        }
    }

    @Override
    public void insertUpdate(DocumentEvent event) {
        final Document document = event.getDocument();

        final String documentName = (String) document.getProperty(Properties.DOCUMENT_NAME_PROPERTY);

        if (Objects.equals(documentName, Properties.NAME_OF_DOCUMENT_WITH_SINGLE_PLAYER_NAME)) {
            try {
                singlePlayerName = document.getText(0, document.getLength());
            } catch (BadLocationException exception) {
                throw new RuntimeException(exception);
            }
        }

        if (Objects.equals(documentName, Properties.NAME_OF_DOCUMENT_WITH_WHITE_PLAYER_NAME)) {
            try {
                whitePlayerName = document.getText(0, document.getLength());
            } catch (BadLocationException exception) {
                throw new RuntimeException(exception);
            }
        }

        if (Objects.equals(documentName, Properties.NAME_OF_DOCUMENT_WITH_BLACK_PLAYER_NAME)) {
            try {
                blackPlayerName = document.getText(0, document.getLength());
            } catch (BadLocationException exception) {
                throw new RuntimeException(exception);
            }
        }
    }

    @Override
    public void removeUpdate(DocumentEvent event) {
        insertUpdate(event);
    }
}
