package ru.nsu.kondrenko.console.controller;

import ru.nsu.kondrenko.model.Model;
import ru.nsu.kondrenko.model.ModelState;
import ru.nsu.kondrenko.model.game.Move;

import java.util.Arrays;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

public class Controller {
    public static final String HELP_COMMAND = "h";
    public static final String QUIT_COMMAND = "q";
    public static final String RESTART_COMMAND = "r";
    public static final String TO_MENU_COMMAND = "z";
    public static final String SINGLEPLAYER_COMMAND = "s";
    public static final String MULTIPLAYER_COMMAND = "m";
    public static final String SCORE_TABLE_COMMAND = "t";
    public static final String WHITE_COLOR_COMMAND = "w";
    public static final String BLACK_COLOR_COMMAND = "b";
    public static final String RANDOM_COLOR_COMMAND = "r";

    private static final List<Character> FILES = Arrays.asList('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
    private static final List<Integer> RANKS = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8);

    private Player currentPlayer = null;
    private Player whitePlayer = null;
    private Player blackPlayer = null;

    private final Scanner scanner = new Scanner(System.in);

    private final Random random = new Random();

    private final Model model;

    public Controller(final Model model) {
        this.model = model;
    }

    public void start() {
        while (!model.quited()) {
            model.changeState(ModelState.MAIN_MENU_CHOICE);
            final String command = scanner.nextLine();
            handleMenuCommand(command);
        }
    }

    private void handleMenuCommand(final String command) {
        if (isQuitCommand(command)) {
            model.changeState(ModelState.QUITING);
            return;
        }

        if (isScoreTableCommand(command)) {
            model.changeState(ModelState.SHOWING_SCORE_TABLE);
            return;
        }

        if (isHelpCommand(command)) {
            model.changeState(ModelState.SHOWING_HELP);
            return;
        }

        if (isSingleplayerCommand(command)) {
            startSingleplayer();
            return;
        }

        if (isMultiplayerCommand(command)) {
            startMultiplayer();
            return;
        }

        model.changeState(ModelState.INCORRECT_CHOICE);
    }

    private void startSingleplayer() {
        String nameOfHumanPlayer;
        do {
            model.changeState(ModelState.ENTERING_NAME_OF_SINGLE_PLAYER);
            nameOfHumanPlayer = scanner.nextLine();
        } while (nameOfHumanPlayer.isBlank());

        String colorOfHumanPlayer;
        while (true) {
            model.changeState(ModelState.ENTERING_COLOR_OF_SINGLE_PLAYER);
            colorOfHumanPlayer = scanner.nextLine();

            final boolean isColorCorrect = isWhiteColorCommand(colorOfHumanPlayer) ||
                                           isBlackColorCommand(colorOfHumanPlayer) ||
                                           isRandomColorCommand(colorOfHumanPlayer);

            if (!isColorCorrect) {
                model.changeState(ModelState.INCORRECT_CHOICE);
                continue;
            }

            startGameCycleFromSingleplayer(nameOfHumanPlayer, colorOfHumanPlayer);

            break;
        }
    }

    private void startMultiplayer() {
        String nameOfWhitePlayer;
        do {
            model.changeState(ModelState.ENTERING_NAME_OF_WHITE_PLAYER);
            nameOfWhitePlayer = scanner.nextLine();
        } while (nameOfWhitePlayer.isBlank());

        String nameOfBlackPlayer;
        while (true) {
            model.changeState(ModelState.ENTERING_NAME_OF_BLACK_PLAYER);
            nameOfBlackPlayer = scanner.nextLine();

            if (nameOfBlackPlayer.isBlank()) {
                continue;
            }

            if (nameOfBlackPlayer.equals(nameOfWhitePlayer)) {
                model.changeState(ModelState.SAME_NAMES);
                continue;
            }

            break;
        }

        whitePlayer = new HumanPlayer(nameOfWhitePlayer);
        blackPlayer = new HumanPlayer(nameOfBlackPlayer);

        startGameCycle();
    }

    private void startGameCycleFromSingleplayer(final String nameOfHumanPlayer, final String colorOfHumanPlayer) {
        final Player humanPlayer = new HumanPlayer(nameOfHumanPlayer);
        final Player botPlayer = new BotPlayer();

        if (isWhiteColorCommand(colorOfHumanPlayer)) {
            whitePlayer = humanPlayer;
            blackPlayer = botPlayer;
        }
        if (isBlackColorCommand(colorOfHumanPlayer)) {
            whitePlayer = botPlayer;
            blackPlayer = humanPlayer;
        }
        if (isRandomColorCommand(colorOfHumanPlayer)) {
            final int randomNumberBetweenZeroAndOne = random.nextInt(0, 2);
            whitePlayer = randomNumberBetweenZeroAndOne == 0 ? botPlayer : humanPlayer;
            blackPlayer = randomNumberBetweenZeroAndOne == 1 ? botPlayer : humanPlayer;
        }

        startGameCycle();
    }

    private void startGameCycle() {
        currentPlayer = whitePlayer;
        model.setWhitePlayer(whitePlayer);
        model.setBlackPlayer(blackPlayer);
        model.restartGame();

        while (!model.isGameOver()) {
            if (!currentPlayer.isBot()) {
                model.changeState(ModelState.WAITING_ON_MOVE);
            }

            Move currentMove;

            if (currentPlayer.isBot()) {
                currentMove = currentPlayer.generateMove(model.getLegalMoves());
            } else {
                final String strMove = scanner.nextLine().toLowerCase().replaceAll(" ", "");

                if (isToMenuCommand(strMove)) {
                    break;
                }
                if (isQuitCommand(strMove)) {
                    model.changeState(ModelState.QUITING);
                    break;
                }
                if (isRestartCommand(strMove)) {
                    model.restartGame();
                    currentPlayer = whitePlayer;
                    continue;
                }
                if (!isStringValidLikeMove(strMove)) {
                    model.changeState(ModelState.INCORRECT_MOVE);
                    continue;
                }

                currentMove = currentPlayer.generateMove(strMove);
                if (model.isMoveIllegal(currentMove)) {
                    model.changeState(ModelState.INCORRECT_MOVE);
                    continue;
                }
            }

            model.makeMove(currentMove);
            switchPlayers();
        }
    }

    private void switchPlayers() {
        currentPlayer = currentPlayer == whitePlayer ? blackPlayer : whitePlayer;
    }

    private static boolean isQuitCommand(final String string) {
        return string.equals(QUIT_COMMAND);
    }

    private static boolean isScoreTableCommand(final String string) {
        return string.equals(SCORE_TABLE_COMMAND);
    }

    private static boolean isHelpCommand(final String string) {
        return string.equals(HELP_COMMAND);
    }

    private static boolean isSingleplayerCommand(final String string) {
        return string.equals(SINGLEPLAYER_COMMAND);
    }

    private static boolean isMultiplayerCommand(final String string) {
        return string.equals(MULTIPLAYER_COMMAND);
    }

    private static boolean isToMenuCommand(final String string) {
        return string.equals(TO_MENU_COMMAND);
    }

    private static boolean isRestartCommand(final String string) {
        return string.equals(RESTART_COMMAND);
    }

    private static boolean isWhiteColorCommand(final String string) {
        return string.equals(WHITE_COLOR_COMMAND);
    }

    private static boolean isBlackColorCommand(final String string) {
        return string.equals(BLACK_COLOR_COMMAND);
    }

    private static boolean isRandomColorCommand(final String string) {
        return string.equals(RANDOM_COLOR_COMMAND);
    }

    private static boolean isStringValidLikeMove(final String string) {
        if (string.length() != 4) {
            return false;
        }

        try {
            final char fileFrom = String.valueOf(string.charAt(0)).toLowerCase().charAt(0);
            final int rankFrom = Integer.parseInt(String.valueOf(string.charAt(1)));
            final char fileTo = String.valueOf(string.charAt(2)).toLowerCase().charAt(0);
            final int rankTo = Integer.parseInt(String.valueOf(string.charAt(3)));

            final boolean isFileFromValid = FILES.contains(fileFrom);
            final boolean isRankFromValid = RANKS.contains(rankFrom);
            final boolean isFileToValid = FILES.contains(fileTo);
            final boolean isRankToValid = RANKS.contains(rankTo);

            return isFileFromValid && isRankFromValid && isFileToValid && isRankToValid;
        } catch (NumberFormatException exception) {
            return false;
        }
    }
}
