package ru.nsu.kondrenko.console.view;

import ru.nsu.kondrenko.console.controller.Controller;
import ru.nsu.kondrenko.model.Model;
import ru.nsu.kondrenko.model.ModelListener;
import ru.nsu.kondrenko.model.ModelState;
import ru.nsu.kondrenko.model.game.*;
import ru.nsu.kondrenko.model.statistics.PlayerScoreRecord;
import ru.nsu.kondrenko.model.statistics.StatisticsStorage;

public class View implements ModelListener {
    @Override
    public void notifyModelStateChange(final ModelState modelState, final Model model) {
        switch (modelState) {
            case MAIN_MENU_CHOICE -> printMainMenu();

            case QUITING -> printQuiting();

            case SHOWING_SCORE_TABLE -> printScoreTable(model.getStatisticStorage());

            case SHOWING_HELP -> printHelp();

            case INCORRECT_CHOICE -> printIncorrectChoice();

            case ENTERING_COLOR_OF_SINGLE_PLAYER -> printEnterColorOfSinglePlayer();

            case ENTERING_NAME_OF_SINGLE_PLAYER -> printEnterNameOfSinglePlayer();

            case ENTERING_NAME_OF_WHITE_PLAYER -> printEnterNameOfWhitePlayer();

            case ENTERING_NAME_OF_BLACK_PLAYER -> printEnterNameOfBlackPlayer();

            case SAME_NAMES -> printSameNamesError();

            case WAITING_ON_MOVE -> printWaitingOnMove();

            case INCORRECT_MOVE -> printIncorrectMove();
        }
    }

    @Override
    public void notifyBoardChange(final PieceContainer pieceContainer, final Context context) {

        if (context.getCurrentSide() == Side.WHITE) {
            printBoardForWhitePlayer(pieceContainer);
        } else {
            printBoardForBlackPlayer(pieceContainer);
        }
    }

    @Override
    public void notifyGameStateChange(final GameState state, final Context context) {
        switch (state) {
            case MATE -> System.out.println("Mate!\n");

            case DRAW -> System.out.println("Draw!\n");
        }
    }

    private static void printMainMenu() {
        final String output = String.format("""
                        What do you want to do?
                          %s — singleplayer
                          %s — multiplayer
                          %s — score table
                          %s — help
                          %s — quit""",
                Controller.SINGLEPLAYER_COMMAND,
                Controller.MULTIPLAYER_COMMAND,
                Controller.SCORE_TABLE_COMMAND,
                Controller.HELP_COMMAND,
                Controller.QUIT_COMMAND);

        System.out.print(output + "\n\nYour choice: ");
    }

    private static void printQuiting() {
        System.out.println("Quiting ...\n");
    }

    private static void printScoreTable(final StatisticsStorage statisticsStorage) {
        System.out.printf("        %-30s        score%n", "player");

        for (int i = 0; i < statisticsStorage.getCountOfRecords(); i++) {
            final PlayerScoreRecord record = statisticsStorage.getRecord(i);
            System.out.printf("%-8d%-30s        %d\n", i + 1, record.playerName(), record.score());
        }
        System.out.println();
    }

    private static void printHelp() {
        System.out.println("""
                This app represents a console version of a famous game "chess".
                Choose "singleplayer" or "multiplayer" to start.
                Format of move: e2e4, E2E4, e2 e3.
                """);
    }

    private void printIncorrectChoice() {
        System.out.println("Incorrect choice!\n");
    }

    private void printEnterNameOfSinglePlayer() {
        System.out.print("Your name: ");
    }

    private void printEnterColorOfSinglePlayer() {
        final String output = String.format("""
                        What color do you choose?
                          %s — white
                          %s — black
                          %s — random""",
                Controller.WHITE_COLOR_COMMAND,
                Controller.BLACK_COLOR_COMMAND,
                Controller.RANDOM_COLOR_COMMAND);

        System.out.print(output + "\n\nYour choice: ");
    }

    private void printEnterNameOfWhitePlayer() {
        System.out.print("Name of white player: ");
    }

    private void printEnterNameOfBlackPlayer() {
        System.out.print("Name of black player: ");
    }

    private void printSameNamesError() {
        System.out.println("The names of the players cannot match!\n");
    }

    private void printWaitingOnMove() {
        System.out.printf("Your move (%s — quit, %s — main menu, %s — restart): ",
                Controller.QUIT_COMMAND,
                Controller.TO_MENU_COMMAND,
                Controller.RESTART_COMMAND);
    }

    private void printIncorrectMove() {
        System.out.println("Incorrect move!\n");
    }

    private void printBoardForWhitePlayer(final PieceContainer pieceContainer) {
        System.out.println("\n      a  b  c  d  e  f  g  h\n");

        for (int rank = 8; rank >= 1; rank--) {
            System.out.print(rank + "     ");

            for (int file = 1; file <= 8; file++) {
                printPieceBlock(pieceContainer, file, rank);
            }
            System.out.print("   " + rank);
            System.out.println();
        }

        System.out.println("\n      a  b  c  d  e  f  g  h\n");
    }

    private void printBoardForBlackPlayer(final PieceContainer pieceContainer) {
        System.out.println("\n      h  g  f  e  d  c  b  a\n");

        for (int rank = 1; rank <= 8; rank++) {
            System.out.print(rank + "     ");

            for (int file = 8; file >= 1; file--) {
                printPieceBlock(pieceContainer, file, rank);
            }
            System.out.print("   " + rank);
            System.out.println();
        }

        System.out.println("\n      h  g  f  e  d  c  b  a\n");
    }

    private void printPieceBlock(final PieceContainer pieceContainer, final int file, final int rank) {
        Piece piece =  pieceContainer.getPiece(file, rank);

        if (piece == Piece.NONE) {
            System.out.print(" ");
        } else {
            System.out.print(getFenSymbol(piece));
        }
        System.out.print("  ");
    }

    private char getFenSymbol(final Piece piece) {
        return piece == Piece.NONE ? ' ' : piece.fen();
    }
}
