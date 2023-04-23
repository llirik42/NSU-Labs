package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.controller.Properties;
import ru.nsu.kondrenko.model.game.Context;
import ru.nsu.kondrenko.model.game.GameState;
import ru.nsu.kondrenko.model.game.PieceContainer;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

public class MainMenuFrame extends FixedSizeFrame {
    private static final Dimension FRAME_DIMENSION = new Dimension(600, 600);
    private static final Dimension BUTTON_DIMENSION = new Dimension(400, 150);
    private static final Font BUTTON_FONT = new Font("Ubuntu", Font.PLAIN, 36);

    public MainMenuFrame(final ActionListener actionListener) {
        super(FRAME_DIMENSION);

        setLayout(new GridBagLayout());
        GridBagConstraints gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridwidth = GridBagConstraints.REMAINDER;
        gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;

        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        final JButton singlePlayerButton = new JButton("Singleplayer");
        final JButton multiPlayerButton = new JButton("Multiplayer");
        final JButton statisticsButton = new JButton("Statistics");

        singlePlayerButton.setActionCommand(Properties.START_SINGLEPLAYER);
        multiPlayerButton.setActionCommand(Properties.START_MULTIPLAYER);
        statisticsButton.setActionCommand(Properties.SHOW_SCORE_BOARD);

        singlePlayerButton.setPreferredSize(BUTTON_DIMENSION);
        multiPlayerButton.setPreferredSize(BUTTON_DIMENSION);
        statisticsButton.setPreferredSize(BUTTON_DIMENSION);

        singlePlayerButton.addActionListener(actionListener);
        multiPlayerButton.addActionListener(actionListener);
        statisticsButton.addActionListener(actionListener);

        singlePlayerButton.setFont(BUTTON_FONT);
        multiPlayerButton.setFont(BUTTON_FONT);
        statisticsButton.setFont(BUTTON_FONT);

        add(singlePlayerButton, gridBagConstraints);
        add(multiPlayerButton, gridBagConstraints);
        add(statisticsButton, gridBagConstraints);

        SwingUtilities.invokeLater(this::pack);
    }

    @Override
    public void notifyBoardChange(PieceContainer pieceContainer, Context context) {

    }

    @Override
    public void notifyGameStateChange(GameState state, Context context) {

    }
}
