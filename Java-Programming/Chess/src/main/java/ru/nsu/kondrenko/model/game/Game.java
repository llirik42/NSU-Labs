package ru.nsu.kondrenko.model.game;

import com.github.bhlangonijr.chesslib.Board;
import com.github.bhlangonijr.chesslib.Square;
import ru.nsu.kondrenko.model.ModelListener;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Game {
    private final PieceContainer pieceContainer = new PieceContainer();
    private final Board board = new Board();

    private final Context context = new DefaultContext();

    private ModelListener listener;

    public void setListener(final ModelListener listener) {
        this.listener = listener;
    }

    public Game() {
        updatePieceContainer();
    }

    public void makeMove(final Move move) {
        final var legalMoves = board.legalMoves();
        final var chessLibMove = modelMoveToChessLibMove(move);

        if (!legalMoves.contains(chessLibMove)) {
            listener.notifyGameStateChange(GameState.INCORRECT_MOVE, context);
        } else {
            board.doMove(chessLibMove);
            context.switchCurrentSide();
            updatePieceContainer();

            if (isDraw()) {
                listener.notifyGameStateChange(GameState.DRAW, context);
                context.setGameOver();
            }
            if (isMate()) {
                listener.notifyGameStateChange(GameState.MATE, context);
                context.setGameOver();
            }
        }
    }

    public PieceContainer getPieceContainer() {
        return pieceContainer;
    }

    public Context getContext() {
        return context;
    }

    public void changeState(final GameState state) {
        if (state == GameState.WAITING) {
            listener.notifyBoardChange(pieceContainer, context);
            return;
        }

        if (state == GameState.GAME_CLOSED) {
            context.setGameOver();
        }

        listener.notifyGameStateChange(state, context);
    }

    public boolean isDraw() {
        return board.isDraw();
    }

    public boolean isMate() {
        return board.isMated();
    }

    public boolean isGameOver() {
        return context.isGameOver();
    }

    public Side getCurrentSide() {
        return context.getCurrentSide();
    }

    public List<Move> getLegalMoves() {
        List<Move> result = new ArrayList<>();

        for (var it : board.legalMoves()) {
            result.add(chessLibMoveToModelMove(it));
        }

        return Collections.unmodifiableList(result);
    }

    private void updatePieceContainer() {
        for (int file = 1; file <= 8; file++) {
            for (int rank = 1; rank <= 8; rank++) {
                final ChessPosition chessPosition = new ChessPosition(file, rank);
                final Square square = chessPositionToSquare(chessPosition);
                final com.github.bhlangonijr.chesslib.Piece libPiece = board.getPiece(square);
                final Piece modelPiece = libPieceToModelPiece(libPiece);

                pieceContainer.placePiece(modelPiece, chessPosition);
            }
        }
    }

    private static Piece libPieceToModelPiece(final com.github.bhlangonijr.chesslib.Piece libPiece) {
        return Piece.valueOf(libPiece.toString());
    }

    private static com.github.bhlangonijr.chesslib.move.Move modelMoveToChessLibMove(final Move modelMove) {
        final Square from = chessPositionToSquare(modelMove.from());
        final Square to = chessPositionToSquare(modelMove.to());
        return new com.github.bhlangonijr.chesslib.move.Move(from, to);
    }

    private static ChessPosition squareToChessPosition(final Square square) {
        final int file = square.ordinal() % 8 + 1;
        final int rank = square.ordinal() / 8 + 1;

        return new ChessPosition(file, rank);
    }

    private static Move chessLibMoveToModelMove(final com.github.bhlangonijr.chesslib.move.Move chessLibMove) {
        final Square from = chessLibMove.getFrom();
        final Square to = chessLibMove.getTo();

        return new Move(squareToChessPosition(from), squareToChessPosition(to));
    }

    private static Square chessPositionToSquare(final ChessPosition chessPosition) {
        return Square.squareAt((chessPosition.rank() - 1) * 8 + (chessPosition.file() - 1));
    }
}
