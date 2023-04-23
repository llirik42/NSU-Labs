package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.controller.Adapter;
import ru.nsu.kondrenko.gui.controller.Properties;
import ru.nsu.kondrenko.gui.exceptions.IconNotFoundException;
import ru.nsu.kondrenko.gui.utils.GridPositionUtils;
import ru.nsu.kondrenko.model.Model;
import ru.nsu.kondrenko.model.ModelListener;
import ru.nsu.kondrenko.model.ModelState;
import ru.nsu.kondrenko.model.game.*;

import javax.swing.*;
import java.awt.*;
import java.util.Map;

public class GameFrame extends FixedSizeFrame implements ModelListener {
    private final Map<Piece, Icon> piecesIcons = PieceIconsLoader.getPiecesIcons();

    private static final Dimension FRAME_DIMENSION = new Dimension(600, 600);

    private final JButton[][] buttons = new JButton[8][8];

    private final JLabel[] upperRow = new JLabel[8];
    private final JLabel[] bottomRow = new JLabel[8];
    private final JLabel[] leftFile = new JLabel[8];
    private final JLabel[] rightFile = new JLabel[8];

    {
        initLabelsContainer(upperRow);
        initLabelsContainer(bottomRow);
        initLabelsContainer(leftFile);
        initLabelsContainer(rightFile);
    }

    public GameFrame(final Adapter adapter) throws IconNotFoundException {
        super(FRAME_DIMENSION);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);

        setLayout(new GridLayout(10, 10));

        initButtons(adapter);

        addRow(upperRow);
        addButtonsAndFiles();
        addRow(bottomRow);

        final JMenu menu1 = new JMenu("back");
        menu1.setActionCommand(Properties.BACK);
        menu1.addMenuListener(adapter);

        final JMenu menu2 = new JMenu("restart");
        menu2.setActionCommand(Properties.RESTART);
        menu2.addMenuListener(adapter);

        final JMenuBar menuBar = new JMenuBar();
        menuBar.add(menu1);
        menuBar.add(menu2);
        setJMenuBar(menuBar);

        SwingUtilities.invokeLater(this::pack);
    }

    @Override
    public void notifyModelStateChange(final ModelState modelState, final Model model) {}

    @Override
    public void notifyBoardChange(final PieceContainer pieceContainer, final Context context) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                ChessPosition chessPosition = GridPositionUtils.gridPositionToChessPosition(context.getCurrentSide(), x, y);
                Piece piece = pieceContainer.getPiece(chessPosition);
                Icon icon = piece == Piece.NONE ? null : piecesIcons.get(piece);
                setButtonIcon(icon, x, y);
            }
        }

        if (context.getCurrentSide() == Side.WHITE) {
            for (int i = 0; i < 8; i++) {
                upperRow[i].setText(fileToString(i + 1));
                bottomRow[i].setText(fileToString(i + 1));
                leftFile[i].setText(String.valueOf(8 - i));
                rightFile[i].setText(String.valueOf(8 - i));
            }
        } else {
            for (int i = 0; i < 8; i++) {
                upperRow[i].setText(fileToString(8 - i));
                bottomRow[i].setText(fileToString(8 - i));
                leftFile[i].setText(String.valueOf(i + 1));
                rightFile[i].setText(String.valueOf(i + 1));
            }
        }
    }

    @Override
    public void notifyGameStateChange(final GameState state, final Context context) {
        if (state == GameState.INCORRECT_MOVE || state == GameState.IDLE) {
            return;
        }

        if (state == GameState.GAME_CLOSED) {
            setVisible(false);
            dispose();
            return;
        }

        final String message = state == GameState.DRAW ? "Draw!" : "Mate!";
        JOptionPane.showMessageDialog(this, message);
    }

    private void initLabelsContainer(final JLabel[] container) {
        for (int i = 0; i < container.length; i++) {
            container[i] = createDefaultLabel();
        }
    }

    private void initButtons(final Adapter adapter) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                final JButton button = new JButton();
                button.setActionCommand(String.format("%d%d", y, x));
                button.addActionListener(adapter);
                button.setBackground((x + y) % 2 == 1 ? Color.DARK_GRAY : Color.WHITE);
                buttons[x][y] = button;
            }
        }
    }

    private void addRow(final JLabel[] row) {
        add(createEmptyLabel());
        for (int i = 0; i < 8; i++) {
            add(row[i]);
        }
        add(createEmptyLabel());
    }

    private void addButtonsAndFiles() {
        for (int y = 0; y < 8; y++) {
            add(leftFile[y]);
            for (int x = 0; x < 8; x++) {
                add(buttons[x][y]);
            }
            add(rightFile[y]);
        }
    }

    private void setButtonIcon(final Icon icon, final int x, final int y) {
        buttons[x][y].setIcon(icon);
    }

    private static JLabel createDefaultLabel() {
        return new JLabel("", SwingConstants.CENTER);
    }

    private static JLabel createEmptyLabel() {
        return new JLabel();
    }

    private static String fileToString(final int file) {
        return switch (file) {
            case 1 -> "a";
            case 2 -> "b";
            case 3 -> "c";
            case 4 -> "d";
            case 5 -> "e";
            case 6 -> "f";
            case 7 -> "g";
            case 8 -> "h";
            default -> null;
        };
    }
}
