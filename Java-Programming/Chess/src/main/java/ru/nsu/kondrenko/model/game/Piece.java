package ru.nsu.kondrenko.model.game;

public enum Piece {
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,
    NONE;

    public char fen() {
        switch (this) {
            case WHITE_PAWN -> {
                return 'P';
            }

            case WHITE_KNIGHT -> {
                return 'N';
            }

            case WHITE_BISHOP -> {
                return 'B';
            }

            case WHITE_ROOK -> {
                return 'R';
            }

            case WHITE_QUEEN -> {
                return 'Q';
            }

            case WHITE_KING -> {
                return 'K';
            }

            case BLACK_PAWN -> {
                return 'p';
            }

            case BLACK_KNIGHT -> {
                return 'n';
            }

            case BLACK_BISHOP -> {
                return 'b';
            }

            case BLACK_ROOK -> {
                return 'r';
            }

            case BLACK_QUEEN -> {
                return 'q';
            }

            case BLACK_KING -> {
                return 'k';
            }

            default -> {
                return 'e';
            }
        }
    }

}
