package ru.nsu.kondrenko.model;

import ru.nsu.kondrenko.model.game.Context;
import ru.nsu.kondrenko.model.game.GameState;
import ru.nsu.kondrenko.model.game.PieceContainer;

public interface ModelListener {
    void notifyModelStateChange(final ModelState modelState, Model model);

    void notifyBoardChange(final PieceContainer pieceContainer, Context context);

    void notifyGameStateChange(final GameState state, Context context);
}
