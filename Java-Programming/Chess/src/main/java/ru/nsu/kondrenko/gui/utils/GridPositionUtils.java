package ru.nsu.kondrenko.gui.utils;

import ru.nsu.kondrenko.model.game.ChessPosition;
import ru.nsu.kondrenko.model.game.Side;

public final class GridPositionUtils {
    private GridPositionUtils() {}

    public static ChessPosition gridPositionToChessPosition(Side side, int x, int y) {
        return side == Side.WHITE
                ? new ChessPosition(x + 1, 8 - y)
                : new ChessPosition(8 - x, y + 1);
    }

    public static ChessPosition gridPositionToChessPosition(Side side, GridPosition position) {
        return gridPositionToChessPosition(side, position.x(), position.y());
    }
}
