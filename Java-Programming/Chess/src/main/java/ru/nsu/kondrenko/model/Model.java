package ru.nsu.kondrenko.model;

import ru.nsu.kondrenko.model.game.Game;
import ru.nsu.kondrenko.model.game.GameState;
import ru.nsu.kondrenko.model.game.Move;
import ru.nsu.kondrenko.model.game.Side;
import ru.nsu.kondrenko.model.statistics.StatisticsStorage;

import java.util.List;

public class Model {
    private final StatisticsStorage statisticsStorage;
    private Game game = null;
    private ModelListener listener = null;

    private boolean quited = false;

    private PlayerInfo whitePlayerInfo = null;
    private PlayerInfo blackPlayerInfo = null;
    private PlayerInfo currentPlayerInfo = null;

    public Model(final StatisticsStorage statisticsStorage) {
        this.statisticsStorage = statisticsStorage;
    }

    public StatisticsStorage getStatisticStorage() {
        return statisticsStorage;
    }

    public void setListener(final ModelListener listener) {
        this.listener = listener;
    }

    public void setWhitePlayer(final PlayerInfo playerInfo) {
        whitePlayerInfo = playerInfo;
        currentPlayerInfo = playerInfo;
    }

    public void setBlackPlayer(final PlayerInfo playerInfo) {
        blackPlayerInfo = playerInfo;
    }

    public Side getCurrentSide() {
        return game.getCurrentSide();
    }

    public void makeMove(final Move move) {
        game.makeMove(move);

        if (game.isMate()) {
            listener.notifyModelStateChange(ModelState.MATE, this);

            if (!currentPlayerInfo.isBot()) {
                statisticsStorage.addWinner(currentPlayerInfo.getName());
                statisticsStorage.storeRecords();
            }

            return;
        }

        if (game.isDraw()) {
            listener.notifyModelStateChange(ModelState.DRAW, this);
            return;
        }

        switchPlayers();
    }

    public void changeState(final ModelState modelState) {
        if (modelState == ModelState.QUITING) {
            quited = true;
        }

        if (modelState == ModelState.WAITING_ON_MOVE && !currentPlayerInfo.isBot()) {
            game.changeState(GameState.WAITING);
        }

        if (modelState == ModelState.SHOWING_SCORE_TABLE) {
            statisticsStorage.fetchRecords();
        }

        listener.notifyModelStateChange(modelState, this);
    }

    public Game getGame() {
        return game;
    }

    public boolean isMoveIllegal(final Move move) {
        return !game.getLegalMoves().contains(move);
    }

    public List<Move> getLegalMoves() {
        return game.getLegalMoves();
    }

    public void restartGame() {
        game = new Game();
        game.setListener(listener);
        currentPlayerInfo = whitePlayerInfo;
        listener.notifyModelStateChange(ModelState.RESTARTING_GAME, this);
    }

    public boolean quited() {
        return quited;
    }

    public boolean isGameOver() {
        return game.isGameOver();
    }

    private void switchPlayers() {
        currentPlayerInfo = currentPlayerInfo == whitePlayerInfo ? blackPlayerInfo : whitePlayerInfo;
    }
}
