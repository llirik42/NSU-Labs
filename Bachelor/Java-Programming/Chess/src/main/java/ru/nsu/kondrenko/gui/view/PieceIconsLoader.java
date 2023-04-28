package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.exceptions.IconNotFoundException;
import ru.nsu.kondrenko.model.game.Piece;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.io.InputStream;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

final class PieceIconsLoader {
    private static final int ICON_SCALE = 60;

    private static final Map<Piece, Icon> piecesIcons = new HashMap<>();

    static {
        loadPiecesImages();
    }

    public static Map<Piece, Icon> getPiecesIcons() {
        return Collections.unmodifiableMap(piecesIcons);
    }

    private static void loadPiecesImages() {
        putPieceIcon(Piece.WHITE_PAWN, "/pieces/wP.png");
        putPieceIcon(Piece.WHITE_KNIGHT, "/pieces/wN.png");
        putPieceIcon(Piece.WHITE_BISHOP, "/pieces/wB.png");
        putPieceIcon(Piece.WHITE_ROOK, "/pieces/wR.png");
        putPieceIcon(Piece.WHITE_QUEEN, "/pieces/wQ.png");
        putPieceIcon(Piece.WHITE_KING, "/pieces/wK.png");
        putPieceIcon(Piece.BLACK_PAWN, "/pieces/bP.png");
        putPieceIcon(Piece.BLACK_KNIGHT, "/pieces/bN.png");
        putPieceIcon(Piece.BLACK_BISHOP, "/pieces/bB.png");
        putPieceIcon(Piece.BLACK_ROOK, "/pieces/bR.png");
        putPieceIcon(Piece.BLACK_QUEEN, "/pieces/bQ.png");
        putPieceIcon(Piece.BLACK_KING, "/pieces/bK.png");
    }

    private static void putPieceIcon(final Piece piece, final String path) throws IconNotFoundException {
        piecesIcons.put(piece, loadImageIcon(path));
    }

    private static Icon loadImageIcon(final String path) throws IconNotFoundException {
        try (InputStream inputStream = PieceIconsLoader.class.getResourceAsStream(path)) {
            if (inputStream == null) {
                throw new IconNotFoundException(String.format("Icon \"%s\" not found", path));
            }

            return new ImageIcon(ImageIO.read(inputStream).
                    getScaledInstance(ICON_SCALE, ICON_SCALE, Image.SCALE_SMOOTH));
        } catch (IOException exception) {
            throw new IconNotFoundException(exception);
        }
    }
}
