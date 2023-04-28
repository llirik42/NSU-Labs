package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.model.game.Context;
import ru.nsu.kondrenko.model.game.GameState;
import ru.nsu.kondrenko.model.game.PieceContainer;

interface GameListener {
    void notifyBoardChange(final PieceContainer pieceContainer, Context context);

    void notifyGameStateChange(final GameState state, Context context);
}
