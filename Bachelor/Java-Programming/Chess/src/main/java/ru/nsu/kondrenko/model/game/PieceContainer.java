package ru.nsu.kondrenko.model.game;

public class PieceContainer {
    private record InsidePosition(int x, int y) {}

    private final Piece[][] board = new Piece[8][8];

    public Piece getPiece(int file, int rank) {
        final InsidePosition insidePosition = calculateInsidePosition(file, rank);
        return board[insidePosition.x][insidePosition.y];
    }

    public Piece getPiece(ChessPosition chessPosition) {
        return getPiece(chessPosition.file(), chessPosition.rank());
    }

    public void placePiece(Piece piece, int file, int rank) {
        final InsidePosition insidePosition = calculateInsidePosition(file, rank);
        board[insidePosition.x][insidePosition.y] = piece;
    }

    public void placePiece(Piece piece, ChessPosition position) {
        placePiece(piece, position.file(), position.rank());
    }

    private static InsidePosition calculateInsidePosition(int file, int rank) {
        return new InsidePosition(file - 1, 8 - rank);
    }
}
