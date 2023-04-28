package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.controller.Controller;
import ru.nsu.kondrenko.gui.controller.Properties;
import ru.nsu.kondrenko.model.game.Context;
import ru.nsu.kondrenko.model.game.GameState;
import ru.nsu.kondrenko.model.game.PieceContainer;

import javax.swing.*;
import java.awt.*;

public class MultiplayerFrame extends FixedSizeFrame {
    private static final Dimension FRAME_DIMENSION = new Dimension(600, 600);
    private static final Dimension BUTTON_DIMENSION = new Dimension(400, 150);
    private static final Font BUTTON_FONT = new Font("Ubuntu", Font.PLAIN, 36);

    public MultiplayerFrame(Controller controller) {
        super(FRAME_DIMENSION);

        setLayout(new GridBagLayout());
        GridBagConstraints gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridwidth = GridBagConstraints.REMAINDER;
        gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;

        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        final JTextField whitePlayerNameField = new JTextField();
        final JTextField blackPlayerNameField = new JTextField();
        final JLabel whitePlayerNameLabel = new JLabel("White player name");
        final JLabel blackPlayerNameLabel = new JLabel("Black player name");
        final JButton submitButton = new JButton("Submit");

        whitePlayerNameField.setActionCommand(Properties.WHITE_PLAYER_NAME_ACTION_COMMAND);
        blackPlayerNameField.setActionCommand(Properties.BLACK_PLAYER_NAME_ACTION_COMMAND);
        submitButton.setActionCommand(Properties.SUBMIT_MULTIPLAYER_ACTION_COMMAND);

        whitePlayerNameField.setPreferredSize(BUTTON_DIMENSION);
        blackPlayerNameField.setPreferredSize(BUTTON_DIMENSION);
        whitePlayerNameLabel.setPreferredSize(BUTTON_DIMENSION);
        blackPlayerNameLabel.setPreferredSize(BUTTON_DIMENSION);
        submitButton.setPreferredSize(BUTTON_DIMENSION);

        whitePlayerNameField.getDocument().addDocumentListener(controller);
        blackPlayerNameField.getDocument().addDocumentListener(controller);
        whitePlayerNameField.getDocument().putProperty(Properties.DOCUMENT_NAME_PROPERTY, Properties.NAME_OF_DOCUMENT_WITH_WHITE_PLAYER_NAME);
        blackPlayerNameField.getDocument().putProperty(Properties.DOCUMENT_NAME_PROPERTY, Properties.NAME_OF_DOCUMENT_WITH_BLACK_PLAYER_NAME);
        submitButton.addActionListener(controller);

        whitePlayerNameField.setFont(BUTTON_FONT);
        blackPlayerNameField.setFont(BUTTON_FONT);
        whitePlayerNameLabel.setFont(BUTTON_FONT);
        blackPlayerNameLabel.setFont(BUTTON_FONT);
        submitButton.setFont(BUTTON_FONT);

        final JMenu menu = new JMenu("back");
        menu.setActionCommand(Properties.BACK);
        menu.addMenuListener(controller);

        final JMenuBar menuBar = new JMenuBar();
        menuBar.add(menu);
        setJMenuBar(menuBar);

        add(whitePlayerNameLabel, gridBagConstraints);
        add(whitePlayerNameField, gridBagConstraints);
        add(blackPlayerNameLabel, gridBagConstraints);
        add(blackPlayerNameField, gridBagConstraints);
        add(submitButton, gridBagConstraints);

        SwingUtilities.invokeLater(this::pack);
    }

    @Override
    public void notifyBoardChange(PieceContainer pieceContainer, Context context) {

    }

    @Override
    public void notifyGameStateChange(GameState state, Context context) {

    }
}
